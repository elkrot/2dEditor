#include "draw.h"
#include <io.h>      // ��� ������� _setmode
#include <fcntl.h>   // ��� ��������� _O_U16TEXT
#include <iostream>
#include <SDL.h>
#include <fstream>
#include <commctrl.h>
#include "sqlite3.h"
#include "base_shape.h"
#include <vector>
#include "engine.h"
using namespace std;

#pragma region Var
struct Notes {    // ��������� ������ �������� ������
	char Name[60];   // �.�.�.
	char Phone[16];  // �������
	int  Age;        // �������
};

const char* SQL = "CREATE TABLE IF NOT EXISTS foo(a,b,c); INSERT INTO FOO VALUES(1,2,3); INSERT INTO FOO SELECT * FROM FOO;";
const double PI = 3.141592653;
void OpenFile(const HWND& hWnd);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL GetPrinterFromDlg(HDC* printerDC, LPWSTR* printerName);
BOOL toPrinter(Engine *engine);
char szClassName[] = "CG_WAPI_Template";
vector<IBaseShape*>  shapes;
#pragma endregion

////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{


	sqlite3* db = 0; // ����� ������� ���������� � ��
	char* err = 0;
	
	// ��������� ����������
	if (sqlite3_open("my_cosy_database.dblite", &db))
		fprintf(stderr, "������ ��������/�������� ��: %s\n", sqlite3_errmsg(db));
	// ��������� SQL
	else if (sqlite3_exec(db, SQL, 0, 0, &err))
	{
		fprintf(stderr, "������ SQL: %sn", err);
		sqlite3_free(err);
	}
	// ��������� ����������
	sqlite3_close(db);


	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "SDL initialization failed. SDL Error: " << SDL_GetError();
	}
	else
	{
		std::cout << "SDL initialization succeeded!";
	}

	HWND hWnd;
	MSG lpMsg;
	WNDCLASS wc;

	// ��������� ��������� ������ ����
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = (LPCWSTR)szClassName;

	// ������������ ����� ����
	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, (LPCWSTR)"�� ���� ���������������� ����� ����!", (LPCWSTR)"������", MB_OK);
		return 0;
	}

	// ������� �������� ���� ����������
	hWnd = CreateWindow(
		(LPCWSTR)szClassName, // ��� ������ 
		L"������ WinAPI ����������", // ����� ���������
		WS_OVERLAPPEDWINDOW, // ����� ���� 
		50, 50, // ������� ������ �������� ���� 
		600, 800, // ������ � ������ ���� 
		(HWND)NULL, // ��������� �� ������������ ���� NULL 
		(HMENU)NULL, // ������������ ���� ������ ���� 
		(HINSTANCE)hInstance, // ��������� �� ������� ����������
		NULL); // ���������� � �������� lParam � ������� WM_CREATE

	if (!hWnd)
	{
		MessageBox(NULL, (LPCWSTR)"�� ������� ������� ������� ����!", (LPCWSTR)"������", MB_OK);
		return 0;
	}

	//HWND hStatusWindow = CreateStatusWindow(WS_CHILD | WS_VISIBLE, L"", hWnd, 5000);

	HMENU MainMenu = CreateMenu();
	HMENU hPopupMenu = CreatePopupMenu();

	AppendMenu(MainMenu, MF_STRING | MF_POPUP, (UINT)hPopupMenu, L"&����");
	{
		AppendMenu(hPopupMenu, MF_STRING, 1001, L"�����");
		AppendMenu(hPopupMenu, MF_SEPARATOR, 1000, L"");

		AppendMenu(hPopupMenu, MF_STRING, 1002, L"���������");
		AppendMenu(hPopupMenu, MF_STRING, 1003, L"��������� ���...");
		AppendMenu(hPopupMenu, MF_SEPARATOR, 1000, L"");

		AppendMenu(hPopupMenu, MF_STRING, 1004, L"�����������");
		AppendMenu(hPopupMenu, MF_SEPARATOR, 1000, L"");

		AppendMenu(hPopupMenu, MF_STRING | MF_CHECKED, 1005, L"������");

		AppendMenu(hPopupMenu, MF_SEPARATOR, 1000, L"");
		AppendMenu(hPopupMenu, MF_STRING, 1006, L"�����");
	}

	AppendMenu(MainMenu, MF_STRING | MF_POPUP, 0, L"&������");
	AppendMenu(MainMenu, MF_STRING | MF_POPUP, 0, L"&����");
	AppendMenu(MainMenu, MF_STRING | MF_POPUP, 0, L"&���������");
	AppendMenu(MainMenu, MF_STRING | MF_POPUP, 0, L"&?");


	MENUINFO mi;
	mi.cbSize = sizeof(mi);
	mi.hbrBack = CreateSolidBrush(RGB(200, 50, 50));
	mi.fMask = MIIM_STATE | MIIM_TYPE | MIIM_SUBMENU | MIIM_ID;
	mi.dwStyle = 0;

	SetMenuInfo(MainMenu, &mi);
	SetMenuInfo(hPopupMenu, &mi);
	SetMenu(hWnd, MainMenu);

	HMENU   hMenu;
	HBITMAP hBitmap;
	hMenu = GetSystemMenu(hWnd, FALSE);
	AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
	AppendMenu(hMenu, MF_STRING, 3000, L"test5");
	hBitmap = (HBITMAP)LoadImage(hInstance, L"pic.bmp", IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
	AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
	AppendMenu(hMenu, MF_BITMAP | MF_STRING, 3001, (PTSTR)(LONG)hBitmap);
	/*
	MF_BITMAP ����� ���� �������� �������. �������� wIDNewItem �������� ��������� ����� �����������
MF_CHECKED ������������� �������
MF_DISABLED ��������� ����� ����
MF_ENABLED ������� ���������� ������ ����
MF_GRAYED ��������� ����� ����, �������� ����� ������
MF_MENUBREAK ����� �������
MF_OWNERDRAW ���������� ���� ����������� ����� �����������
MF_POPUP �������� ����� ����������� �� ����������� ���� ��� �������
MF_SEPARATOR = &H800& �������������� �����
MF_STRING = &H0& ����� ��� ������ ����. ��������� �� ����� ���������� � lpNewItem
MF_UNCHECKED ������� ������� � ������ ����
	*/

	HWND hwndButton = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"OK",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		100,         // x position 
		100,         // y position 
		50,        // Button width
		10,        // Button height
		hWnd,     // Parent window
		NULL,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.
	// ���������� ���� ����
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// ��������� ���� ��������� ��������� �� �������� ����������
	while (GetMessage(&lpMsg, NULL, 0, 0))
	{
		TranslateMessage(&lpMsg);
		DispatchMessage(&lpMsg);
	}
	return (lpMsg.wParam);
}

#pragma endregion


#pragma region OpenFile
void OpenFile(const HWND& hWnd)
{
	OPENFILENAME ofn;       // ��������� �����. ����� �������
	wchar_t szFile[260];       // ����� ��� ����� �����

	HANDLE hf;              // ���������� �����

							// ������������� ��������� OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// ���������� �� ������ ���������� ���� ������� (Open).

	if (GetOpenFileName(&ofn) == TRUE)
		hf = CreateFile(ofn.lpstrFile, GENERIC_READ,
			0, (LPSECURITY_ATTRIBUTES)NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
			(HANDLE)NULL);
	long errcode;
	errcode = CommDlgExtendedError();
}

#pragma endregion


#pragma region SaveFile
void SaveFile(const HWND& hWnd)
{
	OPENFILENAME ofn;       // ��������� �����. ����� �������
	wchar_t szFile[260];       // ����� ��� ����� �����

	HANDLE hf;              // ���������� �����

							// ������������� ��������� OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// ���������� �� ������ ���������� ���� ������� (Open).

	if (GetSaveFileName(&ofn) == TRUE)
		hf = CreateFile(ofn.lpstrFile, GENERIC_READ,
			0, (LPSECURITY_ATTRIBUTES)NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
			(HANDLE)NULL);
	long errcode;
	errcode = CommDlgExtendedError();



	setlocale(LC_ALL, "ru");
	Notes Note1, Note3, Note;
	// ������� ���� �� ������/������ ������������
	fstream file("Notebook.dat", ios::binary | ios::in | ios::out);
	Notes Note50 =
	{ "������ ����� ����������", "095-222-3322", 64 };
	file.write((char*)&Note, sizeof(Notes)); // ������


	file.seekg(2 * sizeof(Notes));	    // ����� � ������� Note3
	file.read((char*)&Note3, sizeof(Notes));
	file.seekg(0);				    // ����� � ������� Note1
	file.read((char*)&Note1, sizeof(Notes));
	file.seekg(0);                    // Note1 <== Note3
	file.write((char*)&Note3, sizeof(Notes));
	file.seekg(2 * sizeof(Notes));      // Note3 <== Note1
	file.write((char*)&Note1, sizeof(Notes));
	//char str[80];
	// ��������� � ���������� ������ � �����, ���� �� eof
	file.seekg(0); // ��������� � ������ �����
	while (!file.read((char*)&Note1, sizeof(Notes)).eof()) {
		/*sprintf(str, "%s\t���: %s\t�������: %d",
			Note1.Name, Note1.Phone, Note1.Age);
		cout << str << endl;*/
	}
	file.close();
}

#pragma endregion

////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region WndProc
LRESULT CALLBACK WndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	RECT Rect;
	HDC hdc;
	HBITMAP hBmp;
	HDC hCmpDC;
	RECT rcClient;                 // ������������� ������� ������� 
	POINT ptClientUL;              // ������� ����� ���� ������� �������
	POINT ptClientLR;              // ������ ������ ���� ������� ������� 
	//static POINTS ptsBegin;        // �������� �����
	//static POINTS ptsEnd;          // ����� �������� ����� 
	//static POINTS ptsPrevEnd;      // ���������� �������� ����� 
	//static BOOL fPrevLine = FALSE; // ������ ���������� ����� 

	static Action* action;
	static Engine* engine;
		POINTS pst, pen;
		
	switch (messg)
	{
	case WM_CREATE:
		engine = new Engine();
		action = new Action(); 
		engine->SetAction(action); 		
		break;

	case WM_SIZE: // ����� ������ ������� WM_PAINT ��� WM_SIZE
		GetClientRect(hWnd, &Rect);
		//engine->viewport.SetWindowSize(Rect.right - Rect.left, Rect.bottom - Rect.top);
		// x = false;
		InvalidateRect(hWnd, NULL, FALSE);
		break; //����������� �������� �����. viewport.Height, viewport.Width;


	case WM_PAINT:
#pragma region WM_PAINT
		GetClientRect(hWnd, &Rect);
		hdc = BeginPaint(hWnd, &ps);
		SetBkColor(hdc, 0xEECCCC);
		engine->Draw(hdc);	
		ReleaseDC(NULL, hdc);
		EndPaint(hWnd, &ps);
#pragma endregion

		break;
	case WM_KEYDOWN:
#pragma region WM_KEYDOWN
		int KeyPressed;
		KeyPressed = int(wParam);
		if (KeyPressed == int('P'))
		{
			_setmode(_fileno(stdout), _O_U16TEXT);
			toPrinter(engine);
		}

		if (KeyPressed == int('O'))
		{
			OpenFile(hWnd);
		}

		if (KeyPressed == int('S'))
		{
			SaveFile(hWnd);
		}
		if (KeyPressed == int('T'))
		{
			action->Transform();
		}


		InvalidateRect(hWnd, NULL, FALSE);
#pragma endregion

		break;
	case WM_LBUTTONDOWN:
#pragma region WM_LBUTTONDOWN
		// ������ ����� ������ �� ����. 
		SetCapture(hWnd);
		// ������� �������� ���������� ������� �������, 
		// � ����������� �� � ������� ����������. 
		GetClientRect(hWnd, &rcClient);
		ptClientUL.x = rcClient.left;
		ptClientUL.y = rcClient.top;
		// ������� �� ������� � ������ � ������ �������, ��������� 
		// ���������� ���������� ��� ������ GetClientRect �� 
		// �������� � ���� ������� ����� � ������� ������ �������. 
		ptClientLR.x = rcClient.right + 1;
		ptClientLR.y = rcClient.bottom + 1;
		ClientToScreen(hWnd, &ptClientUL);
		ClientToScreen(hWnd, &ptClientLR);

		SetRect(&rcClient, ptClientUL.x, ptClientUL.y,
			ptClientLR.x, ptClientLR.y);
		ClipCursor(&rcClient);

		engine->SetPtsBegin(MAKEPOINTS(lParam));
#pragma endregion
		return 0;

	case WM_MOUSEMOVE:
#pragma region WM_MOUSEMOVE 
		if (wParam & MK_LBUTTON)
		{
			hdc = GetDC(hWnd);
			engine->MouseMove(hdc, MAKEPOINTS(lParam));
			ReleaseDC(hWnd, hdc);
		}
#pragma endregion


		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 1001:
			MessageBox(hWnd, TEXT("hit1"), TEXT("caption"), 0);
			break;
		}
		break;
	
	case WM_LBUTTONUP:
#pragma region WM_LBUTTONUP
		engine->AddShape();
		engine->SetPrevLine( FALSE);
		ClipCursor(NULL);
		ReleaseCapture();
#pragma endregion


		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return (DefWindowProc(hWnd, messg, wParam, lParam));
	}
	return (0);
}

#pragma endregion


#pragma region GetPrinterFromDlg
BOOL GetPrinterFromDlg(HDC* printerDC, LPWSTR* printerName)
{
	BOOL OK = FALSE; // ����������-����: ����� ��� ������ ������ �������

	// ��������� ��������� � ������� � �������� �� ����� ��� ����������
	// ����������� ����
	PRINTDLGW printDlgInfo = {}; // ���� ����� ��������� ����������� �������� 0
	printDlgInfo.lStructSize = sizeof(printDlgInfo); // ������ ��������� � ������
	printDlgInfo.Flags =
		PD_RETURNDC; // ���������� �������� ���������� (��������)

	// ������� �������� ���������� (��������) � ��������� printDlgInfo
	OK = PrintDlgW(&printDlgInfo);
	if (!OK)       // ���� �������� ������,
		return OK; // ��������� ������� �������

	// ��, ��� ��� �����, ���������� �������� PrintDlgW � ��������� printDlgInfo,
	// ����� ������ ������� ��� ����������:

	// ������� �� ��������� printDlgInfo �������� ���������� (��������)
	*printerDC = printDlgInfo.hDC;

	// ������� �� ��������� printDlgInfo �������� ��������
	DEVNAMES* pDN = (DEVNAMES*)GlobalLock(printDlgInfo.hDevNames);
	*printerName = (LPWSTR)pDN + pDN->wDeviceOffset;
	GlobalUnlock(printDlgInfo.hDevNames);

	return OK; // ��������� ������� �������
}

#pragma endregion

#pragma region toPrinter
BOOL toPrinter(Engine *engine)
{
	LPCWSTR str;
	str = L"Test";
	HDC printerDC; LPWSTR printerName;// ��� �������� �������� �� ���������
	// �������� ������� ��� ��������� �������� �������� �� ���������
	if (!GetPrinterFromDlg(&printerDC, &printerName))
	{

		return FALSE; // ��������� ��������� �������
	}


	// ���� �������� ���������� ������� �������
	if (printerDC != NULL)
	{
		DOCINFOW docinfo; // ���������� � ���������, ������� ����� ��������
		docinfo.cbSize = sizeof(docinfo);      // ������ ��������� � ����������
		docinfo.lpszDocName = L"��� ��������"; // �������� ���������
		if (wcscmp(printerName,
			L"Microsoft XPS Document Writer") == 0)
			docinfo.lpszOutput = L"������������� ��������.xps";
		else
			docinfo.lpszOutput = NULL;
		docinfo.lpszDatatype = NULL;
		docinfo.fwType = 0;

		int idJob; // ����� �������, ������������� �� �������
		// �������������� ������ ������ � ������ ������ ������ ���������
		idJob = StartDocW(printerDC, &docinfo);
		if (idJob > 0)
		{
			int result;
			// ������ ������ ��������
			result = StartPage(printerDC);
			if (result > 0)
			{
				
				//TextOutW(printerDC, 100, 100, str, wcslen(str));
				
				//XFORM xForm;
				//SetViewportOrgEx(printerDC, 10, 500, NULL);
				//SetGraphicsMode(printerDC, GM_ADVANCED);
				//SetMapMode(printerDC, MM_LOENGLISH);

				// ������ �������� ������� �������. 
				//SetPixel(printerDC, 0, 0, RGB(0xff, 0, 0));

				//DPtoLP(printerDC, (LPPOINT)&rect, 2);

				// ������� ������ �����. 

				SelectObject(printerDC, GetStockObject(HOLLOW_BRUSH));
				//Draw(printerDC);
				engine->Draw(printerDC);


				if (FALSE)
				{
					wcout << L"�� ���������� ������� �� �������� �����!\n";
					return FALSE;
				}

				// ��������� ������ ��������
				EndPage(printerDC);
			}
			else
			{
				wcout << L"�� ���������� ������ ������ ��������!\n";
				return FALSE;
			}
			// �������������� ������ ������ �� ��������� ������ ���������
			EndDoc(printerDC);
		}
		else
		{
			wcout << L"�� ���������� ��������� ����� �������� � ������� ������!\n";
			return FALSE;
		}

		// ������� �������� ���������� (��������), ��� ��� �� ��� ������ �� �����
		DeleteDC(printerDC);
	}
	// �������� ���������� �� �������
	else
	{
		wcout << L"�� ������� �������� �������� ���������� (��������)!\n";
		return FALSE;
	}

	return TRUE;
}
#pragma endregion


