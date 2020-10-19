#include "action.h"
/*
 —игнализирует о начале некоторого действи€
*/
void Action::InitAction(double x, double y) {
	old_mouse.set(x, y);
}
/*
 ѕоворачивает систему координат, опира€сь на текущее
 и предыдущее полощени€ мыши
*/
void Action::Rotate(double x, double y) {
	vec new_mouse(x, y);
	vec_float sina, cosa;
	sina = old_mouse.unit() ^ new_mouse.unit();
	cosa = old_mouse.unit() * new_mouse.unit();
	Matrix Rot;
	Rot.SetRotationMatrixbySinCos(sina, cosa);
	CurrentMatrix.MultiplyMatrices(Rot);
	old_mouse = new_mouse;
}
/*
 ѕеремещает систему координат, опира€сь на текущее
 и предыдущее положени€ мыши
*/
void Action::Translate(double x, double y) {
	vec new_mouse(x, y);
	vec delta = new_mouse - old_mouse;
	Matrix Tr;
	Tr.SetTranslationMatrix(delta.x, delta.y);
	CurrentMatrix.MultiplyMatrices(Tr);
	old_mouse = new_mouse;
}
void Action::Transform() {
	//vec new_mouse(x, y);
	//vec delta = new_mouse - old_mouse;
	Matrix Tr;
	Tr.SetTranslationMatrix1();
	CurrentMatrix.MultiplyMatrices(Tr);
	//old_mouse = new_mouse;
}