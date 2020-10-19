#include "matrix.h"
#include "vec.h"
#include "base_shape.h"

class Action {
	//ActionMode _mode; 
	IBaseShape* _target;
public:
	vec old_mouse;
	Matrix CurrentMatrix;
	void InitAction(double x, double y);
	void Rotate(double x, double y);
	void Translate(double x, double y);
	void Transform();
};

enum ActionMode
{
	Select,
	Edit,
	Create,
	Remove
};