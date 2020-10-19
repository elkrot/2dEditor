#include "geometry.h"
#ifndef _VIEWPORT
class Viewport {
	int Margin;
	int Height, Width;
public:
	Viewport();
	void SetWindowSize(int _Width, int _Height);
	_Point T(_Point point);
	_Point T_inv(_Point point);
	void SetMargin(int _Margin = 10);
};
#define _VIEWPORT
#endif
