/*
  SOLID - Software Library for Interference Detection
  Copyright (C) 1997-1998  Gino van den Bergen

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the Free
  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

  Please send remarks, questions and bug reports to gino@win.tue.nl,
  or write to:
                  Gino van den Bergen
		  Department of Mathematics and Computing Science
		  Eindhoven University of Technology
		  P.O. Box 513, 5600 MB Eindhoven, The Netherlands
*/

/* CHANGED BY ALEX for use in NCManager */

#include "..\StdAfx.h"
#include "DT_Scene.h"
#include "DT_Object.h"
#include "BP_Scene.h"
#include "COConv.h"

//#define DEBUG

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

static void beginOverlap(void *client_data, void *object1, void *object2)
{
	DT_Encounter e(reinterpret_cast<DT_Object *>(object1), reinterpret_cast<DT_Object *>(object2));

#ifdef DEBUG	
	std::cout << "Begin: " << e << std::endl; 
#endif

	reinterpret_cast<DT_Scene *>(client_data)->addEncounter(e);
}


static void endOverlap(void *client_data, void *object1, void *object2) 
{
	DT_Encounter e((DT_Object *)object1, (DT_Object *)object2);

#ifdef DEBUG
	std::cout << "End:   " << e << std::endl; 
#endif
	
	reinterpret_cast<DT_Scene *>(client_data)->removeEncounter(e);
}

struct DT_RayCastData {
	DT_RayCastData(const void *ignore) 
	  : m_ignore(ignore) 
	{}

	const void  *m_ignore;
	MT_Vector3  m_normal;
};

static bool objectRayCast(void *client_data, 
						  void *object,  
						  const DT_Vector3 source,
						  const DT_Vector3 target,
						  DT_Scalar *lambda) 
{
	DT_RayCastData *data = static_cast<DT_RayCastData *>(client_data); 
	if (((DT_Object *)object)->getClientObject() != data->m_ignore)
	{
		MT_Scalar param = MT_Scalar(*lambda);
		
		if (((DT_Object *)object)->ray_cast(MT_Point3(source), MT_Point3(target),
											param, data->m_normal))
		{
			*lambda = param;
			return true;
		}
	}
	return false;
}
//BP_SceneHandle BP_CreateScene(void *client_data,
//							  BP_Callback beginOverlap,
//							  BP_Callback endOverlap)
//{
//	return (BP_SceneHandle)new BP_Scene(client_data, 
//										beginOverlap, 
//										endOverlap);
//}

DT_Scene::DT_Scene() 
	: m_broadphase((BP_SceneHandle)new BP_Scene(this, 
										&beginOverlap, 
										&endOverlap)),
	  m_state(0x0)
{}

DT_Scene::~DT_Scene()
{
//void BP_DestroyScene(BP_SceneHandle scene)
//{
//	delete (BP_Scene *)scene;
//}
//	BP_DestroyScene(m_broadphase);
	delete (BP_Scene *)m_broadphase;
}

void DT_Scene::addObject(DT_Object &object)
{
	const MT_BBox& bbox = object.getBBox();
	DT_Vector3 min, max;
	bbox.getMin().getValue(min);
	bbox.getMax().getValue(max);
//BP_ProxyHandle BP_CreateProxy(BP_SceneHandle scene, void *object,
//							  const DT_Vector3 min, const DT_Vector3 max)
//{
//	return (BP_ProxyHandle)
//		((BP_Scene *)scene)->createProxy(object, min, max);
//}
//    BP_ProxyHandle proxy = BP_CreateProxy(m_broadphase, &object, min, max);
    BP_ProxyHandle proxy = (BP_ProxyHandle)
		((BP_Scene *)m_broadphase)->createProxy(&object, min, max);
	
#ifdef DEBUG
	DT_EncounterTable::iterator it;	
	std::cout << "Add " << &object << ':';
	for (it = m_encounterTable.begin(); it != m_encounterTable.end(); ++it) {
		std::cout << ' ' << (*it);
	}
	std::cout << std::endl;
#endif
	object.addProxy(proxy);
    m_objectList.push_back(std::make_pair(&object, proxy));
}



void DT_Scene::removeObject(DT_Object& object)
{
    T_ObjectList::iterator it = m_objectList.begin();

    while (it != m_objectList.end() && &object != (*it).first)
	{
        ++it;
    }

    if (it != m_objectList.end())
	{
		object.removeProxy((*it).second);
//void BP_DestroyProxy(BP_SceneHandle scene, BP_ProxyHandle proxy) 
//{
//	((BP_Scene *)scene)->destroyProxy((BP_Proxy *)proxy);
//}
//        BP_DestroyProxy(m_broadphase, (*it).second);
        ((BP_Scene *)m_broadphase)->destroyProxy((BP_Proxy *)(*it).second);
		m_objectList.erase(it);

#ifdef DEBUG
		std::cout << "Remove " << &object << ':';
		DT_EncounterTable::iterator it;	
		for (it = m_encounterTable.begin(); it != m_encounterTable.end(); ++it)
		{
			std::cout << ' ' << (*it);
			assert((*it).first() != &object &&
				   (*it).second() != &object);
		}
		std::cout << std::endl;
#endif
    }
}



int DT_Scene::handleCollisions(const DT_RespTable *respTable)
{
    int count = 0;

    assert(respTable);

	m_state |= TESTING;

	DT_EncounterTable::iterator it;	
	for (it = m_encounterTable.begin(); it != m_encounterTable.end(); ++it)
	{
		if ((*it).exactTest(respTable, count))
		{
			break;
        }
    }

	m_state &= ~TESTING;

    return count;
}

void *DT_Scene::rayCast(const void *ignore_client,
						const DT_Vector3 source, const DT_Vector3 target, 
						DT_Scalar& lambda, DT_Vector3 normal) const 
{
	DT_RayCastData data(ignore_client);
//void *BP_RayCast(BP_SceneHandle scene, 
//				 BP_RayCastCallback objectRayCast,
//				 void *client_data,
//				 const DT_Vector3 source,
//				 const DT_Vector3 target,
//				 DT_Scalar *lambda) 
//{
//	return ((BP_Scene *)scene)->rayCast(objectRayCast,
//										client_data,
//										source,	target,
//										*lambda);
//}
//	DT_Object *object = (DT_Object *)BP_RayCast(m_broadphase, 
//												&objectRayCast, 
//												&data, 
//												source, target,
//												&lambda);
	DT_Object *object = (DT_Object *)((BP_Scene *)m_broadphase)->rayCast(&objectRayCast,
										&data,
										source,	target,
										lambda);
	if (object)
	{
		data.m_normal.getValue(normal);
		return object->getClientObject();
	}
	
	return 0;
}
