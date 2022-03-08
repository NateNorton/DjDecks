/*
  ==============================================================================

    Style.h
    Created: 22 Feb 2022 3:28:31pm
    Author:  Nathan Norton

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    //rotary slider
    void drawRotarySlider (juce::Graphics &, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider &) override;
    //juce::Label* createSliderTextBox (juce::Slider &) override;
    
    //text button
    void drawButtonText (juce::Graphics &, juce::TextButton &, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    void drawButtonBackground (juce::Graphics &, juce::Button &, const juce::Colour &backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
};
