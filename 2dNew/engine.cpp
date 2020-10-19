#include <windows.h>
#include "geometry.h"
#include "matrix.h"
#include "engine.h"
#include "segment.h"

/*
 Привязываем к движку объект, который отвечает за действия пользователя
*/
void Engine::SetAction(Action* _action)
{
	action = _action;
}
void Engine::AddShape()
{
	shapes.push_back(new segment(ptsBegin, ptsEnd));
}


void Engine::MouseMove(HDC hdc, POINTS  ptEnd ) {

	SetROP2(hdc, R2_NOTXORPEN);
	if (fPrevLine)
	{
		MoveToEx(hdc, ptsBegin.x, ptsBegin.y,
			(LPPOINT)NULL);
		LineTo(hdc, ptsPrevEnd.x, ptsPrevEnd.y);
	}	
	MoveToEx(hdc, ptsBegin.x, ptsBegin.y, (LPPOINT)NULL);
	LineTo(hdc, ptEnd.x, ptEnd.y); 
	ptsPrevEnd = ptEnd;
	ptsEnd = ptEnd;
	fPrevLine = TRUE;
	
}
void Engine::SetPtsBegin(POINTS ptBegin)
{
	ptsBegin = ptBegin;
	
}
void Engine::SetPrevLine(BOOL PrevLine)
{
	fPrevLine = PrevLine;
}
/*
 Выводит графику на контекст hdc
*/
void Engine::Draw(HDC hdc) {
    RECT rect;
   // XFORM xForm;
    //SetViewportOrgEx(hdc, 100, 100, NULL);
    //SetGraphicsMode(hdc, GM_ADVANCED);
    //SetMapMode(hdc, MM_LOENGLISH);

    //// Найдем середину рабочей области. 
    //SetPixel(hdc, 0, 0, RGB(0xff, 0, 0));

    //DPtoLP(hdc, (LPPOINT)&rect, 2);

    // Выберем пустую кисть. 
	int iVertRes, iHorzRes;
	iVertRes = GetDeviceCaps(hdc, VERTRES);
	iHorzRes = GetDeviceCaps(hdc, HORZRES);


	wchar_t buffer[256];
	wsprintfW(buffer, L"%d x %d", iVertRes, iHorzRes);
	TextOutW(hdc, 200,200, buffer, wcslen(buffer));
    SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
	for (int i = 0; i < shapes.size(); i++) {
		shapes[i]->draw(hdc);
	}
    
	
}