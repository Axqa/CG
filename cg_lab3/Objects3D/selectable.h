#ifndef SELECTABLE_H
#define SELECTABLE_H

#include "../MathEngine/ncl.h"

class Selectable
{
public:
    Selectable();

    bool isSelected() {return selected;}
    virtual void setSelected(bool state) {selected = state;}

    /// if intersects, return true and set dist to distance from ray origin to intersection point
    virtual bool isIntersects(const Ray &ray, float &dist) = 0;

protected:
    bool selected;
};

#endif // SELECTABLE_H
