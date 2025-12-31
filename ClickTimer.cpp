// ClickTimer.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "ClickTimer.h"
#include <iostream>
using namespace std;

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
BOOL STATUS_APP = true;                        // статус таймера
WCHAR text[MAX_LOADSTRING];                  // Текст на окне
long int before;
long int after;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLICKTIMER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLICKTIMER));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLICKTIMER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CLICKTIMER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void FillWindowColor(HWND hWnd, PAINTSTRUCT ps, HDC hdc, COLORREF color) {
    HBRUSH brush = CreateSolidBrush(color);
    FillRect(hdc, &ps.rcPaint, brush);
    DeleteObject(brush);
}


void DrawWText(HWND hWnd, PAINTSTRUCT ps, HDC hdc, LPCWSTR text) {
    RECT rect = { 0, 0, 100, 200 };
    GetClientRect(hWnd, &rect);
    SetTextColor(hdc, RGB(0x00, 0x00, 0x00));
    rect.left = 20;
    rect.top = 20;
    DrawText(hdc, text, -1, &rect, DT_SINGLELINE);
    SetBkMode(hdc, TRANSPARENT);
}

void DrawWResult(HWND hWnd, PAINTSTRUCT ps, HDC hdc, LPCWSTR text) {
    RECT rect = { 0, 0, 100, 200 };
    GetClientRect(hWnd, &rect);
    SetTextColor(hdc, RGB(0x00, 0x00, 0x00));
    rect.left = 20;
    rect.top = 50;
    DrawText(hdc, text, -1, &rect, DT_SINGLELINE);
    SetBkMode(hdc, TRANSPARENT);
}


void DrawScene(HWND hWnd, PAINTSTRUCT ps, HDC hdc) {
    int latency = rand() % 4000 + 2000;
    wchar_t buffer[256];
    wsprintfW(buffer, L"%d", latency);

    if (STATUS_APP == false) {
        STATUS_APP = true;
        FillWindowColor(hWnd, ps, hdc, RGB(62, 95, 138));
        DrawWText(hWnd, ps, hdc, L"Wait for green");
        Sleep(latency);
        FillWindowColor(hWnd, ps, hdc, RGB(62, 138, 95));
        before = GetTickCount();

        
    }
    else {
        STATUS_APP = false;
        wsprintfW(buffer, L"%d", after - before);
        DrawWText(hWnd, ps, hdc, L"Click for begin");
        DrawWResult(hWnd, ps, hdc, buffer);
    }
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            DrawScene(hWnd, ps, hdc);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_LBUTTONDOWN:
        InvalidateRect(hWnd, NULL, TRUE);
        after = GetTickCount();
        if (STATUS_APP == false) {
           // STATUS_APP = true;
           // int latency = rand() % 4000 + 2000;    
        }
        else {

        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
