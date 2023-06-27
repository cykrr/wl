#include <chrono>
#include <cmath>
#include <ctime>
#include "clock.h"
#define HOURS 12
#define MINUTES 60
static int time_to_angle(int x, int max) {
        float norm = (float)x/max;
        float angle = norm*2*M_PI + 3*M_PI_2;
        return angle;
}

int Clock::get_x() {
    return View::get_x() + get_width() / 2;
}

int Clock::get_y() {
    return View::get_y() + get_height() / 2;
}

Clock::Clock(NVGcontext *ctx, float scale) {
    View();
    this->vg = ctx;
    this->set_width(100*scale);
    this->set_height(100*scale);
    this->set_margin(YGEdgeAll, 0);
}


void Clock::update_time() {
    std::chrono::time_point<std::chrono::system_clock> now =
        std::chrono::system_clock::now();

    // Convert the system time to a time_t object
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    struct std::tm* localTime = std::localtime(&currentTime);

    // Extract the hours and minutes from the local time struct
    this->hours = localTime->tm_hour;
    this->minutes = localTime->tm_min;
}


float
Clock::get_radius() {
    return YGNodeLayoutGetWidth(yg)/2;
}


void Clock::draw() {
        nvgStrokeWidth(vg, 3.0f);
        nvgStrokeColor(vg, nvgRGBA(254,255,255,254));
        nvgBeginPath(vg);
        nvgCircle(vg, get_x(), get_y(), get_radius());
        nvgStroke(vg);

        nvgFillColor(vg, nvgRGBA(0,0,0, 100));
        nvgBeginPath(vg);
        nvgCircle(vg, get_x(), get_y(), get_radius());
        nvgFill(vg);

        nvgBeginPath(vg);
        nvgMoveTo(vg,
                get_x() + cos(time_to_angle(hours, HOURS))*0.5*get_radius(),
                get_y() + sin(time_to_angle(hours, HOURS))*0.5*get_radius()
                );

        nvgLineTo(vg, get_x(),get_y());
        nvgStroke(vg);

        nvgBeginPath(vg);
        nvgMoveTo(vg,
                get_x() + cos(time_to_angle(minutes, MINUTES))*0.8*get_radius(),
                get_y() + sin(time_to_angle(minutes, MINUTES))*0.8*get_radius()
                );

        nvgLineTo(vg, get_x(),get_y());
        nvgStroke(vg);
}
