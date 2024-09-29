#ifndef GAPPLICATION_HPP
#define GAPPLICATION_HPP

#include <GApplicationDelegate/GApplicationDelegate.hpp>

#include <string>

class GApplication
{
    public:

        std::string Title;
        std::string Description;

        int MajorVersion;
        int MinorVersion;
        int SubVersion;

        GApplicationDelegate* Delegate;

        int Run();
};


#endif // GAPPLICATION_HPP