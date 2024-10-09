#pragma once

#ifdef NCMAPI_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif


class DLL_API NCM_API
{
public:


	//@{
	//! uchar
	typedef unsigned char UCHAR;
	//! toolId
	typedef int  TOOLID;
	//! real
	typedef double real;
	//! axis
 	enum AXIS {
		X = 0,	/*!< (1,0,0). */  
		Y,		/*!< (0,1,0). */  
		Z		/*!< (0,0,1). */  
	};
	//@}
	//! Vector
 	struct  Vector{

		real v[3];/*!< array of 3 reals */  
		Vector(real x = 0, real y = 0, real z = 0){v[0] = x; v[1] = y; v[2] = z;}
		Vector(const Vector& a) {memcpy(v,a.v,3 * sizeof(real));}
	};
	//! Point
 	typedef Vector  Point;
	//! ToolPosition
	struct ToolPosition {

		Point  position; /*!< the position of the tool. */  
		Vector direction;/*!< the orientation of the tool. */  

		ToolPosition(){};
		ToolPosition(const Point& _position,const Vector& _direction)
		:position(_position),direction(_direction){};
		ToolPosition(const ToolPosition& toolPosition)
		:position(toolPosition.position),direction(toolPosition.direction){};

	};//------------------------------------------------------------------------
	//! ToolStep
	struct ToolStep {

		ToolPosition from;/*!< the start position of the tool. */ 
		ToolPosition to;  /*!< the end position of the tool. */ 

		bool arcInterPolation;/*!< are we doing linear or arc interpolation . */ 
		Point center;         /*!< the center of the arc. */ 
		Vector planeNormal;   /*!< the plane normal of the arc. */ 
		bool	counterClockWise; /*!< the orientation of the arc. */ 
		
		//! default constructor
		ToolStep():arcInterPolation(false){};
		//!constructor for linear interpolation
		ToolStep(const ToolPosition& _from, const ToolPosition& _to)
		:from(_from),to(_to),arcInterPolation(false){};
		//!constructor for arc interpolation
		ToolStep(const ToolPosition& _from, const ToolPosition& _to,const Point& _center,const Vector& _planeNormal, bool _counterClockWise = true)
		:from(_from),to(_to),arcInterPolation(true),center(_center),planeNormal(_planeNormal),counterClockWise(_counterClockWise){};
	};//------------------------------------------------------------------------



	NCM_API(void);
	virtual ~NCM_API(void);
public:
	//!Axis aligned box.
	/*!
	\param	min are the minimum coordinates of the box.
	\param	max are the maximum coordinates of the box.
	\param resolution is the resolution of the discrete model associated with the stock.
	\param rgb defines the color. Default is set to (255,255,255).
	\return true if successful. 
	\sa getGridSize().
	*/
	virtual bool setStock(const double min[3],const double max[3],double resolution = 0.8,const UCHAR rgb[3] =0);

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
	virtual bool setStock(const double center[3],double length,double radius,AXIS rotationAxis,double resolution = 0.8,const UCHAR rgb[3] = 0);

	//!Triangle Mesh.
	/*!
	\param	numberOfTriangles is the number of triangles.
	\param	vertexData is a pointer to an array of numberOfTriangles * 3 * 3 floats.
	\param resolution is the resolution of the discrete model associated with the stock.
	\param rgb defines the color. Default is set to (255,255,255).
	\return true if successful. 
	\sa getGridSize().
	*/
	virtual bool setStock(const size_t numberOfTriangles,const float* vertexData,double resolution = 0.8,const UCHAR rgb[3] =0);
	//@}

	//!Stl file.
	/*!
	\param	stlFileName is path to an file containing stl(stereo-lithography file) data.
	\param resolution is the resolution of the discrete model associated with the stock.
	\param rgb defines the color. Default is set to (255,255,255).
	\return true if successful. 
	\sa getGridSize().
	*/
	virtual bool setStock(const char * stlFileName,double resolution = 0.8,const UCHAR rgb[3] =0);
	//@}

	/** @name Stock related functions
	*/
	//@{

	//!Returns the color of  the stock.
	virtual void getColor(UCHAR color[3]);

	//!Returns the bounding box of the stock
	virtual void getBoundingBox(double min[3],double max[3]);

	//!Resets the stock to the initial state.
	virtual void resetStock();

	/// render the stock gemoetry
	virtual bool renderStock() ;

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
	virtual TOOLID  newCustomMillingTool(const char* contour,const UCHAR rgb[3]= 0);

	//!Set the tool color.
	/*!
	\param toolId must be a valid tool id returned by a newTool() function.
	\param rgb is the new color. Default is set to (255,255,255).
	*/
	virtual bool setToolColor(TOOLID toolId, const UCHAR rgb[3]);
	//!Set the tool reference point.
	/*!
	\param toolId must be a valid tool id returned by a newTool() function.
	\param point is the new  reference point. Default is set to (0,0,0).
	*/
	virtual bool setToolReferencePoint(TOOLID toolId, const Point& point);

	//!Write the stock as triangle list in Stl format to the file 'fileName'.
	/*!
	\return true if successful.
	*/
	virtual bool writeStl(const char* fileName,bool binary = true);


	//!Subtract the swept volume of the tool from the stock.
	/*!
	\param toolId must be a valid id returned by a newTool() function.
	\param toolStep describes one single step of the tool.
	\param checkCollison should be set to true if the nonCutting part of the tool should be tested for collision with the workpiece.
	\return true if successful.
	*/
	virtual bool  removeToolTrace(TOOLID toolId,const ToolStep& toolStep,bool checkCollison);
	virtual bool removeToolTrace(int ToolID
		, const double Bp[3]
		, const double Bn[3]
		, const double Ep[3]
		, const double En[3]
		, const double Ca[3]
		, bool Arc
		, bool CCW
		, bool checkCollison);
	//!Render the tool with id 'toolId' at position 'toolPosition' 
	/*!
	\param toolId must be a valid tool id returned by a newTool() function.
	\param toolPosition the position and orientation of the tool in world coordinates. 
	*/
	virtual bool renderTool(TOOLID toolId, const ToolPosition& toolPosition) ;
};

