#pragma once

#include <JuceHeader.h>
#include "style.h"

class CustomSlider : public Slider
{
public:
    struct Range
    {
        float start;
        float stop;
        float step;
    };

    CustomSlider(const String& name,
                 const Range& range,
                 Slider::Listener* listener,
                 Slider::SliderStyle style,
                 float initValue);
private:
    Style m_style;
};
