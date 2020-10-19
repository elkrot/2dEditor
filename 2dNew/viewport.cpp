#include "viewport.h"
/*
 ������ ������ ����
*/
void Viewport::SetWindowSize(int _Width, int _Height) {
	Width = _Width;
	Height = _Height;
}
/*
 ��������� �������������� �� ��������� [-1, 1] x [-1, 1]
 � ���������� ������, � ������ ��������
*/
_Point Viewport::T(_Point point) {
	_Point TPoint;
	TPoint.x = Margin + (1.0 / 2) * (point.x + 1) * (Width - 2 * Margin);
	TPoint.y = Margin + (-1.0 / 2) * (point.y - 1) * (Height - 2 * Margin);
	return TPoint;
}
/*
 ��������� �������������� �� ��������� ������, � ����������
 [-1, 1] x [-1, 1] � ������ ��������
*/
_Point Viewport::T_inv(_Point point) {
	_Point TPoint;
	TPoint.x = double(point.x - Margin) / (1.0 / 2) / (Width - 2 * Margin) - 1;
	TPoint.y = double(point.y - Margin) / (-1.0 / 2) / (Height - 2 * Margin) + 1;
	return TPoint;
}
/*
 �����������, ���������� ������� �� ���������
*/
Viewport::Viewport() {
	SetMargin();
}
/*
 ������������� ������ �� ����� ������
*/
void Viewport::SetMargin(int _Margin) {
	Margin = _Margin;
}