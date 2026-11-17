#include "color.h"

Color hueToRgb(int hue)
{
    int region = hue / 60;
    int rise = (hue % 60) * 255 / 60;
    int fall = 255 - rise;

    switch (region)
    {
    case 0:
        return {255, rise, 0};
    case 1:
        return {fall, 255, 0};
    case 2:
        return {0, 255, rise};
    case 3:
        return {0, fall, 255};
    case 4:
        return {rise, 0, 255};
    default:
        return {255, 0, fall};
    }
}
