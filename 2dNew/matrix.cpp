#include <math.h>
#include <memory.h>
#include "matrix.h"
#include "geometry.h"
/*
  онструктор, инициализирует матрицу единичной (матрица тождественного преобразовани€)
*/
Matrix::Matrix() {
	SetUnit();
}
/*
 “екуща€ матрица становитс€ единичной
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
 ”станавливает текущую матрицу в качестве матрицы вращени€ на угол alpha,
 заданный косинусом и синусом
*/
void Matrix::SetRotationMatrixbySinCos(double sinalpha, double cosalpha) {
	SetUnit();
	data[0][0] = cosalpha;
	data[0][1] = sinalpha;
	data[1][0] = -sinalpha;
	data[1][1] = cosalpha;
}
/*
 ”станавливает текущую матрицу в качестве матрицы вращени€ на угол alpha
*/
void Matrix::SetRotationMatrix(double alpha) {
	SetRotationMatrixbySinCos(sin(alpha), cos(alpha));
}
/*
 ”станавливает текущую матрицу в качестве матрицы параллельного переноса на вектор (tx, ty)
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
 ”множает текущую матрицу на матрицу, переданную в качестве параметра
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
 ѕеремножает точку, переданную в качестве параметра на текущую матрицу.
 ѕри этом последний столбец матрицы не учитываетс€
*/
void Matrix::ApplyMatrixtoPoint(_Point& point) {
	double _x, _y;
	_x = point.x;
	_y = point.y;
	point.x = _x * data[0][0] + _y * data[1][0] + data[2][0];
	point.y = _x * data[0][1] + _y * data[1][1] + data[2][1];
}