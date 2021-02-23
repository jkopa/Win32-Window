#include <windows.h>
#include <stdio.h>

LRESULT CALLBACK
MainWindowCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
    LRESULT Result = 0;

    // TODO(jarek): WIP case statements to handle messages from windows
    switch (Message)
    {
    case WM_QUIT:
    {
        PostQuitMessage(0);
        OutputDebugStringA("WM_QUIT\n");
    }
    break;
    case WM_SIZE:
    {
        OutputDebugStringA("WM_SIZE\n");
    }
    break;
    case WM_DESTROY:
    {
        OutputDebugStringA("WM_DESTROY\n");
    }
    break;
    case WM_CLOSE:
    {
        PostQuitMessage(0);
        OutputDebugStringA("WM_CLOSE\n");
    }
    break;
    case WM_ACTIVATEAPP:
    {
        OutputDebugStringA("WM_ACTIVATEAPP\n");
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT paint;
        HDC deviceConstext = BeginPaint(Window, &paint);
        LONG x = paint.rcPaint.left;
        LONG y = paint.rcPaint.top;
        LONG Height = paint.rcPaint.bottom - paint.rcPaint.top;
        LONG Width = paint.rcPaint.right - paint.rcPaint.left;
        PatBlt(deviceConstext, x, y, Width, Height, BLACKNESS);
        EndPaint(Window, &paint);
        OutputDebugStringA("WM_PAINT\n");
    }
    break;
    default:
    {
        Result = DefWindowProc(Window, Message, WParam, LParam);
    }
    break;
    }
    return (Result);
}

//Applcation entry point
int CALLBACK
WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CommandLine, int ShowCode)
{
    WNDCLASS WindowClass = {};

    // TODO(jarek): Double check what flags are actually necessary
    WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = MainWindowCallback;
    WindowClass.hInstance = Instance;
    //WindowClass.hIcon; //will add icon later on, leave commented for the time being
    WindowClass.lpszClassName = __TEXT("CustomWindowClass");

    if (RegisterClass(&WindowClass))
    {
        HWND WindowHandle =
            CreateWindowEx(
                0,
                WindowClass.lpszClassName,
                __TEXT("Template Window"),
                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                0,
                0,
                Instance,
                0);

        if (WindowHandle)
        {
            MSG Message;
            for (;;)
            {
                
                BOOL MessageResult = GetMessage(&Message, 0, 0, 0);
                DispatchMessage(&Message);
                if(MessageResult > 0)
                {
                    TranslateMessage(&Message);
                    DispatchMessage(&Message);
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            //TODO Logging
            fprintf(stderr, "Create Window failed!\n");
        }
    }
    else
    {
        //TODO Logging
        fprintf(stderr, "Register Window class failed!\n");
    }
    return 0;
}
//end of program