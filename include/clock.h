#ifndef clock_h
#define clock_h
#include "view.h"
#include "nanovg.h"
class Clock : public View {
private:
    int hours, minutes;

    NVGcontext *vg;
    public:
    Clock(NVGcontext *ctx, float scale);
    void update_time();
    float get_radius();
    void draw();
    int get_x();
    int get_y();
};
#endif
