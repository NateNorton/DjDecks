#pragma once

#include <JuceHeader.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include <string>
#include "LevelMeter.h"

//==============================================================================
/*
 */
class DeckGUI  : public juce::Component,
public juce::Button::Listener,
public juce::Slider::Listener,
public juce::FileDragAndDropTarget,
public juce::Timer,
public juce::DragAndDropTarget
{
public:
    DeckGUI(DJAudioPlayer* _djAudioPlayer,
            juce::AudioFormatManager& formatManagerToUse,
            juce::AudioThumbnailCache& cacheToUse);
    ~DeckGUI() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged (juce::Slider *slider) override;
    /**load in the waveform from chosen track when played from the playlist component*/
    void loadWaveformFromPlaylist(juce::URL audioURL);
    /** receives track title from playlist and displays the playing track*/
    void showTrackTitle(juce::String trackTitle);
    
    //drag and drop
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;
    bool isInterestedInDragSource(const SourceDetails &dragSourceDetails) override;
    void itemDropped(const SourceDetails &dragSourceDetails) override;
    void timerCallback() override;
    
private:
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::TextButton loadButton;
    juce::Slider volumeSlider;
    juce::Slider positionSlider;
    juce::Label positionLabel;
    juce::Slider speedSlider;
    juce::Label speedLabel;
    
    LevelMeter levelMeterL;
    LevelMeter levelMeterR;
    
    juce::FileChooser chooser{"Browse audio file"};
      
    juce::Label trackTitle;
  
    //audio player
    DJAudioPlayer* player;
    //waveform component
    WaveformDisplay waveformDisplay;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
