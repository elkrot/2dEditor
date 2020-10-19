#include "geometry.h"
#ifndef _MATRIX
class Matrix {
	double data[3][3];
public:
	Matrix();
	void SetUnit();
	void SetRotationMatrix(double alpha);
	void SetRotationMatrixbySinCos(double sinalpha, double cosalpha);
	void SetTranslationMatrix(double tx, double ty);
	void SetTranslationMatrix1();
	void MultiplyMatrices(Matrix& right);
	void ApplyMatrixtoPoint(_Point& point);
};
#define _MATRIX
#endif
