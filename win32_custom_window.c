#include <windows.h>
#include <stdio.h>

int main()
{
    printf("Hello, world!");
}


/*CustomWindowProc(   HWND    hwnd,
                    UINT    uMsg,
                    WPARAM  wParam,
                    LPARAM  lParam)
{
    LRESULT result = 0;
    switch(uMsg)
    {
        default:
        {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }break;
    }
    return result;
};

int WinMain(HINSTANCE   hInstance,
            HINSTANCE   hPrevInstance,
            LPSTR       pCmdLine,
            int         nCmdShow)
{
    #define WINDOW_CLASS L"mainWindow";

    WNDCLASSW window_class = {};
    window_class.style          = 0;
    window_class.lpfnWndProc    = CustomWindowProc;
    window_class.hInstance      = hInstance;
    window_class.lpszClassName  = WINDOW_CLASS;

    ATOM atom = RegisterClassW(&window_class);
}
*/