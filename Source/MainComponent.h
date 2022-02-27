#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
#include "WaveformDisplay.h"
#include "Style.h"

class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    bool playing;
    
    //deck 1
    DJAudioPlayer player1{formatManager};
    DeckGUI deck1{&player1, formatManager, thumbnailCache};
    
    // deck 2
    DJAudioPlayer player2{formatManager};
    DeckGUI deck2{&player2, formatManager, thumbnailCache};
    
    //player for playlist component to get meta data without intefering with the audio playback
    DJAudioPlayer playlistPlayer{formatManager};
    
    //playlist
    PlaylistComponent playlistComponent{&player1, &player2, &deck1, &deck2, &playlistPlayer};
    
    juce::MixerAudioSource mixerSource;
    
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbnailCache{50};
    
    //include custom look and feel
    CustomLookAndFeel customLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
