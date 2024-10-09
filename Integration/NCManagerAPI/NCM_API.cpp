#include "StdAfx.h"
#include "NCM_Integration.h"
#include "NCM_API.h"

NCM_Integration Integration;


NCM_API::NCM_API(void)
{
}


NCM_API::~NCM_API(void)
{
}

//!Axis aligned box.
/*!
\param	min are the minimum coordinates of the box.
\param	max are the maximum coordinates of the box.
\param resolution is the resolution of the discrete model associated with the stock.
\param rgb defines the color. Default is set to (255,255,255).
\return true if successful. 
\sa getGridSize().
*/
bool NCM_API::setStock(const double min[3],const double max[3],double resolution,const UCHAR rgb[3])
{
	return Integration.setStock(min, max, resolution, rgb);
}

//!Axis aligned cylinder.
/*!
\param	center is the center of the cylinder.
\param	length is the length of the cylinder.
\param	radius is the radius of the cylinder.
\param	rotationAxis is the rotationAxis of the cylinder.
\param resolution is the resolution of the discrete model associated with the stock.
\param rgb defines the color. Default is set to (255,255,255).
\return true if successful. 
\sa getGridSize().
*/
bool  NCM_API::setStock(const double center[3],double length,double radius,AXIS rotationAxis,double resolution,const UCHAR rgb[3]) {return false;}


//!Triangle Mesh.
/*!
\param	numberOfTriangles is the number of triangles.
\param	vertexData is a pointer to an array of numberOfTriangles * 3 * 3 floats.
\param resolution is the resolution of the discrete model associated with the stock.
\param rgb defines the color. Default is set to (255,255,255).
\return true if successful. 
\sa getGridSize().
*/
bool  NCM_API::setStock(const size_t numberOfTriangles,const float* vertexData,double resolution ,const UCHAR rgb[3]) {return false;}

//@}

//!Stl file.
/*!
\param	stlFileName is path to an file containing stl(stereo-lithography file) data.
\param resolution is the resolution of the discrete model associated with the stock.
\param rgb defines the color. Default is set to (255,255,255).
\return true if successful. 
\sa getGridSize().
*/
bool  NCM_API::setStock(const char* stlFileName,double resolution,const UCHAR rgb[3])
{
	return false;
}

//@}

/** @name Stock related functions
*/
//@{

//!Returns the color of  the stock.
void  NCM_API::getColor(UCHAR color[3]){return ;}

//!Returns the bounding box of the stock
void  NCM_API::getBoundingBox(double min[3],double max[3]){return ;}

//!Resets the stock to the initial state.
void  NCM_API::resetStock()
{
	Integration.resetStock();
	return ;
}

/// render the stock gemoetry
bool  NCM_API::renderStock()
{
	return Integration.renderStock();
}

//! Defines a custom tool
/*!
\anchor newCustomMillingTool
\param rgb defines the color. Default is set to (255,255,255).
\param contour defines the 2D contour of the tool. The contour will be rotated around the z-axis.
contour is a list of simple G-Codes like:

\verbatim
contour = "
G1 Z0 R2
G1 Z2 R4
G2 Z6 R4 I2 J0
G1 Z8 R4
G0 Z15 R4"

Z defines the z-coordinate and R the radius at z.
I(relative to Z),J(relative to R) define the center of the arc relative to the current position.
G0, G1 define linear pieces, G0 is used for non cutting parts of the tool.
G2, G3 define arc pieces.
\endverbatim

*/
int NCM_API::newCustomMillingTool(const char* contour,const UCHAR rgb[3])
{
	return Integration.newCustomMillingTool(contour, rgb);
}

//!Set the tool color.
/*!
\param toolId must be a valid tool id returned by a newTool() function.
\param rgb is the new color. Default is set to (255,255,255).
*/
bool  NCM_API::setToolColor(TOOLID toolId, const UCHAR rgb[3])
{
	return Integration.setToolColor(toolId, rgb);
}
//!Set the tool reference point.
/*!
\param toolId must be a valid tool id returned by a newTool() function.
\param point is the new  reference point. Default is set to (0,0,0).
*/
bool  NCM_API::setToolReferencePoint(TOOLID toolId, const Point& point){return false;}

//!Write the stock as triangle list in Stl format to the file 'fileName'.
/*!
\return true if successful.
*/
bool  NCM_API::writeStl(const char* fileName,bool binary){return false;}


//!Subtract the swept volume of the tool from the stock.
/*!
\param toolId must be a valid id returned by a newTool() function.
\param toolStep describes one single step of the tool.
\param checkCollison should be set to true if the nonCutting part of the tool should be tested for collision with the workpiece.
\return true if successful.
*/
bool NCM_API::removeToolTrace(TOOLID toolId,const ToolStep& toolStep,bool checkCollison)
{
	return Integration.removeToolTrace(toolId
		, toolStep.from.position.v
		, toolStep.from.direction.v
		, toolStep.to.position.v
		, toolStep.to.direction.v
		, toolStep.center.v
		, toolStep.arcInterPolation
		, toolStep.counterClockWise
		, checkCollison);
}
bool NCM_API::removeToolTrace(int ToolId
		, const double Bp[3]
		, const double Bn[3]
		, const double Ep[3]
		, const double En[3]
		, const double Ca[3]
		, bool Arc
		, bool CCW
		, bool checkCollison)
{
	return Integration.removeToolTrace(ToolId, Bp, Bn, Ep, En, Ca, Arc, CCW, checkCollison);
}
//!Render the tool with id 'toolId' at position 'toolPosition' 
/*!
\param toolId must be a valid tool id returned by a newTool() function.
\param toolPosition the position and orientation of the tool in world coordinates. 
*/
bool  NCM_API::renderTool(TOOLID toolId, const ToolPosition& toolPosition) {return false;}


