#include <windows.h>
#include "resource.h"
#include <fstream>
#include <locale>
#include <cassert>

#include "Logic.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPWSTR IpszClass = L"LogicGenerator";

float TotalBoxSize;
const float TotalBoxSizeArr[4] =
    {370.f, 370.f, 370.5f, 372.f};
int currentBoxSize;
float divBoxSize;

int index;

const int boxStartPointXArr[4] =
    {301, 301, 302, 301};
const int boxStartPointYArr[4] = 
    {108, 110, 110, 110};

int boxStartPointX; 
int boxStartPointY; 

CellState arr[ArSize][ArSize];
bool isModified = false;
bool isChecked = false;

enum
{
    Unclicked,
    Color,
    Uncolor,
} clickState;

#define ID_FIELDSIZE_COMBO 100
HWND hCombo_FieldSize;
#define ID_CLEAR 101
HWND hButton_Clear;
#define ID_SAVE 102
HWND hButton_Save;
#define ID_LOAD 103
HWND hButton_Load;
#define ID_CHECK 104
HWND hButton_Check;
#define ID_EXPORT 105
HWND hButton_Export;

void init();
bool Save(HWND hWnd);
bool Export(HWND hWnd);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
					 LPSTR IpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = IpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(IpszClass, IpszClass, WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 
						946, 508, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while(GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch(iMessage)
	{
	case WM_CREATE:

        {
        	hCombo_FieldSize=CreateWindow(L"combobox",NULL,WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL
		    ,820,10,100,200,hWnd,(HMENU)ID_FIELDSIZE_COMBO,g_hInst,NULL);

            SendMessage(hCombo_FieldSize, CB_ADDSTRING, 0, (LPARAM)L"5x5");
            SendMessage(hCombo_FieldSize, CB_ADDSTRING, 0, (LPARAM)L"10x10");
            SendMessage(hCombo_FieldSize, CB_ADDSTRING, 0, (LPARAM)L"15x15");
            SendMessage(hCombo_FieldSize, CB_ADDSTRING, 0, (LPARAM)L"20x20");

            hButton_Clear = CreateWindow(L"button", L"초기화", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
	        	810, 50, 120, 50, hWnd, (HMENU)ID_CLEAR, g_hInst, NULL);
            hButton_Load = CreateWindow(L"button", L"불러오기", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
	        	810, 350, 120, 50, hWnd, (HMENU)ID_LOAD, g_hInst, NULL);
            hButton_Save = CreateWindow(L"button", L"저장", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
	        	810, 420, 120, 50, hWnd, (HMENU)ID_SAVE, g_hInst, NULL);
            hButton_Check = CreateWindow(L"button", L"체크하기", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                810, 280, 120, 50, hWnd, (HMENU)ID_CHECK, g_hInst, NULL);
            hButton_Export = CreateWindow(L"button", L"내보내기", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                810, 210, 120, 50, hWnd, (HMENU)ID_EXPORT, g_hInst, NULL);
        }

        clickState = Unclicked;
        index = 3;
        //currentBoxSize = 20;

        init();
		return 0;

    case WM_CLOSE:
        if(isModified)
        {
            int mbResult = MessageBox(hWnd, L"저장하시겠습니까?", L"모아모아큥♡", MB_YESNOCANCEL);

            switch(mbResult)
            {
            case IDOK:
                {
                    bool result = Save(hWnd);

                    if(result)
                        break;
                    else
                        return 0;
                }

            case IDNO:
                break;

            case IDCANCEL:
                return 0;
            }
        }
        break;

	case WM_PAINT:
        {
            HBITMAP MyBitmap, OldBitmap;
            HDC MemDC;

            HBRUSH paintedBrush;
            HBRUSH redPaintedBrush;
            HBRUSH yellowPaintedBrush;
            //HBRUSH unpaintedBrush;
            HBRUSH def_brush;
            HPEN pen;
            HPEN def_pen;

		    hdc = BeginPaint(hWnd, &ps);

            MemDC = CreateCompatibleDC(hdc);
            switch(currentBoxSize)
            {
                case 20:
                    MyBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_UI20));
                    break;
                case 15:
                    MyBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_UI15));
                    break;
                case 10:
                    MyBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_UI10));
                    break;
                case 5:
                    MyBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_UI05));
                    break;
            }
            OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);
            StretchBlt(hdc, 0, 0, 800, 480, MemDC, 0, 0, 800, 480, SRCCOPY);
            SelectObject(MemDC, OldBitmap);
            DeleteObject(MyBitmap);
            DeleteDC(MemDC);

            pen = (HPEN)GetStockObject(NULL_PEN);
            def_pen = (HPEN)SelectObject(hdc, pen);

            paintedBrush =  CreateSolidBrush(RGB(0, 0, 0));
            redPaintedBrush = CreateSolidBrush(RGB(64, 0, 0));
            yellowPaintedBrush = CreateSolidBrush(RGB(255, 255, 212));
            //unpaintedBrush =  CreateSolidBrush(RGB(255, 255, 255));
    
            for(int i = 0; i < currentBoxSize; i++)
            {
                for(int j = 0; j < currentBoxSize; j++)
                {
                    if(arr[i][j] == White)
                        //def_brush = (HBRUSH)SelectObject(hdc, unpaintedBrush);
                        continue;
                    else if(arr[i][j] == Black)
                        def_brush = (HBRUSH)SelectObject(hdc, paintedBrush);
                    else if(arr[i][j] == Red)
                        def_brush = (HBRUSH)SelectObject(hdc, redPaintedBrush);
                    else if(arr[i][j] == Yellow)
                        def_brush = (HBRUSH)SelectObject(hdc, yellowPaintedBrush);
                    else
                        assert(0);
                        
                    
                    Rectangle(hdc, static_cast<int>(boxStartPointX + divBoxSize * i), static_cast<int>(boxStartPointY + divBoxSize * j),
                        static_cast<int>(boxStartPointX + divBoxSize * (i + 1)), static_cast<int>(boxStartPointY + divBoxSize * (j + 1)));

                    SelectObject(hdc, def_brush);
                }
            }

            SelectObject(hdc, def_pen);
            DeleteObject(paintedBrush);
            DeleteObject(redPaintedBrush);
            DeleteObject(yellowPaintedBrush);
            //DeleteObject(uunpaintedBrush);
            DeleteObject(pen);
        }

		EndPaint(hWnd, &ps);
		return 0;

    case WM_LBUTTONDOWN:
        {
    		int x=LOWORD(lParam);
	    	int y=HIWORD(lParam);
            
            if(x >= boxStartPointX && x < boxStartPointX + TotalBoxSize
                && y >= boxStartPointY && y < boxStartPointY + TotalBoxSize)
            {
                x -= boxStartPointX;
                y -= boxStartPointY;
                x = static_cast<int>(x / divBoxSize);
                y = static_cast<int>(y / divBoxSize);

                if (x < 0 || y < 0 || x >= currentBoxSize || y >= currentBoxSize)
                    break;

                arr[x][y] = (arr[x][y] == Black ? White : Black);

                if(arr[x][y] == White)
                    clickState = Uncolor;
                else
                    clickState = Color;

                isModified = true;
                isChecked = false;

                InvalidateRect(hWnd, nullptr, false);
            }

            SetCapture(hWnd);
        }
        return 0;

    case WM_MOUSEMOVE:
        if(clickState != Unclicked)
        {
    		int x=LOWORD(lParam);
	    	int y=HIWORD(lParam);
            
            if(x >= boxStartPointX && x < boxStartPointX + TotalBoxSize
                && y >= boxStartPointY && y < boxStartPointY + TotalBoxSize)
            {
                x -= boxStartPointX;
                y -= boxStartPointY;
                x = static_cast<int>(x / divBoxSize);
                y = static_cast<int>(y / divBoxSize);
            }

            if (x < 0 || y < 0 || x >= currentBoxSize || y >= currentBoxSize)
                break;
                
            CellState target = (clickState == Color ? Black : White);

            if(arr[x][y] != target)
            {
                arr[x][y] = target;

                InvalidateRect(hWnd, nullptr, false);
            }
        }
        break;

    case WM_LBUTTONUP:
        clickState = Unclicked;
        ReleaseCapture();
        break;

    case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
            case ID_FIELDSIZE_COMBO:
                switch(HIWORD(wParam))
                {
                case CBN_SELCHANGE:
                    if(isModified)
                    {
                        int mbResult = MessageBox(hWnd, L"현재 내용이 초기화됩니다. 정말 바꾸겠습니까?", L"모아모아큥♡", MB_YESNO);

                        switch(mbResult)
                        {
                        case IDYES:
                            break; // go next block

                        case IDNO:
                            return 0;;
                        }
                    }

                    index = SendMessage(hCombo_FieldSize, CB_GETCURSEL, 0, 0);
                    init();
                    InvalidateRect(hWnd, nullptr, false);
                    break;
                }
                break;

            case ID_CLEAR:
                if(isModified)
                {
                    int mbResult = MessageBox(hWnd, L"정말로 초기화하겠습니까?", L"모아모아큥♡", MB_YESNO);

                    switch(mbResult)
                    {
                    case IDYES:
                        break; // go next block

                    case IDNO:
                        return 0;;
                    }
                }

                init();
                InvalidateRect(hWnd, nullptr, false);
                break;

            case ID_SAVE:
                {
                    bool checkResult = Check(arr, currentBoxSize);

                    if(checkResult)
                    {
                        bool saveResult = Save(hWnd);
                        if(saveResult)
                            isModified = false;
                    }
                    else
                    {
                        MessageBox(hWnd, L"답이 유일하지 않아 저장할 수 없습니다!", L"ㅠㅠ", MB_OK);

                        InvalidateRect(hWnd, nullptr, false);
                    }
                }
                break;

            case ID_EXPORT:
                {
                    bool checkResult = Check(arr, currentBoxSize);

                    if(checkResult)
                    {
                        bool saveResult = Export(hWnd);
                        if(saveResult)
                            isModified = false;
                    }
                    else
                    {
                        MessageBox(hWnd, L"답이 유일하지 않아 저장할 수 없습니다!", L"ㅠㅠ", MB_OK);

                        InvalidateRect(hWnd, nullptr, false);
                    }
                }
                break;

            case ID_LOAD:
                if(isModified)
                {
                    int mbResult = MessageBox(hWnd, L"현재 편집한 패턴을 저장하시겠습니까?", L"모아모아큥♡", MB_YESNOCANCEL);

                    switch(mbResult)
                    {
                    case IDYES:
                        {
                            bool result = Save(hWnd);

                            if(result)
                                break;
                            else
                                return 0;
                        }

                    case IDNO:
                        break;

                    case IDCANCEL:
                        return 0;
                    }
                }

		        {
			        OPENFILENAME OFN;
			        TCHAR str[300];
			        TCHAR lpstrFile[MAX_PATH]=L"";
			        std::wifstream fin;
			        wchar_t buffer[7];

			        memset(&OFN, 0, sizeof(OPENFILENAME));
			        OFN.lStructSize = sizeof(OPENFILENAME);
			        OFN.hwndOwner=hWnd;
			        OFN.lpstrFilter=TEXT("모에모에네모로직 로직파일(*.moap)\0*.moap\0");
			        OFN.lpstrFile=lpstrFile;
			        OFN.nMaxFile=MAX_PATH;

			        if (GetOpenFileName(&OFN)!=0) 
			        {
				        wsprintf(str,TEXT("%s"),OFN.lpstrFile);
				        fin.open(str);

                        fin >> buffer;
                        if(wcscmp(buffer, L"MOAP") != 0)
                        {
                            MessageBox(hWnd, L"모에모에 네모로직파일이 아니거나 손상되었습니다!", L"모아모아링☆", MB_OK);
                            break;
                        }

                        fin >> index;
                        if(index < 0 || index > 3)
                        {
                            MessageBox(hWnd, L"모에모에 네모로직파일이 아니거나 손상되었습니다!", L"모아모아큥♡", MB_OK);
                            break;
                        }

                        init();

                        for(int y = 0; y < currentBoxSize; y++)
                        {
                            for(int x = 0; x < currentBoxSize; x++)
                            {
                                int i;
                                fin >> i;
                                arr[x][y] = (i == 0 ? White : Black);
                            }
                        }

                        InvalidateRect(hWnd, nullptr, false);
                    }
                }
                break;

            case ID_CHECK:
                {
                    bool result = Check(arr, currentBoxSize);

                    if(result)
                        MessageBox(hWnd, L"이 퍼즐은 답이 유일합니다", L"데헷", MB_OK);
                    else
                        MessageBox(hWnd, L"답이 유일하지 않은 퍼즐입니다ㅣ!", L"ㅠㅠ", MB_OK);

                    InvalidateRect(hWnd, nullptr, false);
                }
                break;
            }
        }
        break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void init()
{
    switch(index)
    {
    case 0:
        currentBoxSize = 5;
        break;

    case 1:
        currentBoxSize = 10;
        break;

    case 2:
        currentBoxSize = 15;
        break;

    case 3:
        currentBoxSize = 20;
        break;
    }

    boxStartPointX = boxStartPointXArr[index];
    boxStartPointY = boxStartPointYArr[index];
    TotalBoxSize = TotalBoxSizeArr[index];

    divBoxSize = TotalBoxSize / currentBoxSize;

    for(int i = 0; i < ArSize; i++)
    {
        for(int j = 0; j < ArSize; j++)
        {
            arr[i][j] = White;
        }
    }

    isModified = false;
}

bool Save(HWND hWnd)
{
    OPENFILENAME OFN;
    TCHAR str[300];
    TCHAR lpstrFile[MAX_PATH]=L"";
    std::wofstream fout;

    memset(&OFN, 0, sizeof(OPENFILENAME));
    OFN.lStructSize = sizeof(OPENFILENAME);
    OFN.hwndOwner=hWnd;
    OFN.lpstrFilter=TEXT("모에모에네모로직 로직파일(*.moap)\0*.moap\0");
    OFN.lpstrFile=lpstrFile;
    OFN.nMaxFile=MAX_PATH;

    if (GetSaveFileName(&OFN)!=0) 
    {
	    wsprintf(str,TEXT("%s"),OFN.lpstrFile);
	    int i = 0;
	    while(str[i] != L'\0')
	    {
		    i++;
	    }

	    if(str[i-5] == L'.' && 
		    (str[i-4] == L'm' || str[i-4] == L'M') && 
		    (str[i-3] == L'o' || str[i-3] == L'O') && 
		    (str[i-2] == L'a' || str[i-2] == L'A') &&
            (str[i-1] == L'p' || str[i-1] == L'p'))
	    {

	    }
	    else
	    {
		    wcscat_s(str, L".moap");
	    }

	    fout.open(str);
        if(!fout.is_open())
        {
            MessageBox(hWnd, L"파일을 저장할 수 없습니다", L"모아모아링☆", MB_OK);
            return false;
        }

	    fout.imbue(std::locale("kor"));

        fout << L"MOAP\n";

        fout << index << std::endl;

        for(int y = 0; y < currentBoxSize; y++)
        {
            for(int x = 0; x < currentBoxSize; x++)
            {
                fout << ((arr[x][y] == White || arr[x][y] == Yellow) ? 0 : 1) << ' ';
            }
            fout << std::endl;
        }

        fout.close();

        return true;
    }

    return false;
}

bool Export(HWND hWnd)
{
    OPENFILENAME OFN;
    TCHAR str[300];
    TCHAR lpstrFile[MAX_PATH]=L"";
    std::wofstream fout;

    memset(&OFN, 0, sizeof(OPENFILENAME));
    OFN.lStructSize = sizeof(OPENFILENAME);
    OFN.hwndOwner=hWnd;
    OFN.lpstrFilter=TEXT("모에모에네모로직 로직파일(*.lua)\0*.lua\0");
    OFN.lpstrFile=lpstrFile;
    OFN.nMaxFile=MAX_PATH;

    if (GetSaveFileName(&OFN)!=0) 
    {
	    wsprintf(str,TEXT("%s"),OFN.lpstrFile);
	    int i = 0;
	    while(str[i] != L'\0')
	    {
		    i++;
	    }

	    if(str[i-4] == L'.' && 
		    (str[i-3] == L'l' || str[i-3] == L'L') && 
		    (str[i-2] == L'u' || str[i-2] == L'U') &&
            (str[i-1] == L'a' || str[i-1] == L'A'))
	    {

	    }
	    else
	    {
		    wcscat_s(str, L".lua");
	    }

	    fout.open(str);
        if(!fout.is_open())
        {
            MessageBox(hWnd, L"파일을 저장할 수 없습니다", L"모아모아링☆", MB_OK);
            return false;
        }

	    fout.imbue(std::locale("kor"));

        fout << L"              Pattern = {\n";

        for(int y = 0; y < currentBoxSize; y++)
        {
            fout << "                '";

            for(int x = 0; x < currentBoxSize; x++)
            {
                fout << ((arr[x][y] == White || arr[x][y] == Yellow) ? 0 : 1);
            }
            fout << "'," << std::endl;
        }

        fout << "              }";

        fout.close();

        return true;
    }

    return false;
}
