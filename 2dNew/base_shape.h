#pragma once
#include <windows.h>
 class IBaseShape 
{

public:
	virtual void draw(HDC hdc) {};
	virtual ~IBaseShape() {};
};

 class Chain :public IBaseShape {

	 
	 POINTS _ptsStart, _ptsEnd;
 public:
	 Chain(POINTS ptsStart, POINTS ptsEnd)
	 {
		 _ptsStart = ptsStart;
		 _ptsEnd = ptsEnd;
	 }
	 void draw(HDC hdc) override
	 {

		// RECT rect;
		// XFORM xForm;
		 MoveToEx(hdc, _ptsStart.x, _ptsStart.y,
			 (LPPOINT)NULL);
		 LineTo(hdc, _ptsEnd.x, _ptsEnd.y);
		 
	 //IBaseShape:draw();
	 }
 };