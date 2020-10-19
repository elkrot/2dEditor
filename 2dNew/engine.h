#include "action.h"
#include "viewport.h"
#include <vector>
#include "base_shape.h"
class Engine {
	//Matrix current_rot;
	Action* action;
	std::vector<IBaseShape*>  shapes;
	 POINTS ptsBegin;        // �������� �����
	 POINTS ptsEnd;          // ����� �������� ����� 
	 POINTS ptsPrevEnd;      // ���������� �������� ����� 
	 BOOL fPrevLine = FALSE; // ������ ���������� ����� 
public:
	Viewport viewport;
	void Draw(HDC hdc);
	void SetAction(Action* _action);
	void AddShape();
	void MouseMove(HDC hdc, POINTS ptsEnd);
	void SetPtsBegin(POINTS ptBegin);
	void SetPrevLine(BOOL fPrevLine);
};
