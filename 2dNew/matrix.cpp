#include <math.h>
#include <memory.h>
#include "matrix.h"
#include "geometry.h"
/*
 �����������, �������������� ������� ��������� (������� �������������� ��������������)
*/
Matrix::Matrix() {
	SetUnit();
}
/*
 ������� ������� ���������� ���������
*/
void Matrix::SetUnit() {
	//memset(data, sizeof(data), 0);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			data[i][j] = 0.0;
		}
	}
	for (int i = 0; i < 3; i++) {
		data[i][i] = 1.0;
	}
}
/*
 ������������� ������� ������� � �������� ������� �������� �� ���� alpha,
 �������� ��������� � �������
*/
void Matrix::SetRotationMatrixbySinCos(double sinalpha, double cosalpha) {
	SetUnit();
	data[0][0] = cosalpha;
	data[0][1] = sinalpha;
	data[1][0] = -sinalpha;
	data[1][1] = cosalpha;
}
/*
 ������������� ������� ������� � �������� ������� �������� �� ���� alpha
*/
void Matrix::SetRotationMatrix(double alpha) {
	SetRotationMatrixbySinCos(sin(alpha), cos(alpha));
}
/*
 ������������� ������� ������� � �������� ������� ������������� �������� �� ������ (tx, ty)
*/
void Matrix::SetTranslationMatrix(double tx, double ty) {
	SetUnit();
	//
	//data[0][0] = 1 - tx;
	//data[1][1] = 1 - ty;
	data[2][0] = tx;
	data[2][1] = ty;
}
void Matrix::SetTranslationMatrix1() {
	SetUnit();
	data[0][0] = 0.0; // a // a d 0 //
	data[0][1] = -1.0; // d // b e 0 //
	//data[0][2] = 0.1; // 0 // c f 1 //
	data[1][0] = 1.0; // b
	data[1][1] = 0.0; // e
	//data[1][2] = 0.1; // 0 
	//data[2][0] = 0.1; // c
	//data[2][1] = 0.1; // f
	//data[2][2] = 1.0; // 1
}
/*
 �������� ������� ������� �� �������, ���������� � �������� ���������
*/
void Matrix::MultiplyMatrices(Matrix& right) {
	double temp[3][3];
	double val;
	memcpy(temp, data, sizeof(data));
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			val = 0;
			for (int v = 0; v < 3; v++) {
				val += temp[i][v] * right.data[v][j];
			}
			data[i][j] = val;
		}
	}
}
/*
 ����������� �����, ���������� � �������� ��������� �� ������� �������.
 ��� ���� ��������� ������� ������� �� �����������
*/
void Matrix::ApplyMatrixtoPoint(_Point& point) {
	double _x, _y;
	_x = point.x;
	_y = point.y;
	point.x = _x * data[0][0] + _y * data[1][0] + data[2][0];
	point.y = _x * data[0][1] + _y * data[1][1] + data[2][1];
}