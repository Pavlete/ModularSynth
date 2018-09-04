#include "style.h"


void Style::drawRotarySlider(Graphics &g, int x, int y, int width,
                             int height, float sliderPos, const float rotaryStartAngle,
                             const float rotaryEndAngle, Slider &slider)
{
    auto fill = slider.isMouseOverOrDragging()? colours::slider_focus : colours::slider;
    auto font = slider.isMouseOverOrDragging()? colours::font_focus : colours::font;

    auto bounds = Rectangle<int> (x, y, width, height).withHeight(height*0.80);
    auto textBounds = Rectangle<int> (x, y, width, height).removeFromBottom(height*0.25);


    auto radius = jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = jmin (12.0f, radius * 0.5f);
    auto arcRadius = radius - lineW * 0.5f;

    Path backgroundArc;
    backgroundArc.addCentredArc (bounds.getCentreX(),
                                 bounds.getCentreY(),
                                 arcRadius,
                                 arcRadius,
                                 0.0f,
                                 rotaryStartAngle,
                                 rotaryEndAngle,
                                 true);

    g.setColour (fill);
    g.strokePath (backgroundArc, PathStrokeType (lineW, PathStrokeType::curved, PathStrokeType::rounded));


    g.setColour (colours::module_background);
    g.strokePath (backgroundArc, PathStrokeType (lineW - 4, PathStrokeType::curved, PathStrokeType::rounded));

    if (slider.isEnabled())
    {
        Path valueArc;
        valueArc.addCentredArc (bounds.getCentreX(),
                                bounds.getCentreY(),
                                arcRadius,
                                arcRadius,
                                0.0f,
                                rotaryStartAngle,
                                toAngle,
                                true);

        g.setColour (fill);
        g.strokePath (valueArc, PathStrokeType (lineW - 6, PathStrokeType::curved, PathStrokeType::rounded));
    }


    g.setColour (font);
    g.setFont(Font(18,1));

    g.drawText(String(slider.getValue()),bounds,juce::Justification::centred);
    g.drawText(slider.getName(), textBounds, juce::Justification::centred);
}
