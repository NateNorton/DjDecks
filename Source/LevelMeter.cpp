#include <JuceHeader.h>
#include "LevelMeter.h"

//==============================================================================
LevelMeter::LevelMeter(DJAudioPlayer* _player, int _channel)
: player(_player),
channel(_channel),
level(-60.f)
{
    //repain every 24 hz
    startTimerHz(24);
}

LevelMeter::~LevelMeter()
{
}

void LevelMeter::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    
    //set background colour
    g.setColour(juce::Colours::black);
    //create a filled rectangle taking up the whole component
    g.fillRect(bounds);
    
    g.setGradientFill(gradient);
    
    //scale the overlaying black rectangle depending on the level value recieved from audioPlayer;
    const auto scaledY = juce::jmap(level, -60.f, 6.f, 0.f, static_cast<float>(getHeight()));
    g.fillRect(bounds.removeFromBottom(scaledY));
}

void LevelMeter::resized()
{
    const auto bounds = getLocalBounds().toFloat();
    //create colour gradient going from green to red
    gradient = juce::ColourGradient
    {
        juce::Colours::green,
        bounds.getBottomLeft(),
        juce::Colours::red,
        bounds.getTopRight(),
        false
    };
    gradient.addColour(0.5, juce::Colours::yellow);
}

void LevelMeter::setLevel(const float value)
{
    
}

void LevelMeter::timerCallback()
{
    //set the level with the audio player sending the channel for right and left
    level = player->getRmsValue(channel);
    repaint();
}

