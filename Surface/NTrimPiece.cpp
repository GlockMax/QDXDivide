// NTrimPiece.cpp: implementation of the NTrimPiece class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NTrimPiece.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NTrimPiece::NTrimPiece():NSquarePiece()
{
	num = 0;
}

NTrimPiece::~NTrimPiece()
{

}

void NTrimPiece::Draw()
{
	int i, j=0, l=0, n=3;// Points Number in a contour
	int step = 8;
	GLfloat *ctlarray = new GLfloat [3*n*4];

	for (i=0; i<3; i++)
	{
		for (j=0; j<3; j++)
		{
			ctlarray[(j+l)*4] = (float)pc[j][i].GetX();
			ctlarray[(j+l)*4+1] = (float)pc[j][i].GetY();
			ctlarray[(j+l)*4+2] = (float)pc[j][i].GetZ();
			ctlarray[(j+l)*4+3] = (float)pc[j][i].GetH();
		}
		l+=3;
	}

	GLint un = 2*step, vn = step;
	GLfloat u1=0.,u2=1.,v1=0.,v2=1.;

//	glDisable(GL_LIGHTING);
//	glDisable(GL_CULL_FACE);

	glPushAttrib(GL_EVAL_BIT);
	glMapGrid2f( un, u1, u2, vn, v2, v1);
	glEnable(GL_MAP2_VERTEX_4);

	glMap2f(GL_MAP2_VERTEX_4,
		0.,1.,4,3,
		0.,1.,4*n,3,
		ctlarray);
	glEvalMesh2( GL_FILL, 0, un, 0, vn);
//	glEvalMesh2( GL_LINE, 0, un, 0, vn);

	glPopAttrib();

	glEnable(GL_LIGHTING);

	delete []ctlarray;

/*
	GLUnurbsObj *theNurb;
//	GLint uknot_count=PointsNumber +3,vknot_count=6;
//	GLint uorder=3,vorder=3;
//	GLint u_stride=3,v_stride=3*PointsNumber;
//	GLint type = GL_MAP2_VERTEX_4;

//	GLfloat	vknot[6]={0.f,0.f,0.f,1.f,1.f,1.f};
//	GLfloat	uknot[6]={0.f,0.f,0.f,1.f,1.f,1.f};

	GLenum errCode;
	if((errCode = glGetError())!=GL_NO_ERROR)
	{	
		AfxMessageBox("Draw 0:Ошибка OpenGL"+CString(gluErrorString(errCode)));
	}
	
	ASSERT(theNurb); 

//	GLfloat mat_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
//	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
//	GLfloat mat_shininess[] = { 100.0 };

	GLfloat knots[6] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0};

	GLfloat **EssayLine;
	
	EssayLine = new GLfloat *[uvps.GetSize()];

	for (i=0; i<uvps.GetSize(); i++)
	{
		EssayLine[i] = new GLfloat [2];
		
		EssayLine[i][0] = (GLfloat) uvps[i].GetU();
		EssayLine[i][1] = (GLfloat) uvps[i].GetV();
	}

	GLfloat edgePt[5][2] = 
      {{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}, 
       {0.0, 0.0}};
	GLfloat curvePt[4][2] = 
	  {{0.25, 0.5}, {0.25, 0.75}, {0.75, 0.75}, {0.75, 0.5}};
	GLfloat curveKnots[8] = 
	  {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
	GLfloat pwlPt[4][2] =  
	  {{0.75, 0.5}, {0.5, 0.25}, {0.25, 0.5}};

//	glClearColor (0.0, 0.0, 0.0, 0.0);
//	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
//	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
//	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	theNurb = gluNewNurbsRenderer();
	gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
	gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gluBeginSurface(theNurb);
	gluNurbsSurface(theNurb, 
				   6, knots, 6, knots,
				   4, 4*n, ctlarray, 
				   3, 3, GL_MAP2_VERTEX_4);
	//	gluBeginTrim (theNurb);
	//		gluPwlCurve (theNurb, 5, 
	//			&edgePt[0][0], 2,
    //            GLU_MAP1_TRIM_2);
	//	gluEndTrim (theNurb);
	//	gluBeginTrim (theNurb);
	//		gluPwlCurve (theNurb, 3, &pwlPt[0][0], 2,
    //             GLU_MAP1_TRIM_2);
	//	gluEndTrim (theNurb);

	//	gluBeginTrim (theNurb);
	//		gluPwlCurve (theNurb, 5, 
	//			&edgePt[0][0], 2,
    //            GLU_MAP1_TRIM_2);
	//	gluEndTrim (theNurb);
	//	gluBeginTrim (theNurb);
	//		gluNurbsCurve (theNurb, 8, curveKnots, 2, 
    //             &curvePt[0][0], 4, GLU_MAP1_TRIM_2);
	//		gluPwlCurve (theNurb, 3, &pwlPt[0][0], 2,
    //             GLU_MAP1_TRIM_2);
	//	gluEndTrim (theNurb);

	gluEndSurface(theNurb);

	glEnable(GL_LIGHTING);

	for (i=0; i<uvps.GetSize(); i++)
		delete EssayLine[i];
	delete []EssayLine;

	delete []ctlarray;*/
}

// добавление uv точки
int NTrimPiece::AddUVPoint(NUVPoint uv)
{
	uvps.Add(uv);
	num = uvps.GetSize();

	return num;
}

// Удаление uv точки
bool NTrimPiece::RemoveUVPoint(int index)
{
	if (index <0 || index>=num || num==0)
		return false;

	if (index == 1)
	{
		ClearUVPoint();
		return true;
	}
	for (int i=0; i<3; i++)
		uvps.RemoveAt(index,3);
	num--;

	return true;
}

// очистка всего массива
void NTrimPiece::ClearUVPoint()
{
	if (num==0) return;
	num=0;
	uvps.RemoveAll();
}
