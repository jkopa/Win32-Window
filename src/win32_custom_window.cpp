#include <windows.h>

#define internal static
#define local_persist static
#define global_variable static

// TODO This is temporarily global
global_variable bool Running;

LRESULT CALLBACK
MainWindowCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
    LRESULT Result = 0;

    // TODO(jarek): WIP case statements to handle messages from windows
    switch (Message)
    {
    case WM_QUIT:
    {
        Running = false;
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
        Running = false;
        OutputDebugStringA("WM_DESTROY\n");
    }
    break;
    case WM_CLOSE:
    {
        Running = false;
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
            while(Running)
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
            MessageBoxA(0, "Error", "Create window failed!",
                MB_OK|MB_ICONINFORMATION);   
        }
    }
    else
    {
        //TODO Logging
        MessageBoxA(0, "Error", "Create window class failed!",
                MB_OK|MB_ICONINFORMATION);
    }
    return 0;
}
//end of program