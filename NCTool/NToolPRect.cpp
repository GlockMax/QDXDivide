// NToolPRect.cpp: implementation of the NToolPRect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NToolPRect.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NToolPRect::NToolPRect():NToolPunch()
{
	QObj = gluNewQuadric();
	X = 5; Y= 30; H=20; R=2.5; Angle = 30;
}

NToolPRect::~NToolPRect()
{
	gluDeleteQuadric(QObj);
}

void NToolPRect::Draw(RenderMode GlobRMode, int step)
{
	glColor4fv(Color.array);
	if(!Visible)
	{
		NTool::Draw(GlobRMode, 1);// Draw tool marker
		return;
	}
	double	nx = -X/2.+R,
			px = -nx,
			ny = -Y/2.+R,
			py = - ny,
			nz = Zdisp,
			pz = nz + H;
	glPushMatrix();
	glRotated(Angle,0,0,1);
	glBegin(GL_QUADS);
		glNormal3d(-1.,0.,0.);
		glVertex3d(-0.5*X,ny,nz);
		glVertex3d(-0.5*X,ny,pz);
		glVertex3d(-0.5*X,py,pz);
		glVertex3d(-0.5*X,py,nz);

		glNormal3d(1.,0.,0.);
		glVertex3d(0.5*X,ny,nz);
		glVertex3d(0.5*X,py,nz);
		glVertex3d(0.5*X,py,pz);
		glVertex3d(0.5*X,ny,pz);

		glNormal3d(0.,0.,-1.);
		glVertex3d(nx,0.5*Y,nz);
		glVertex3d(px,0.5*Y,nz);
		glVertex3d(px,-0.5*Y,nz);
		glVertex3d(nx,-0.5*Y,nz);

//		glNormal3d(0.,0.,-1.);
		glVertex3d(px,py,nz);
		glVertex3d(0.5*X,py,nz);
		glVertex3d(0.5*X,ny,nz);
		glVertex3d(px,ny,nz);

//		glNormal3d(0.,0.,-1.);
		glVertex3d(-0.5*X,py,nz);
		glVertex3d(nx,py,nz);
		glVertex3d(nx,ny,nz);
		glVertex3d(-0.5*X,ny,nz);

		glNormal3d(0.,1.,0.);
		glVertex3d(nx,0.5*Y,pz);
		glVertex3d(px,0.5*Y,pz);
		glVertex3d(px,0.5*Y,nz);
		glVertex3d(nx,0.5*Y,nz);

		glNormal3d(0.,0.,1.);
		glVertex3d(nx,-0.5*Y,pz);
		glVertex3d(px,-0.5*Y,pz);
		glVertex3d(px,0.5*Y,pz);
		glVertex3d(nx,0.5*Y,pz);

//		glNormal3d(0.,0.,1.);
		glVertex3d(px,ny,pz);
		glVertex3d(0.5*X,ny,pz);
		glVertex3d(0.5*X,py,pz);
		glVertex3d(px,py,pz);

//		glNormal3d(0.,0.,1.);
		glVertex3d(-0.5*X,ny,pz);
		glVertex3d(nx,ny,pz);
		glVertex3d(nx,py,pz);
		glVertex3d(-0.5*X,py,pz);

		glNormal3d(0.,-1.,0.);
		glVertex3d(nx,-0.5*Y,nz);
		glVertex3d(px,-0.5*Y,nz);
		glVertex3d(px,-0.5*Y,pz);
		glVertex3d(nx,-0.5*Y,pz);

	glEnd();
	for(int ri=0; ri<2; ri++)
	{
		glPushMatrix();
			glTranslated(px,py,0);
			GLdouble equation1[4] = {1,0,0,0};
			glClipPlane(GL_CLIP_PLANE0,equation1);
			glEnable(GL_CLIP_PLANE0);
			GLdouble equation2[4] = {0,1,0,0};
			glClipPlane(GL_CLIP_PLANE1,equation2);
			glEnable(GL_CLIP_PLANE1);
			gluCylinder(QObj,R,R,H,12,1);
			glDisable(GL_CLIP_PLANE0);
			glDisable(GL_CLIP_PLANE1);
			gluPartialDisk(QObj,0,R,step,step,0,90);
		glPopMatrix();
		glPushMatrix();
			glTranslated(px,py,nz);
			gluPartialDisk(QObj,0,R,step,step,0,90);
		glPopMatrix();
		glPushMatrix();
			glTranslated(px,py,pz);
			gluPartialDisk(QObj,0,R,step,step,0,90);
		glPopMatrix();
		glPushMatrix();
			glTranslated(px,-py,0);
			GLdouble equation3[4] = {1,0,0,0};
			glClipPlane(GL_CLIP_PLANE0,equation3);
			glEnable(GL_CLIP_PLANE0);
			GLdouble equation4[4] = {0,-1,0,0};
			glClipPlane(GL_CLIP_PLANE1,equation4);
			glEnable(GL_CLIP_PLANE1);
			gluCylinder(QObj,R,R,H,12,1);
			glDisable(GL_CLIP_PLANE0);
			glDisable(GL_CLIP_PLANE1);
			gluPartialDisk(QObj,0,R,step,step,0,90);
		glPopMatrix();
		glPushMatrix();
			glTranslated(px,-py,nz);
			gluPartialDisk(QObj,0,R,step,step,180,-90);
		glPopMatrix();
		glPushMatrix();
			glTranslated(px,-py,pz);
			gluPartialDisk(QObj,0,R,step,step,180,-90);
		glPopMatrix();
		glRotated(180,0,0,1);
	}
	glPopMatrix();
}

NToolPRect::NToolPRect(double X, double Y, double H, double R, double Angle):NToolPunch()
{
	QObj = gluNewQuadric();
	this->X = X;
	this->Y = Y;
	this->H = H;
	this->R = R;
	this->Angle = Angle;
}
