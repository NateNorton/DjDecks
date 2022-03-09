#pragma once

#include <JuceHeader.h>

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    //rotary slider
    void drawRotarySlider (juce::Graphics &, int x, int y,
                           int width, int height,
                           float sliderPosProportional,
                           float rotaryStartAngle, float rotaryEndAngle,
                           juce::Slider &) override;
    
    // volume slider
    void drawLinearSlider (juce::Graphics &, int x, int y,
                           int width, int height,
                           float sliderPos, float minSliderPos,
                           float maxSliderPos,
                           const juce::Slider::SliderStyle,
                           juce::Slider &) override;
    
    //text button
    void drawButtonText (juce::Graphics &, juce::TextButton &, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    void drawButtonBackground (juce::Graphics &, juce::Button &, const juce::Colour &backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
};
