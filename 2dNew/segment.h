#pragma once
#include "base_shape.h"
#include "node.h"
class segment :public IBaseShape
{
	node* _nodeStart;
	node* _nodeEnd;
public:
	segment(POINTS ptsStart, POINTS ptsEnd);
	void draw(HDC hdc) override;
	~segment();
};

