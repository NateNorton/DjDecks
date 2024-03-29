#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"

class WaveformDisplay : public juce::Component,
                    public juce::ChangeListener
{
public:
    WaveformDisplay(DJAudioPlayer* _player,
                    juce::AudioFormatManager& formatManagerToUse,
                    juce::AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay();
    void paint (juce::Graphics&) override;
    /**Called when windows size is changed*/
    void resized() override;
    /**implemented to receive the callback*/
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    /** called when a mouse button is pressed */
    void mouseDown(const juce::MouseEvent& event) override;
    
    void loadURL(juce::URL audioURL);
    /**receives position from DJAudioPlayer */
    void setPositionRelative(double pos);
private:
    juce::AudioThumbnail audioThumbnail;
    bool fileLoaded;
    double position;
    DJAudioPlayer* player;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
    
};
