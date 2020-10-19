#pragma once
#include "base_shape.h"
class node :public IBaseShape
{
	POINTS _ptsStart;

public:
	node();
	node(POINTS ptsStart);
	void draw(HDC hdc) override;
	int x();
	int y();
};

