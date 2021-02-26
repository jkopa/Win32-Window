#include <windows.h>

#define internal static
#define local_persist static
#define global_variable static

// TODO(jarek): This is temporarily global
global_variable bool Running;

global_variable BITMAPINFO BitmapInfo;
global_variable void *BitmapMemory;

internal void
Win32ResizeDIBSection(int Width, int Height)
{
    /* TODO(jarek): Review this, potentially dont free it first. 
    free after or free first if that fails*/
    if(BitmapMemory)
    {
        VirtualFree(BitmapMemory, 0, MEM_RELEASE);
    }

    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = Width;
    BitmapInfo.bmiHeader.biHeight = Height;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;

    int BytesPerPixel = 4;
    int BitmapMemorySize = (Width*Height)*BytesPerPixel;
    BitmapMemory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);

}

internal void
Win32UpdateWindow(HDC DeviceContext, int X, int Y, int Width, int Height)
{
    StretchDIBits(DeviceContext,
                  X, Y, Width, Height,
                  X, Y, Width, Height,
                  BitmapMemory,
                  &BitmapInfo,
                  DIB_RGB_COLORS, SRCCOPY);
}


LRESULT CALLBACK 
Win32MainWindowCallback(HWND Window,
                        UINT Message,
                        WPARAM WParam,
                        LPARAM LParam)
{
    LRESULT Result = 0;
    // TODO(jarek): WIP case statements to handle messages from windows
    switch (Message)
    {
        case WM_QUIT:
        {
            Running = false;
            OutputDebugStringA("WM_QUIT\n");
        }break;
        case WM_SIZE:
        {
            RECT ClientRect;
            GetClientRect(Window, &ClientRect);
            int Width = ClientRect.right - ClientRect.left;
            int Height = ClientRect.bottom - ClientRect.top; 
            Win32ResizeDIBSection(Width, Height);
            OutputDebugStringA("WM_SIZE\n");
        }break;
        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);
            int X = Paint.rcPaint.left;
            int Y = Paint.rcPaint.top;
            int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
            int Width = Paint.rcPaint.right - Paint.rcPaint.left;
            Win32UpdateWindow(DeviceContext, X, Y, Width, Height);
            EndPaint(Window, &Paint);
            OutputDebugStringA("WM_PAINT\n");
        }break;
        case WM_DESTROY:
        {
            Running = false;
            OutputDebugStringA("WM_DESTROY\n");
        }break;
        case WM_CLOSE:
        {
            Running = false;
            OutputDebugStringA("WM_CLOSE\n");
        }break;
        case WM_ACTIVATEAPP:
        {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        }break;
        default:
        {
            Result = DefWindowProc(Window, Message, WParam, LParam);
        }break;
    }return (Result);
}

//Applcation entry point
int CALLBACK
WinMain(HINSTANCE Instance,
        HINSTANCE PrevInstance,
        LPSTR CommandLine,
        int ShowCode)
{
    Running = true;
    WNDCLASS WindowClass = {};

    // TODO(jarek): Double check what flags are actually necessary
    WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = Win32MainWindowCallback;
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
            //TODO(jarek): Logging
            MessageBoxA(0, "Error", "Create window failed!",
                MB_OK|MB_ICONINFORMATION);   
        }
    }
    else
    {
        //TODO(jarek): Logging
        MessageBoxA(0, "Error", "Create window class failed!",
                MB_OK|MB_ICONINFORMATION);
    }
    return 0;
}
//end of program