#ifndef view_h
#define view_h
#include "yoga/Yoga.h"
class View {
    protected:
        YGNodeRef yg;
    public:
        int get_x();
        int get_y();
        int get_width();
        int get_height();
        void set_x(int x);
        void set_y(int y);
        void set_width(int w);
        void set_height(int h);
        void set_flex_direction(YGFlexDirection);
        void set_padding(YGEdge, int);
        void set_margin(YGEdge, int);
        void add_child(View & child);
        void update_node();
        const YGNodeRef get_node();
        View();
};
#endif
