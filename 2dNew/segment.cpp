#include "segment.h"

segment::segment(POINTS ptsStart, POINTS ptsEnd) 
{
	_nodeStart = new node(ptsStart);
	_nodeEnd = new node(ptsEnd);
}
void segment::draw(HDC hdc) 
{
	_nodeStart->draw(hdc);
	_nodeEnd->draw(hdc);
	MoveToEx(hdc, _nodeStart->x(), _nodeStart->y(),
		(LPPOINT)NULL);
	LineTo(hdc, _nodeEnd->x(), _nodeEnd->y());
}

segment::~segment() {
	delete _nodeStart;
	delete _nodeEnd;
}