#include <math.h>
typedef double vec_float;
class vec
{
public:
	vec_float x, y;
	vec() {}
	vec(vec_float xx, vec_float yy)
	{
		x = xx;
		y = yy;
	}
	vec(const vec& vector) {
		x = vector.x;
		y = vector.y;
	}
	inline void set(vec_float xx, vec_float yy)
	{
		x = xx;
		y = yy;
	}
	inline vec operator + (vec t) // ��������
	{
		return vec(x + t.x, y + t.y);
	}
	inline vec operator - (vec t) // ���������
	{
		return vec(x - t.x, y - t.y);
	}
	inline vec operator * (vec_float t) // ������������ �� �����
	{
		return vec(x * t, y * t);
	}
	inline vec_float operator * (vec t) // ��������� ������������
	{
		return x * t.x + y * t.y;
	}
	inline vec_float operator ^ (vec t) // ����� ���������� ���������� ������������ � ������ �����������
	{
		return x * t.y - y * t.x;
	}
	inline vec_float length() // ����� �������
	{
		return sqrt(x * x + y * y);
		//return x * x + y * y;
	}
	inline vec unit() // ������������ �������
	{
		vec_float l = length();
		if (l == 0.0f) return vec(0.0f, 0.0f);
		return vec(x / l, y / l);
	}
	inline bool zero() // ���������� ������� �� ������
	{
		return (x == 0.0f) && (y == 0.0f);
	}
	inline bool equals(vec t) // ��������� ������� �� ������ ����������
	{
		return (x == t.x) && (y == t.y);
	}
};
