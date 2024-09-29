#ifndef GAPPLICATION_HPP
#define GAPPLICATION_HPP

#include <string>

class GApplication
{
    protected:

        std::string Title;
        std::string Description;

        int MajorVersion;
        int MinorVersion;
        int SubVersion;
        
        virtual void DidLaunch() {};
        virtual bool ShouldTerminate() { return true; };
        virtual void WillTerminate() {};
    
    public:
        int Run();
};


#endif // GAPPLICATION_HPP