// inspiration for levelMeter taken from https://www.youtube.com/watch?v=ILMdPjFQ9ps&ab_channel=AkashMurthy

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class LevelMeter  : public juce::Component,
                    public juce::Timer
{
public:
    LevelMeter(DJAudioPlayer* _player, int _channel);
    ~LevelMeter() override;
    
    void paint (juce::Graphics&) override;
    /**Called when this component's size has been changed.*/
    void resized() override;
    /**The user-defined callback routine that gets called periodically. */
    void timerCallback() override;
private:
    DJAudioPlayer* player;
    int channel;
    float level;
    juce::ColourGradient gradient;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeter)
};
