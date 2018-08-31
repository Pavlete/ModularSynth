#pragma once

#include <JuceHeader.h>

namespace  measures
{
    static constexpr int top_band_size = 25;
    static constexpr int side_margin = 10;
    static constexpr int controls_margin = 10;
    static constexpr int module_radius = 10;
    static constexpr int connectors_width = 20;

    static constexpr int separation_bar_width = 2;

    static constexpr int rotary_width = 75;
    static constexpr int rotary_height = 75;
    static constexpr int remove_size = 10;

    static constexpr int basic_module_width = side_margin * 2 +
                                              connectors_width * 2 +
                                              controls_margin * 2;
    static constexpr int basic_module_height = top_band_size +
                                               side_margin * 2;
}

namespace colours
{
    static const Colour module_background = {0xd3, 0xd3, 0xd3};
    static const Colour font = {0x55, 0x55, 0x55};
    static const Colour remove_module = {0xFF, 0, 0};
    static const Colour slider = {0,120,0};

    static const Colour font_focus =  font.darker(0.5f);
    static const Colour slider_focus = slider.darker(0.2f);
}

class Style : public LookAndFeel_V2
{
public:
    void drawRotarySlider (Graphics& g, int x, int y, int width,
                           int height, float sliderPos,
                           const float rotaryStartAngle,
                           const float rotaryEndAngle, Slider& slider) override;
};
