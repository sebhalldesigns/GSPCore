#ifndef GAPPLICATIONDELEGATE_HPP
#define GAPPLICATIONDELEGATE_HPP

class GApplicationDelegate
{
    public:
        GApplicationDelegate() = default;
        virtual ~GApplicationDelegate() = default;

        virtual void ApplicationDidLaunch() = 0;
        virtual bool ApplicationShouldTerminate() = 0;
        virtual void ApplicationWillTerminate() = 0;
};


#endif // GAPPLICATIONDELEGATE_HPP

