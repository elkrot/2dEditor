#include "draw.h"
#include <io.h>      // для функции _setmode
#include <fcntl.h>   // для константы _O_U16TEXT
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
struct Notes {    // структура данных записной книжки
	char Name[60];   // Ф.И.О.
	char Phone[16];  // телефон
	int  Age;        // возраст
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


	sqlite3* db = 0; // хэндл объекта соединение к БД
	char* err = 0;
	
	// открываем соединение
	if (sqlite3_open("my_cosy_database.dblite", &db))
		fprintf(stderr, "Ошибка открытия/создания БД: %s\n", sqlite3_errmsg(db));
	// выполняем SQL
	else if (sqlite3_exec(db, SQL, 0, 0, &err))
	{
		fprintf(stderr, "Ошибка SQL: %sn", err);
		sqlite3_free(err);
	}
	// закрываем соединение
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

	// Заполняем структуру класса окна
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

	// Регистрируем класс окна
	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, (LPCWSTR)"Не могу зарегистрировать класс окна!", (LPCWSTR)"Ошибка", MB_OK);
		return 0;
	}

	// Создаем основное окно приложения
	hWnd = CreateWindow(
		(LPCWSTR)szClassName, // Имя класса 
		L"Шаблон WinAPI приложения", // Текст заголовка
		WS_OVERLAPPEDWINDOW, // Стиль окна 
		50, 50, // Позиция левого верхнего угла 
		600, 800, // Ширина и высота окна 
		(HWND)NULL, // Указатель на родительское окно NULL 
		(HMENU)NULL, // Используется меню класса окна 
		(HINSTANCE)hInstance, // Указатель на текущее приложение
		NULL); // Передается в качестве lParam в событие WM_CREATE

	if (!hWnd)
	{
		MessageBox(NULL, (LPCWSTR)"Не удается создать главное окно!", (LPCWSTR)"Ошибка", MB_OK);
		return 0;
	}

	//HWND hStatusWindow = CreateStatusWindow(WS_CHILD | WS_VISIBLE, L"", hWnd, 5000);

	HMENU MainMenu = CreateMenu();
	HMENU hPopupMenu = CreatePopupMenu();

	AppendMenu(MainMenu, MF_STRING | MF_POPUP, (UINT)hPopupMenu, L"&Файл");
	{
		AppendMenu(hPopupMenu, MF_STRING, 1001, L"Новый");
		AppendMenu(hPopupMenu, MF_SEPARATOR, 1000, L"");

		AppendMenu(hPopupMenu, MF_STRING, 1002, L"Сохранить");
		AppendMenu(hPopupMenu, MF_STRING, 1003, L"Сохранить как...");
		AppendMenu(hPopupMenu, MF_SEPARATOR, 1000, L"");

		AppendMenu(hPopupMenu, MF_STRING, 1004, L"Авторизация");
		AppendMenu(hPopupMenu, MF_SEPARATOR, 1000, L"");

		AppendMenu(hPopupMenu, MF_STRING | MF_CHECKED, 1005, L"Печать");

		AppendMenu(hPopupMenu, MF_SEPARATOR, 1000, L"");
		AppendMenu(hPopupMenu, MF_STRING, 1006, L"Выход");
	}

	AppendMenu(MainMenu, MF_STRING | MF_POPUP, 0, L"&Правка");
	AppendMenu(MainMenu, MF_STRING | MF_POPUP, 0, L"&Окна");
	AppendMenu(MainMenu, MF_STRING | MF_POPUP, 0, L"&Настройки");
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
	MF_BITMAP Пункт меню является растром. Параметр wIDNewItem содержит описатель этого изображения
MF_CHECKED Устанавливает галочку
MF_DISABLED Блокирует пункт меню
MF_ENABLED Снимает блокировку пункта меню
MF_GRAYED Блокирует пункт меню, затемняя серым цветом
MF_MENUBREAK Новый столбец
MF_OWNERDRAW Прорисовка меню выполняется самим приложением
MF_POPUP Заданный пункт связывается со всплывающим меню или подменю
MF_SEPARATOR = &H800& Разделительная черта
MF_STRING = &H0& Текст для пункта меню. Указатель на текст содержится в lpNewItem
MF_UNCHECKED Снимает галочку с пункта меню
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
	// Показываем наше окно
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// Выполняем цикл обработки сообщений до закрытия приложения
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
	OPENFILENAME ofn;       // структура станд. блока диалога
	wchar_t szFile[260];       // буфер для имени файла

	HANDLE hf;              // дескриптор файла

							// Инициализация структуры OPENFILENAME
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

	// Показываем на экране диалоговое окно Открыть (Open).

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
	OPENFILENAME ofn;       // структура станд. блока диалога
	wchar_t szFile[260];       // буфер для имени файла

	HANDLE hf;              // дескриптор файла

							// Инициализация структуры OPENFILENAME
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

	// Показываем на экране диалоговое окно Открыть (Open).

	if (GetSaveFileName(&ofn) == TRUE)
		hf = CreateFile(ofn.lpstrFile, GENERIC_READ,
			0, (LPSECURITY_ATTRIBUTES)NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
			(HANDLE)NULL);
	long errcode;
	errcode = CommDlgExtendedError();



	setlocale(LC_ALL, "ru");
	Notes Note1, Note3, Note;
	// Открыть файл на чтение/запись одновременно
	fstream file("Notebook.dat", ios::binary | ios::in | ios::out);
	Notes Note50 =
	{ "Ельцин Борис Николаевич", "095-222-3322", 64 };
	file.write((char*)&Note, sizeof(Notes)); // замена


	file.seekg(2 * sizeof(Notes));	    // найти и считать Note3
	file.read((char*)&Note3, sizeof(Notes));
	file.seekg(0);				    // найти и считать Note1
	file.read((char*)&Note1, sizeof(Notes));
	file.seekg(0);                    // Note1 <== Note3
	file.write((char*)&Note3, sizeof(Notes));
	file.seekg(2 * sizeof(Notes));      // Note3 <== Note1
	file.write((char*)&Note1, sizeof(Notes));
	//char str[80];
	// Считывать и отображать записи в цикле, пока не eof
	file.seekg(0); // вернуться к началу файла
	while (!file.read((char*)&Note1, sizeof(Notes)).eof()) {
		/*sprintf(str, "%s\tТел: %s\tВозраст: %d",
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
	RECT rcClient;                 // прямоугольник рабочей области 
	POINT ptClientUL;              // верхний левый угол рабочей области
	POINT ptClientLR;              // нижний правый угол рабочей области 
	//static POINTS ptsBegin;        // исходная точка
	//static POINTS ptsEnd;          // новая конечная точка 
	//static POINTS ptsPrevEnd;      // предыдущая конечная точка 
	//static BOOL fPrevLine = FALSE; // флажок предыдущей линии 

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

	case WM_SIZE: // какое раньше событие WM_PAINT или WM_SIZE
		GetClientRect(hWnd, &Rect);
		//engine->viewport.SetWindowSize(Rect.right - Rect.left, Rect.bottom - Rect.top);
		// x = false;
		InvalidateRect(hWnd, NULL, FALSE);
		break; //определение закрытых перем. viewport.Height, viewport.Width;


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
		// Захват ввода данных от мыши. 
		SetCapture(hWnd);
		// Получим экранные координаты рабочей области, 
		// и преобразуем их в рабочие координаты. 
		GetClientRect(hWnd, &rcClient);
		ptClientUL.x = rcClient.left;
		ptClientUL.y = rcClient.top;
		// Добавим по единице с правой и нижней стороны, поскольку 
		// координаты полученные при помощи GetClientRect не 
		// включают в себя крайний левый и крайний нижний пиксели. 
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
	BOOL OK = FALSE; // переменная-флаг: успех или провал вызова функции

	// определим структуру с данными и значения ее полей для построения
	// диалогового окна
	PRINTDLGW printDlgInfo = {}; // всем полям структуры присваиваем значение 0
	printDlgInfo.lStructSize = sizeof(printDlgInfo); // размер структуры в байтах
	printDlgInfo.Flags =
		PD_RETURNDC; // возвратить контекст устройства (принтера)

	// получим контекст устройства (принтера) в структуру printDlgInfo
	OK = PrintDlgW(&printDlgInfo);
	if (!OK)       // если получена ошибка,
		return OK; // завершить функцию ошибкой

	// всё, что нам нужно, возвращено функцией PrintDlgW в структуру printDlgInfo,
	// нужно только извлечь эту информацию:

	// получим из структуры printDlgInfo контекст устройства (принтера)
	*printerDC = printDlgInfo.hDC;

	// получим из структуры printDlgInfo название принтера
	DEVNAMES* pDN = (DEVNAMES*)GlobalLock(printDlgInfo.hDevNames);
	*printerName = (LPWSTR)pDN + pDN->wDeviceOffset;
	GlobalUnlock(printDlgInfo.hDevNames);

	return OK; // завершить функцию успехом
}

#pragma endregion

#pragma region toPrinter
BOOL toPrinter(Engine *engine)
{
	LPCWSTR str;
	str = L"Test";
	HDC printerDC; LPWSTR printerName;// для названия принтера по умолчанию
	// вызываем функцию для получения названия принтера по умолчанию
	if (!GetPrinterFromDlg(&printerDC, &printerName))
	{

		return FALSE; // завершаем программу ошибкой
	}


	// если контекст устройства получен успешно
	if (printerDC != NULL)
	{
		DOCINFOW docinfo; // информация о документе, который будем печатать
		docinfo.cbSize = sizeof(docinfo);      // размер структуры с документом
		docinfo.lpszDocName = L"Мой документ"; // название документа
		if (wcscmp(printerName,
			L"Microsoft XPS Document Writer") == 0)
			docinfo.lpszOutput = L"Распечатанный документ.xps";
		else
			docinfo.lpszOutput = NULL;
		docinfo.lpszDatatype = NULL;
		docinfo.fwType = 0;

		int idJob; // номер задания, отправляемого на принтер
		// проинформируем спулер печати о начале печати нового документа
		idJob = StartDocW(printerDC, &docinfo);
		if (idJob > 0)
		{
			int result;
			// начать печать страницы
			result = StartPage(printerDC);
			if (result > 0)
			{
				
				//TextOutW(printerDC, 100, 100, str, wcslen(str));
				
				//XFORM xForm;
				//SetViewportOrgEx(printerDC, 10, 500, NULL);
				//SetGraphicsMode(printerDC, GM_ADVANCED);
				//SetMapMode(printerDC, MM_LOENGLISH);

				// Найдем середину рабочей области. 
				//SetPixel(printerDC, 0, 0, RGB(0xff, 0, 0));

				//DPtoLP(printerDC, (LPPOINT)&rect, 2);

				// Выберем пустую кисть. 

				SelectObject(printerDC, GetStockObject(HOLLOW_BRUSH));
				//Draw(printerDC);
				engine->Draw(printerDC);


				if (FALSE)
				{
					wcout << L"Не получилось вывести на страницу текст!\n";
					return FALSE;
				}

				// закончить печать страницы
				EndPage(printerDC);
			}
			else
			{
				wcout << L"Не получилось начать печать страницы!\n";
				return FALSE;
			}
			// проинформируем спулер печати об окончании печати документа
			EndDoc(printerDC);
		}
		else
		{
			wcout << L"Не получилось поставить новый документ в очередь печати!\n";
			return FALSE;
		}

		// удалить контекст устройства (принтера), так как он нам больше не нужен
		DeleteDC(printerDC);
	}
	// контекст устройства не получен
	else
	{
		wcout << L"Не удалось получить контекст устройства (принтера)!\n";
		return FALSE;
	}

	return TRUE;
}
#pragma endregion


