#include <GApplication/GApplication.hpp>

#include <GLog/GLog.hpp>

#ifdef _WIN32
    #include <Windows.h>
#elif __linux__
    #include <X11/Xlib.h>
    #include <GWindow/X11/X11Window.hpp>
#endif

int GApplication::Run()
{

    GLog::Info("GSPCore Framework 0.1");
    GLog::Info("Starting: %s %d.%d.%d", Title.c_str(), MajorVersion, MinorVersion, SubVersion);

    if (Delegate == nullptr)
    {
        GLog::Error("GApplication Delegate not set");
        return EXIT_FAILURE;
    }

    Delegate->ApplicationDidLaunch();

    #ifdef _WIN32
        bool shouldQuit = false;
        MSG msg;

        while (!shouldQuit)
        {
            while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
            {

                if (msg.message == WM_QUIT || msg.message == WM_CLOSE || (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE)) 
                {
                    printf("CALLING SHOULD TERMINATE\n");
                    if (Delegate->ApplicationShouldTerminate()) 
                    {
                        shouldQuit = true;
                    }
                }
                else 
                {
                    TranslateMessage(&msg);
                    DispatchMessageW(&msg);
                }
                
            }
        }
        
        Delegate->ApplicationWillTerminate();
    #elif __linux__

        return X11Window::RunXLoop();
    #endif

    return EXIT_SUCCESS;
}