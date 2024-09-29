#ifndef GWINDOW_HPP
#define GWINDOW_HPP

#ifdef _WIN32
    #include <Windows.h>
#endif

class GWindow
{
    private:

        #ifdef _WIN32
            HWND hwnd;
        #endif

        static bool Init();
        static void Poll();


    public:

        GWindow(const char* title, float width, float height);
        
    friend class GSPCore;
};


#endif // GWINDOW_HPP