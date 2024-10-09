#pragma once

class NMeshArray;

class AFX_EXT_CLASS NMakeStdMesh
{
public:
	NMakeStdMesh(void);
	~NMakeStdMesh(void);
	static bool Cylinder(MeshArr & MeshArray, double  D, double H, double Tolerance = 0.1);
	static bool BoxBounds(MeshArr & MeshArray, const BPoint &Min, const BPoint &Max);
	static bool BoxBounds(MeshArr & MeshArray, const BPoint *Cont, int ContSize, double Height);
};
