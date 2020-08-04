#include <windows.h>
#include <stdio.h>
#include <vulkan/vulkan.h>

int keepRunning;

LRESULT CALLBACK
MainWindowCallback(HWND window, UINT message, WPARAM WParam, LPARAM LParam)
{
    LRESULT result = 0;
    
    // TODO(jarek): WIP case statements to handle messages from windows
    switch(message)
    {
        case WM_QUIT:
        {
            keepRunning = 0;
            OutputDebugStringA("WM_QUIT\n");
        }break;
        case WM_SIZE:
        {
            OutputDebugStringA("WM_SIZE\n");
        }break;
        case WM_DESTROY:
        {
            OutputDebugStringA("WM_DESTROY\n");
        }break;
        case WM_CLOSE:
        {
            keepRunning = 0;
            OutputDebugStringA("WM_CLOSE\n");
        }break;
        case WM_ACTIVATEAPP:
        {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        }break;
        case WM_PAINT:
        {
            // TODO(jarek):research PatBlt()... I have no clue what it actually does
            PAINTSTRUCT paint;
            HDC deviceConstext = BeginPaint(window, &paint);
            LONG x = paint.rcPaint.left;
            LONG y = paint.rcPaint.top;
            LONG height = paint.rcPaint.bottom - paint.rcPaint.top;
            LONG width = paint.rcPaint.right - paint.rcPaint.left;
            PatBlt(deviceConstext, x, y, width, height, BLACKNESS);
            EndPaint(window, &paint);
            OutputDebugStringA("WM_PAINT\n");
        }break;
        default:
        {
            //OutputDebugStringA("Default\n");
            result = DefWindowProc(window, message, WParam, LParam);
        } break;
    }
    return(result);
}


//Applcation entry point
int CALLBACK
WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR commandLine, int showCode)
{
    WNDCLASS windowClass = {0}; 
    
    // TODO(jarek): Double check what flags are actually necessary 
    windowClass.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
    windowClass.lpfnWndProc = MainWindowCallback;
    windowClass.hInstance = instance; 
    //windowClass.hIcon; //will add icon later on, leave commented for the time being
    windowClass.lpszClassName = "CustomWindowClass";
    
    if (RegisterClass(&windowClass))
    {
        HWND windowHandle = CreateWindowEx(0, windowClass.lpszClassName, 
                                           "Custom Window", WS_OVERLAPPEDWINDOW|WS_VISIBLE, 
                                           CW_USEDEFAULT, CW_USEDEFAULT, 
                                           CW_USEDEFAULT, CW_USEDEFAULT, 
                                           0, 0, instance, 0);
        if (windowHandle)
        {
            keepRunning = 1;
            // TODO(jarek): stand in
            for (;keepRunning;) 
            {
                MSG message;
                GetMessage(&message, 0, 0, 0);
                DispatchMessage(&message);
                if (keepRunning == 0)
                {
                    exit(1);
                }
            }
        }
        else
        {
            fprintf(stderr, "Create window failed!\n");
        }
    }
    else
    {
        fprintf(stderr, "Register window class failed!\n");
    }
    return 0;
}
//end of program