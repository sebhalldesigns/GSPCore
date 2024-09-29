#include <GApplication/GApplication.hpp>

#include <GLog/GLog.hpp>

#ifdef _WIN32
    #include <Windows.h>
#endif

int GApplication::Run()
{

    GLog::Info("GSPCore Framework 0.1");
    GLog::Info("Starting: %s %d.%d.%d", Title.c_str(), MajorVersion, MinorVersion, SubVersion);



    DidLaunch();

    #ifdef _WIN32
        bool shouldQuit = false;
        MSG msg;

        while (!shouldQuit)
        {
            while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
            {

                if (msg.message == WM_QUIT || msg.message == WM_CLOSE) 
                {
                    printf("CALLING SHOULD TERMINATE\n");
                    if (ShouldTerminate()) 
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
        

        WillTerminate();
        
    #endif

    return EXIT_SUCCESS;
}