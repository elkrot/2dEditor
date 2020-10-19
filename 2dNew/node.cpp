#include "node.h"
node::node()
{
}
node::node(POINTS ptsStart)
{
	_ptsStart = ptsStart;
}

void node::draw(HDC hdc) 
{	
	MoveToEx(hdc, _ptsStart.x, _ptsStart.y,
		(LPPOINT)NULL);
}

int node::x()
{
	return _ptsStart.x;
}

int node::y()
{
	return _ptsStart.y;
}
