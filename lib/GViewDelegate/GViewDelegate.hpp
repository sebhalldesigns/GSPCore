#ifndef GVIEWDELEGATE_HPP
#define GVIEWDELEGATE_HPP

class GViewDelegate
{
    public:
        GViewDelegate() = default;
        virtual ~GViewDelegate() = default;

        virtual void ViewDidLoad() = 0;
};


#endif // GVIEWDELEGATE_HPP

