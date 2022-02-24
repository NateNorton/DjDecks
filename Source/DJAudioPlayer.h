/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 3 Feb 2022 12:29:52pm
    Author:  Nathan Norton

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public juce::AudioSource {
public:
    //constructor
    DJAudioPlayer(juce::AudioFormatManager& formatManager);
    //deconstructor
    ~DJAudioPlayer();
    //pure virtual functions
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    void loadURL(juce::URL file);
    /** start playing the file*/
    void play();
    /** stop playing the file**/
    void stop();
    /** set the position of the play head to skip forward and backward through the track**/
    void setPosition(double posInSecs);
    /** set the volume***/
    void setGain(double gain);
    /** converts passed 0-1 from gain slider into time appropriate for playing track**/
    void setPositionRelative(double pos);
    /** sets speed of track**/
    void setSpeed(double ratio);
    /** divedes transportSource current position by its length*/
    double getPositionRelative();
    /**immediately p[lay a song selected from the playlist*/
    void playFromPlaylist(juce::URL audioURL);
    /**gets the rms values*/
    float getRmsValue(const int channel) const;
    
private:
    //juce audio classes
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};
    
    // will hold the rms valus for the volume meter
    float rmsLevelLeft;
    float rmsLevelRight;
};
