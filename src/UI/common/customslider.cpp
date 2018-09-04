#include "customslider.h"

CustomSlider::CustomSlider(const String &name,
                           const CustomSlider::Range &range,
                           Slider::Listener *listener,
                           Slider::SliderStyle style,
                           float initValue)
{
    setSliderStyle(style);
    setTextBoxStyle (Slider::TextBoxBelow, false, 0, 0);
    addListener(listener);
    setRange(range.start, range.stop, range.step);
    setValue(initValue);
    setLookAndFeel(&m_style);
    setName(name);
}
