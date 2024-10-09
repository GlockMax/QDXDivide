#ifdef NCMEXT_EXPORTS
#define NCMEXT_API __declspec(dllexport)
#else
#define NCMEXT_API __declspec(dllimport)
#endif

#include "NExtEnums.h"

enum PosAngle
{
	PA_0,
	PA_90,
	PA_180,
	PA_270
};

enum NCMECode
{ // Return codes for NCMEXT_API functions
	NCME_OK,
	NCME_NOT_INSTALLED, // NCManager is not installed properly
	NCME_CANT_WRITE, // Unable to write temporary file
	NCME_BAD_STRING, // Input string is not valid
	NCME_BAD_WINDOW, // Input HWND is not valid
	NCME_PROG_NOT_FOUND, // Can't find the program to remove or replace
	NCME_TOOL_NOT_FOUND, // Can't find the tool to remove
	NCME_BAD_RETURN_CODE, // NCManager can't perform requested operation
	NCME_BAD_ARGUMENT // Wrong argument passed
};

// To start NCManager in the external process window (hosting mode)
// MaxWaitTime is measured in milliseconds
NCMEXT_API NCMECode NCMStartHost(HWND ParentWnd, unsigned int MaxWaitTime = 15000);
NCMEXT_API NCMECode NCMStopHost();

// To load the project to NCManager in hosting mode
NCMEXT_API NCMECode NCMSendHost(void);

// To change size of the NCManager window in hosting mode
NCMEXT_API NCMECode NCMSizeHost(const CRect& Rect);

//To load the project to NCManager
NCMEXT_API NCMECode NCMSend(void);

//To clear the project 
NCMEXT_API NCMECode NCMClear(void);

//To define the prototype file (full path)
NCMEXT_API NCMECode NCMSetPrtFileName(const char *FileName);

//To define the milling project
NCMEXT_API NCMECode NCMSetMillProject(void);

//To define the mill-turn project
NCMEXT_API NCMECode NCMSetMillTurnProject(void);

//To define the turning project
NCMEXT_API NCMECode NCMSetTurnProject(void);

//To define the welding project
NCMEXT_API NCMECode NCMSetWeldProject(void);


//To define the cnc-control file
//param Name is name and ext. of cnc-control file
NCMEXT_API NCMECode NCMSetNCControl(const char *Name);

//To define the machine file
//param Name is folder name of machine file
NCMEXT_API NCMECode NCMSetNCMachine(const char *Name);



// NC programs

//To add the cnc program
//param FileName is path to a file containing cnc (G-code file) data
NCMEXT_API NCMECode NCMAddProgram(const char *FileName);

//To add the cnc program
//param FileName is path to a file containing cnc (G-code file) data
NCMEXT_API NCMECode NCMAddSubProgram(const char *FileName);

//To delete the cnc program
//param FileName is path to a file containing cnc (G-code file) data
NCMEXT_API NCMECode NCMRemoveProgram(const char *FileName);

//To replace the cnc program
//param OldFileName is path to a file to be replaced
//param NewFileName is path to a file replacing old file
NCMEXT_API NCMECode NCMReplaceProgram(const char *OldFileName, const char *NewFileName);

//To remove all cnc programs
NCMEXT_API NCMECode NCMRemoveAllPrograms(void);



// Tools

//To define cutting part of the mill tool contour in STC format
/*
param Contour defines the 2D contour of the tool. 
The contour will be rotated around the z-axis.
Contour is a list of simple G-Codes like without blanks.
The symbol "\n" is used for transfer of a line.
Record example:
G0X0Y0G1X10Y10G1X10Y60G1X40Y60G3X45Y65I0J5G1X45Y100G1X0Y100
X defines the x-coordinate and Y the z-coordinate.
I(relative to X),J(relative to z) define the center of the arc relative 
to the current position.
G0, G1 define linear pieces, G0 is used in first line.
G2, G3 define arc pieces.
Contour should be down-up oriented.
*/
//param Position is tool position in machine
//param Turret is turret number in machine
//param Axis is axis number in machine
NCMEXT_API NCMECode NCMAddMillCutCont(const char *Contour, int Position, int Turret = 0, int Axis = 0);

//To define non cutting (holder) part of the mill tool contour in STC format
//param Contour defines the 2D contour of the holder. 
//param Position is tool position in machine
//param Turret is turret number in machine
//param Axis is axis number in machine
NCMEXT_API NCMECode NCMAddMillHolderCont(const char *Contour, int Position, int Turret = 0, int Axis = 0);

//To define lathe insert contour in STC format
//param Contour defines the 2D contour of the insert. 
//The contour will be extruded along the y-axis.
//param Position is tool position in machine
//param Turret is turret number in machine
//param Axis is axis number in machine
//param Thickness is a thickness lathe insert
//param Angle is back angle lathe insert 
NCMEXT_API NCMECode NCMAddTurnInsertCont(const char *Contour, int Position, int Turret = 0, int Axis = 0, double Thickness = 1., double Angle = 0.);

//To define cutting part of the drill tool contour in STC format for turning
/*
param Contour defines the 2D contour of the tool. 
The contour will be rotated around the z-axis.
*/
//param Position is tool position in machine
//param Turret is turret number in machine
//param Axis is axis number in machine
NCMEXT_API NCMECode NCMAddTurnDrillCutCont(const char *Contour, int Position, int Turret = 0, int Axis = 0);

//To define non cutting (holder) part of the drill tool contour in STC format for turning
/*
param Contour defines the 2D contour of the tool. 
The contour will be rotated around the y-axis.
*/
//param Position is tool position in machine
//param Turret is turret number in machine
//param Axis is axis number in machine
NCMEXT_API NCMECode NCMAddTurnDrillHolderCont(const char *Contour, int Position, int Turret = 0, int Axis = 0);

//To set the tool colors
//param CutColor is the color for cutting part of a tool
//param NonCutColor is the color for non cutting part of a tool
//param Position is tool position in machine
//param Turret is turret number in machine
NCMEXT_API NCMECode NCMSetToolColors( COLORREF CutColor, COLORREF NonCutColor, int Position, int Turret = 0);

//To delete the tool
NCMEXT_API NCMECode NCMRemoveTool(int Position, int Turret = 0);

//To remove all tools
NCMEXT_API NCMECode NCMRemoveAllTools(void);


// NC control start state

//To define start tool number
//param Position is tool number
//param Turret is turret number
NCMEXT_API NCMECode NCMStartToolNumber(int Position, int Turret = 0);

//To define start tool position
// param X, Y, Z, A, B, C are coordinates of the start tool position 
NCMEXT_API NCMECode NCMStartToolPosition(double dX=0., double dY=0., double dZ=0., double dA=0., double dB=0., double dC=0.);

//To define H corrector
//param Number is number of H corrector
//param Value is value of H corrector
NCMEXT_API NCMECode NCMAddHCompensation(int Number, double Value);

//To define D corrector
//param Number is number of D corrector
//param Value is value of D corrector
NCMEXT_API NCMECode NCMAddDCompensation(int Number, double Value);

//To define G54-G59 coordinate systems
//param Number is number of coordinate system
//0 - G54, 1 - G55, 2 - G56, 3 - G57, 4 - G58, 5 - G59
// param X, Y, Z, A, B, C are offsets value  
NCMEXT_API NCMECode NCMAddFixOffset(int iNumber=0,double dX=0.,double dY=0.,double dZ=0.,double dA=0.,double dB=0.,double dC=0.);

// Stock

//To delete the stock
NCMEXT_API NCMECode NCMRemoveStock(void);

//To define the stock for milling as a box
//param X0, Y0, Z0 are coordinates of the first corner of a box   
//param X1, Y1, Z1 are coordinates of the second corner of a box   
NCMEXT_API NCMECode NCMSetMillStockBox(double X0, double Y0, double Z0, double X1, double Y1, double Z1);

//To define the stock for milling as a cylinder
//param X0, Y0, Z0 are coordinates of the center point of the cylinder base
//param H is cylinder heght
//param D is cylinder diameter
//param Axis defines coordinate axis to be used as a cylinder axis
// Axis = 0 - X axis
// Axis = 1 - Y axis
// Axis = 2 - Z axis
NCMEXT_API NCMECode NCMSetMillStockCyl(double X0, double Y0, double Z0, double H, double D, int Axis);

//To define the stock for milling from stl.
//param	FileName is path to a file containing stl(stereo-lithography file) data.
NCMEXT_API NCMECode NCMSetMillStockStl(const char *FileName);

//To define the stock for milling by a contour extruding
//param FileName is path to a file containing contour (StC-code file) data (contour should lie in the XY plane)
//param Zmin is z value for start position of extruded contour
//param Zmax is z value for end position of extruded contour ( Zmax should be grater than Zmin)
//param RotX is angle around X-axis
//param RotY is angle around Y-axis
//param RotZ is angle around Z-axis
//Value for RotX, RotY, RotZ:
//	PA_0 = 0 degrees,
//	PA_90 = 90 degrees,
//	PA_180 = 180 degrees,
//	PA_270 = 270 degrees
NCMEXT_API NCMECode NCMSetMillStockExtrude(const char *FileName, double Zmin, double Zmax, PosAngle RotX = PA_0, PosAngle RotY = PA_0, PosAngle RotZ = PA_0);
//param Contour param Contour defines the 2D contour of the stock.  (contour should lie in the XY plane)
NCMEXT_API NCMECode NCMSetMillStockExtrudeStC(const char *Contour, double Zmin, double Zmax, PosAngle RotX = PA_0, PosAngle RotY = PA_0, PosAngle RotZ = PA_0);

//To define the stock for turning (turn and millturn project) by a contour of rotating
//param FileName is path to a file containing contour (StC-code file) data
//Y axis of the contour corresponds to the Z axis of the stock
NCMEXT_API NCMECode NCMSetTurnStockContour(const char *FileName);
//param Contour defines the 2D contour of the stock.  (contour should lie in the XY plane)
//Y axis of the contour corresponds to the Z axis of the stock
NCMEXT_API NCMECode NCMSetTurnStockContourStC(const char *Contour);

//To subscribe to program change messages
//After subscription NCManager will send WM_COPYDATA message when a program has been changed
//using parameter Pass value as the COPYDATASTRUCT.dwData value
//COPYDATASTRUCT.lpData contains changed program name (as char *)
//COPYDATASTRUCT.cbData contains changed program name length, without terminating '\0' character
//param MainWnd is the application main window handle (NCManager sends WM_COPYDATA message to this window)
NCMEXT_API NCMECode NCMSubscribeToProgramChanges(HWND MainWnd, ULONG_PTR Pass = 0);
//To unsubscribe from program change messages
NCMEXT_API NCMECode NCMUnsubscribeFromProgramChanges();
//To save all programs texts
NCMEXT_API NCMECode NCMSaveAllPrograms();

//To blank all programs
NCMEXT_API NCMECode NCMBlankAllPrograms();
//To unblank all programs
NCMEXT_API NCMECode NCMUnblankAllPrograms();
//To show text of the specified program
NCMEXT_API NCMECode NCMShowProgramText(const char *FileName);

//To reset the project
NCMEXT_API NCMECode NCMReset();
//To execute simulation up to a breakpoint
NCMEXT_API NCMECode NCMStartExecution();
//To stop simulation
NCMEXT_API NCMECode NCMStopExecution();
//To set the cutting mode
NCMEXT_API NCMECode NCMSetModeCut();
//To set the dont cut mode
NCMEXT_API NCMECode NCMSetModeDontCut();
//To set the fast cutting mode
NCMEXT_API NCMECode NCMSetModeFastCut();
// To set smooth tool motion
NCMEXT_API NCMECode NCMSetTMSmoothly();
//To set smooth tool motion with feedrate from CNC-program
NCMEXT_API NCMECode NCMSetTMFeedrate();
//To set getting the simulation result without tool motion
NCMEXT_API NCMECode NCMSetTMResult();


//To subscribe to collision messages
//After subscription NCManager will send WM_COPYDATA message when a collision is found
//using parameter Pass value as the COPYDATASTRUCT.dwData value
//COPYDATASTRUCT.lpData contains pointer to CollisionData structure(as CollisionData *)
//COPYDATASTRUCT.cbData contains CollisionData structure length
//param MainWnd is the application main window handle (NCManager sends WM_COPYDATA message to this window)
NCMEXT_API NCMECode NCMSubscribeToColl(HWND MainWnd, ULONG_PTR Pass = 0);

//To set tools collision "Check" property
// Val = 0 - No
// Val = 1 - Tool
// Val = 2 - Tool and holder
NCMEXT_API NCMECode NCMSetToolsCollCheck(int Val);

//To set tools collision "Ignore Up Tool" property
NCMEXT_API NCMECode NCMSetToolsCollIgnoreUp(BOOL Val);

//To set tools collision "Tolerance for diameter" property
NCMEXT_API NCMECode NCMSetToolsCollMarginD(double Val);

//To set tools collision "Tolerance for length" property
NCMEXT_API NCMECode NCMSetToolsCollMarginL(double Val);



//To set stock "Quality" property
NCMEXT_API NCMECode NCMSetStockQuality(int Val);
//To get maximum allowable stock quality property value
// This value is a hint only and does not imply any warranties
NCMEXT_API NCMECode NCMGetMaxStockQuality(int &Val);
//To set stock "Colour" property
NCMEXT_API NCMECode NCMSetStockColour(COLORREF Val);
//To set stock "Visibility" property
NCMEXT_API NCMECode NCMSetStockVisibility(BOOL Val);
//To set stock "Edges" property
NCMEXT_API NCMECode NCMSetStockEdges(BOOL Val);
//To set stock edges "Thickness" property
NCMEXT_API NCMECode NCMSetStockEdgesThickness(double Val);
//To set programs "To show" property (see TrajPart definition)
NCMEXT_API NCMECode NCMSetProgramsShow(enum TrajPart Val);
//To set program "Thickness" property
NCMEXT_API NCMECode NCMSetProgramThickness(const char *FileName, double Val);
//To set program "Visibility" property
NCMEXT_API NCMECode NCMSetProgramVisibility(const char *FileName, BOOL Val);
//To set tools "Visibility" property
NCMEXT_API NCMECode NCMSetToolsVisibility(BOOL Val);
//To set tool "Toolpath" property. The tool is identified by Position and Turret values
NCMEXT_API NCMECode NCMSetToolToolpath(int Position, int Turret, BOOL Val);
//Zoom all-
NCMEXT_API NCMECode NCMViewZoomAll();
//Rotate around coordinate axis-
// Axis = 0 - X axis
// Axis = 1 - Y axis
// Axis = 2 - Z axis
NCMEXT_API NCMECode NCMViewRotateA(int Axis);
//Scale-
NCMEXT_API NCMECode NCMViewScale();
//Zoom window-
NCMEXT_API NCMECode NCMViewZoomW();
//Pan-
NCMEXT_API NCMECode NCMViewPan();
//Set view-
//eyex, eyey, eyez - The position of the eye point.
//upx, upy, upz - The direction of the up vector.
//See gluLookAt documentation for details
NCMEXT_API NCMECode NCMViewSetView(double eyex, double eyey, double eyez, double upx, double upy, double upz);
//To remove all cnc subprograms-
NCMEXT_API NCMECode NCMRemoveAllSubprograms(void);
//To remove the cnc subprogram-
//param FileName is a path to a file containing cnc (G-code file) data
NCMEXT_API NCMECode NCMRemoveSubprogram(const char *FileName);
//To save specified program or subprogram text to the specified file. If NewFileName = nullptr
// then text should be saved in FileName file-
NCMEXT_API NCMECode NCMSaveProgram(const char *FileName, const char *NewFileName = nullptr);

// Activate/deactivate breakpoints
// Program Start/Stop
NCMEXT_API NCMECode NCM_BPProgramStop();
// Optional/Program stop
NCMEXT_API NCMECode NCM_BPOptionalStop();
// Tool change
NCMEXT_API NCMECode NCM_BPToolChange();
// Bookmark
NCMEXT_API NCMECode NCM_BPBookmark();

//Show main menu
NCMEXT_API NCMECode NCM_ShowMainMenu(BOOL Show);
//Set fixed rotation center
NCMEXT_API NCMECode NCM_SetFixedCenter(BOOL Set);
