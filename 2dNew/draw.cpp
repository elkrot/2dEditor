#include "draw.h"
#include "point.h"
#define RGB(r,g,b) \
   ((COLORREF)(((BYTE)(r)|((WORD)(g)<<8)) | \
   (((DWORD)(BYTE)(b))<<16)))

void Draw(HDC hdc) {

    RECT rect;
    //XFORM xForm;
    SetViewportOrgEx(hdc, 100, 100, NULL);
    SetGraphicsMode(hdc, GM_ADVANCED);
    SetMapMode(hdc, MM_LOENGLISH);
    
    // ������ �������� ������� �������. 
    SetPixel(hdc, 0, 0, RGB(0xff, 0, 0));
    
    DPtoLP(hdc, (LPPOINT)&rect, 2);

    // ������� ������ �����. 

    SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));

    // ������ ������� ����. 

    Ellipse(hdc, (rect.right / 2 - 100), (rect.bottom / 2 + 100),
        (rect.right / 2 + 100), -(rect.bottom / 2 - 100));

    // ������ ���������� ����. 

    Ellipse(hdc, (rect.right / 2 - 94), -(rect.bottom / 2 + 94),
        (rect.right / 2 + 94), -(rect.bottom / 2 - 94));

    // ������ �������. 

    Rectangle(hdc, (rect.right / 2 - 13), (rect.bottom / 2 + 113),
        (rect.right / 2 + 13), (rect.bottom / 2 + 50));
    Rectangle(hdc, (rect.right / 2 - 13), (rect.bottom / 2 + 96),
        (rect.right / 2 + 13), (rect.bottom / 2 + 50));

    // ������ �������������� �����. 

    MoveToEx(hdc, (rect.right / 2 - 150), (rect.bottom / 2 + 0), NULL);
    LineTo(hdc, (rect.right / 2 - 16), (rect.bottom / 2 + 0));

    MoveToEx(hdc, (rect.right / 2 - 13), (rect.bottom / 2 + 0), NULL);
    LineTo(hdc, (rect.right / 2 + 13), (rect.bottom / 2 + 0));

    MoveToEx(hdc, (rect.right / 2 + 16), (rect.bottom / 2 + 0), NULL);
    LineTo(hdc, (rect.right / 2 + 150), (rect.bottom / 2 + 0));

    // ������ ������������ �����. 

    MoveToEx(hdc, (rect.right / 2 + 0), (rect.bottom / 2 - 150), NULL);
    LineTo(hdc, (rect.right / 2 + 0), (rect.bottom / 2 - 16));

    MoveToEx(hdc, (rect.right / 2 + 0), (rect.bottom / 2 - 13), NULL);
    LineTo(hdc,(rect.right / 2 + 0), (rect.bottom / 2 + 13));

    MoveToEx(hdc, (rect.right / 2 + 0), (rect.bottom / 2 + 16), NULL);
    LineTo(hdc, (rect.right / 2 + 0), (rect.bottom / 2 + 150));



    point p;

    p.X = 200;
    p.Y = 200;
    MoveToEx(hdc, p.X, p.Y, NULL);
    LineTo(hdc, 150, -150);
    LineTo(hdc, 50, -200);
    LineTo(hdc, 100, -100);

    MoveToEx(hdc,0, 0,NULL);
    POINT aPoint[4] = { 0, -100, 100, -100, 100, 0, 0, 0 };
    PolylineTo(hdc, aPoint, 4);

    
    Ellipse(hdc, 10, -10,
        200, -200);
    int iVertRes, iHorzRes;
    iVertRes = GetDeviceCaps(hdc, VERTRES);
    iHorzRes = GetDeviceCaps(hdc, HORZRES);
    LPCWSTR str;
    str = L"xxx";
    wchar_t buffer[256];
    wsprintfW(buffer, L"%d x %d", iVertRes, iHorzRes);
    TextOutW(hdc, 200, -200, buffer, wcslen(buffer));

}

const int SCALE = 1;
const int TRANSLATE = 2;
const int ROTATE = 3;
const int  SHEAR = 4;
const int  REFLECT = 5;
const int NORMAL = 6;


void TransformAndDraw(int iTransform, HWND hWnd)
{
    HDC hDC;
    XFORM xForm;
    RECT rect;

    // ������� ���������� DC ���� ����������. 

    hDC = GetDC(hWnd);

    // ��������� ����� ����������� LOENGLISH. ����� ���������� 
    // ������ ��������� ������� ������� �� ������ �������� ���� 
    // ���� � ����� ������ ���� (��� ����� ��������������� 
    // ��� Y ���, ��� �������� ��������� ���������� � �������� 
    // ���������� ������������). ��� ����������� ����������� ����� �������, ��� 
    //  ������������ ������ ��������� ���� ������� �� ����� �������. 

    SetGraphicsMode(hDC, GM_ADVANCED);
    SetMapMode(hDC, MM_LOENGLISH);

    // ��������� ��������������� �������������� �������� ������������ (���������� �� 
    // ������ ���� ������������).

    switch (iTransform)
    {
    case SCALE:        // ������� 1/2 ��������������� �������. 
        xForm.eM11 = (FLOAT)0.5;
        xForm.eM12 = (FLOAT)0.0;
        xForm.eM21 = (FLOAT)0.0;
        xForm.eM22 = (FLOAT)0.5;
        xForm.eDx = (FLOAT)0.0;
        xForm.eDy = (FLOAT)0.0;
        SetWorldTransform(hDC, &xForm);
        break;

    case TRANSLATE:   // ���������� �� ������ �� 3/4 �����. 
        xForm.eM11 = (FLOAT)1.0;
        xForm.eM12 = (FLOAT)0.0;
        xForm.eM21 = (FLOAT)0.0;
        xForm.eM22 = (FLOAT)1.0;
        xForm.eDx = (FLOAT)75.0;
        xForm.eDy = (FLOAT)0.0;
        SetWorldTransform(hDC, &xForm);
        break;

    case ROTATE:      // �������� �� 30 �������� ������ ������� �������. 
        xForm.eM11 = (FLOAT)0.8660;
        xForm.eM12 = (FLOAT)0.5000;
        xForm.eM21 = (FLOAT)-0.5000;
        xForm.eM22 = (FLOAT)0.8660;
        xForm.eDx = (FLOAT)0.0;
        xForm.eDy = (FLOAT)0.0;
        SetWorldTransform(hDC, &xForm);
        break;

    case SHEAR:       // ������� �� ��� X � 
                      // ������������� ������������������ 1.0.  
        xForm.eM11 = (FLOAT)1.0;
        xForm.eM12 = (FLOAT)1.0;
        xForm.eM21 = (FLOAT)0.0;
        xForm.eM22 = (FLOAT)1.0;
        xForm.eDx = (FLOAT)0.0;
        xForm.eDy = (FLOAT)0.0;
        SetWorldTransform(hDC, &xForm);
        break;

    case REFLECT:     // ������� ������������ �������������� ���. 
        xForm.eM11 = (FLOAT)1.0;
        xForm.eM12 = (FLOAT)0.0;
        xForm.eM21 = (FLOAT)0.0;
        xForm.eM22 = (FLOAT)-1.0;
        xForm.eDx = (FLOAT)0.0;
        xForm.eDy = (FLOAT)0.0;
        SetWorldTransform(hDC, &xForm);
        break;

    case NORMAL:      // ��������� ������� ��������� � ��������������. 
        xForm.eM11 = (FLOAT)1.0;
        xForm.eM12 = (FLOAT)0.0;
        xForm.eM21 = (FLOAT)0.0;
        xForm.eM22 = (FLOAT)1.0;
        xForm.eDx = (FLOAT)0.0;
        xForm.eDy = (FLOAT)0.0;
        SetWorldTransform(hDC, &xForm);
        break;

    }

    // ������ �������� ������� �������. 

    GetClientRect(hWnd, (LPRECT)&rect);
    DPtoLP(hDC, (LPPOINT)&rect, 2);

    // ������� ������ �����. 

    SelectObject(hDC, GetStockObject(HOLLOW_BRUSH));

    // ������ ������� ����. 

    Ellipse(hDC, (rect.right / 2 - 100), (rect.bottom / 2 + 100),
        (rect.right / 2 + 100), (rect.bottom / 2 - 100));

    // ������ ���������� ����. 

    Ellipse(hDC, (rect.right / 2 - 94), (rect.bottom / 2 + 94),
        (rect.right / 2 + 94), (rect.bottom / 2 - 94));

    // ������ �������. 

    Rectangle(hDC, (rect.right / 2 - 13), (rect.bottom / 2 + 113),
        (rect.right / 2 + 13), (rect.bottom / 2 + 50));
    Rectangle(hDC, (rect.right / 2 - 13), (rect.bottom / 2 + 96),
        (rect.right / 2 + 13), (rect.bottom / 2 + 50));

    // ������ �������������� �����. 

    MoveToEx(hDC, (rect.right / 2 - 150), (rect.bottom / 2 + 0), NULL);
    LineTo(hDC, (rect.right / 2 - 16), (rect.bottom / 2 + 0));

    MoveToEx(hDC, (rect.right / 2 - 13), (rect.bottom / 2 + 0), NULL);
    LineTo(hDC, (rect.right / 2 + 13), (rect.bottom / 2 + 0));

    MoveToEx(hDC, (rect.right / 2 + 16), (rect.bottom / 2 + 0), NULL);
    LineTo(hDC, (rect.right / 2 + 150), (rect.bottom / 2 + 0));

    // ������ ������������ �����. 

    MoveToEx(hDC, (rect.right / 2 + 0), (rect.bottom / 2 - 150), NULL);
    LineTo(hDC, (rect.right / 2 + 0), (rect.bottom / 2 - 16));

    MoveToEx(hDC, (rect.right / 2 + 0), (rect.bottom / 2 - 13), NULL);
    LineTo(hDC, (rect.right / 2 + 0), (rect.bottom / 2 + 13));

    MoveToEx(hDC, (rect.right / 2 + 0), (rect.bottom / 2 + 16), NULL);
    LineTo(hDC, (rect.right / 2 + 0), (rect.bottom / 2 + 150));

    ReleaseDC(hWnd, hDC);
}

void Draw2() {
    /*  hCmpDC = CreateCompatibleDC(hdc);
       hBmp = CreateCompatibleBitmap(hdc, Rect.right - Rect.left,
           Rect.bottom - Rect.top);
       SelectObject(hCmpDC, hBmp);

       // �������� ������� ������
       LOGBRUSH br;
       br.lbStyle = BS_SOLID;
       br.lbColor = 0xEECCCC;
       HBRUSH brush;
       brush = CreateBrushIndirect(&br);
       FillRect(hCmpDC, &Rect, brush);
       DeleteObject(brush);*/

       // ����� ������ �� ��������� hCmpDC

      // Draw(hCmpDC);
    
    // �������� ����������� �� �������� ��������� �� �����
   /* SetStretchBltMode(hdc, COLORONCOLOR);
    BitBlt(hdc, 0, 0, Rect.right - Rect.left, Rect.bottom - Rect.top,
        hCmpDC, 0, 0, SRCCOPY);

    // ������� �������� ��������� �������
    DeleteDC(hCmpDC);
    DeleteObject(hBmp);
    hCmpDC = NULL;*/
}