#ifndef GLOG_HPP
#define GLOG_HPP

class GLog 
{
    public:
        static void Info(const char* message, ...);
        static void Warning(const char* message, ...);
        static void Error(const char* message, ...);
};

#endif // GLOG_HPP