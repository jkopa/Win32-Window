#include <windows.h>

LRESULT CALLBACK
MainWindowCallback(HWND window, UINT message, WPARAM WParam, LPARAM LParam)
{
    LRESULT result = 0;

// TODO(jarek): WIP case statements to handle messages from windows
    switch(message)
    {
        case WM_CREATE:
        {
            OutputDebugStringA("WM_CREATE\n");
        } break;
        case WM_SIZE:
        {
            OutputDebugStringA("WM_SIZE\n");
        } break;
        case WM_DESTROY:
        {
            OutputDebugStringA("WM_DESTROY\n");
        } break;
        case WM_CLOSE:
        {
            OutputDebugStringA("WM_CLOSE\n");
        } break;
        case WM_ACTIVATEAPP:
        {
            OutputDebugStringA("Wm_ACTIVATEAPP\n");
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
WinMain(HINSTANCE Instance, HINSTANCE prevInstance, LPSTR commandLine, int showCode)
{
    WNDCLASS windowClass = {}; 
    
    // TODO(jarek): Double check what flags are actually necessary 
    windowClass.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
    windowClass.lpfnWndProc = MainWindowCallback;
    windowClass.hInstance = Instance; 
    //windowClass.hIcon; //will add icon later on, leave commented for the time being
    windowClass.lpszClassName = "CustomWindowClass";

    if (RegisterClass(&windowClass))
    {
        HWND windowHandle = CreateWindowEx(
            0,
            windowClass.lpszClassName,
            "CustomWindow",
            WS_OVERLAPPEDWINDOW|WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            Instance,
            0);
        if (windowHandle)
        {
            MSG message;
            // TODO(jarek): stand in
            for (;;) 
            {
                BOOL messageResult = GetMessage(&message, 0, 0, 0);
                if (messageResult > 0)
                {

                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            // TODO(jarek): log error!
        }
    }
    else
    {
        // TODO(jarek): log error!
    }

    return 0;
}