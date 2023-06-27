#include "view.h"

View::View() {
    this->yg = YGNodeNew();
    YGNodeStyleSetWidthAuto(yg);
    YGNodeStyleSetHeightAuto(yg);
    this->set_flex_direction(YGFlexDirectionRow);
}

void View::add_child(View & child) {
    YGNodeInsertChild(yg, child.get_node(), 0);
}

void View::update_node() {
    YGNodeCalculateLayout(yg, YGUndefined, YGUndefined, YGDirectionLTR);
}

void View::set_padding(YGEdge ed, int x) {
    YGNodeStyleSetPadding(yg, ed, x);

}

void View::set_margin(YGEdge ed, int x) {
    YGNodeStyleSetMargin(yg, ed, x);

}

void View::set_flex_direction(YGFlexDirection fd) {
    YGNodeStyleSetFlexDirection(yg, fd);
}

void View::set_x(int x) {
    YGNodeStyleSetPosition(yg, YGEdgeTop, x);
}

void View::set_y(int y) {
    YGNodeStyleSetPosition(yg, YGEdgeLeft, y);
}

void View::set_width(int w) {
    YGNodeStyleSetWidth(yg, w);
}

void View::set_height(int h) {
    YGNodeStyleSetHeight(yg, h);
}


const YGNodeRef
View::get_node() {
    return yg;
}

int
View::get_x() {
    return YGNodeLayoutGetLeft(yg);
}

int
View::get_y() {
    return YGNodeLayoutGetTop(yg);
}

int
View::get_width() {
    return YGNodeLayoutGetWidth(yg);
}

int
View::get_height() {
    return YGNodeLayoutGetHeight(yg);
}
