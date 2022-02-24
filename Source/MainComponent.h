#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
//#include "SharedGUI.h"
#include "WaveformDisplay.h"
#include "Style.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
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
    
    //shared gui
    //SharedGUI sharedGUI{&playlistComponent};
    
    //playlist
    PlaylistComponent playlistComponent{&player1, &player2, &deck1, &deck2};
    
    juce::MixerAudioSource mixerSource;
    
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbnailCache{50};
    
    //include custom look and feel
    CustomLookAndFeel customLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
