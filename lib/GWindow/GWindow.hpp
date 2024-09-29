#ifndef GWINDOW_HPP
#define GWINDOW_HPP

#ifdef _WIN32
    #include <GWindow/Win32/Win32GWindow.hpp>
    #define GNativeWindow Win32GWindow
#endif

class GWindow
{
    private:

        GNativeWindow window;

    public:

        GWindow(const char* title, float width, float height);
        static void Poll();
        
    friend class GSPCore;
};


#endif // GWINDOW_HPP