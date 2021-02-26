#include <windows.h>
#include <stdint.h>

#define internal static
#define local_persist static
#define global_variable static


// TODO(jarek): This is temporarily global

struct win32_offscreen_buffer 
{
    BITMAPINFO Info;
    void *Memory;
    int Width;
    int Height;
    int Pitch;
    int BytesPerPixel = 4;
};

global_variable bool Running;
global_variable win32_offscreen_buffer GlobalBackBuffer;


internal void RenderGradiant(win32_offscreen_buffer *Buffer, int BlueOffset, int GreenOffset)
{
    uint8_t *Row = (uint8_t *)Buffer->Memory;
    for(int Y = 0; Y < Buffer->Height; ++Y)
    {
        uint32_t *Pixel = (uint32_t *)Row;
        for(int X = 0; X < Buffer->Width; ++X)
        {
            uint8_t Blue = (X + BlueOffset);
            uint8_t Green = (Y + GreenOffset);
            //Blue channel
            *Pixel++ = ((Green << 8) | Blue);    
        }
        Row += Buffer->Pitch;
    }
}

internal void Win32ResizeDIBSection(win32_offscreen_buffer *Buffer, int Width, int Height)
{
    /* TODO(jarek): Review this, potentially dont free it first. 
    free after or free first if that fails*/
    if(Buffer->Memory)
    {
        VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
    }

    Buffer->Width = Width;
    Buffer->Height = Height;

    Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
    Buffer->Info.bmiHeader.biWidth = Buffer->Width;
    Buffer->Info.bmiHeader.biHeight = -Buffer->Height; // NOTE(jarek): a negative value will make StretchDIBits() to have its origin in the top left corner.
    Buffer->Info.bmiHeader.biPlanes = 1;
    Buffer->Info.bmiHeader.biBitCount = 32;
    Buffer->Info.bmiHeader.biCompression = BI_RGB;

    int BitmapMemorySize = (Buffer->Width*Buffer->Height)*Buffer->BytesPerPixel;
    Buffer->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
 
    Buffer->Pitch = Width*Buffer->BytesPerPixel;
}

internal void Win32DisplayBufferInWindow(HDC DeviceContext, RECT ClientRect, win32_offscreen_buffer *Buffer, int X, int Y, int Width, int Height)
{
    int WindowWidth = ClientRect.right - ClientRect.left;
    int WindowHeight = ClientRect.bottom - ClientRect.top;
    StretchDIBits(DeviceContext,
                  /*
                  X, Y, Width, Height,
                  X, Y, Width, Height,
                  */
                  0, 0, Buffer->Width, Buffer->Height,
                  0, 0, WindowWidth, WindowHeight,
                  Buffer->Memory,
                  &Buffer->Info,
                  DIB_RGB_COLORS, SRCCOPY);
}


LRESULT CALLBACK Win32MainWindowCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
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
            Win32ResizeDIBSection(&GlobalBackBuffer, Width, Height);
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

            RECT ClientRect;
            GetClientRect(Window, &ClientRect);
            Win32DisplayBufferInWindow(DeviceContext, ClientRect, &GlobalBackBuffer, X, Y, Width, Height);
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
int CALLBACK WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CommandLine, int ShowCode)
{
    WNDCLASS WindowClass = {};

    // TODO(jarek): Double check what flags are actually necessary
    WindowClass.style = CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = Win32MainWindowCallback;
    WindowClass.hInstance = Instance;
    //WindowClass.hIcon; //will add icon later on, leave commented for the time being
    WindowClass.lpszClassName = __TEXT("CustomWindowClass");
    if (RegisterClass(&WindowClass))
    {
        HWND Window = CreateWindowEx(0, WindowClass.lpszClassName, __TEXT("Template Window"), WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, Instance, 0);
        
        if (Window)
        {
            int BlueOffset = 0;
            int GreenOffset = 0;

            Running = true;
            while(Running)
            {
                MSG Message;
                while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
                {
                    if(Message.message == WM_QUIT)
                    {
                        Running = false;
                    }
                    TranslateMessage(&Message);
                    DispatchMessage(&Message);
                }

                RenderGradiant(&GlobalBackBuffer, BlueOffset, GreenOffset);

                HDC DeviceContext = GetDC(Window);
                RECT ClientRect;
                GetClientRect(Window, &ClientRect);
                int WindowWidth = ClientRect.right - ClientRect.left;
                int WindowHeight = ClientRect.bottom - ClientRect.top;
                Win32DisplayBufferInWindow(DeviceContext, ClientRect, &GlobalBackBuffer, 0, 0, WindowWidth, WindowHeight);
                ReleaseDC(Window, DeviceContext);
                
                ++BlueOffset;
                ++GreenOffset;
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