/*
  ==============================================================================

    Turntable.h
    Created: 8 Mar 2022 2:45:41pm
    Author:  Nathan Norton

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Turntable  : public juce::Component
{
public:
    Turntable();
    ~Turntable() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Turntable)
};
