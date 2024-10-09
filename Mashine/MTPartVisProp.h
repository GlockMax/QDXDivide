#pragma once

class MTPartVisProp
{
public:
	MTPartVisProp(void){};
	~MTPartVisProp(void){};

	bool Visible;
	bool WFrame;
	bool SharpEdges;
	bool SmoothSurf;
	enum RenderMode RMode;
	COLORREF Color;
};
