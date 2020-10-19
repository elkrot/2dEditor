#include "action.h"
#include "viewport.h"
#include <vector>
#include "base_shape.h"
class Engine {
	//Matrix current_rot;
	Action* action;
	std::vector<IBaseShape*>  shapes;
	 POINTS ptsBegin;        // исходная точка
	 POINTS ptsEnd;          // новая конечная точка 
	 POINTS ptsPrevEnd;      // предыдущая конечная точка 
	 BOOL fPrevLine = FALSE; // флажок предыдущей линии 
public:
	Viewport viewport;
	void Draw(HDC hdc);
	void SetAction(Action* _action);
	void AddShape();
	void MouseMove(HDC hdc, POINTS ptsEnd);
	void SetPtsBegin(POINTS ptBegin);
	void SetPrevLine(BOOL fPrevLine);
};
