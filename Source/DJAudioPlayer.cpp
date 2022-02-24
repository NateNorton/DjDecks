/*
 ==============================================================================
 
 DJAudioPlayer.cpp
 Created: 3 Feb 2022 12:29:52pm
 Author:  Nathan Norton
 
 ==============================================================================
 */

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager)
: formatManager(_formatManager)
{
}

DJAudioPlayer::~DJAudioPlayer()
{
}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    transportSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) {
    if(readerSource.get() == nullptr) {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    //transportSource.getNextAudioBlock (bufferToFill);
    resampleSource.getNextAudioBlock(bufferToFill);
    
    //get the rms values for each channel and convert to decibels
    rmsLevelLeft = juce::Decibels::gainToDecibels(bufferToFill.buffer->getRMSLevel(0, 0, bufferToFill.buffer->getNumSamples()));
    rmsLevelRight = juce::Decibels::gainToDecibels(bufferToFill.buffer->getRMSLevel(1, 0, bufferToFill.buffer->getNumSamples()));
}

void DJAudioPlayer::releaseResources() {
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(juce::URL audioURL) {
    auto *reader = this->formatManager.createReaderFor(audioURL.createInputStream(juce::URL::InputStreamOptions{juce::URL::ParameterHandling::inAddress}));
    
    //if good file
    if(reader != nullptr) {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader -> sampleRate);
        readerSource.reset(newSource.release());
    }
}

void DJAudioPlayer::play() {
    transportSource.start();
}

void DJAudioPlayer::stop() {
    transportSource.stop();
}

void DJAudioPlayer::setPosition(double posInSecs) {
    if(posInSecs < 0 || posInSecs > transportSource.getLengthInSeconds()) {
        DBG("WARNING: set p[osition " << posInSecs << " greater than length " << transportSource.getLengthInSeconds());
        return;
    }
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setGain(double gain) {
    transportSource.setGain(gain);
}

float DJAudioPlayer::getRmsValue(const int channel) const
{
    if (channel == 0)
    {
        return rmsLevelLeft;
    }
    if (channel == 1) {
        return rmsLevelRight;
    }
    return 0.f;
}

//receives a value 0-1
void DJAudioPlayer::setPositionRelative(double pos) {
    if (pos >= 0 && pos <= 1) {
        double posInSecs = pos * transportSource.getLengthInSeconds();
        setPosition(posInSecs);
    }
}

void DJAudioPlayer::setSpeed(double ratio) {
    if(ratio < 0 || ratio > 100.0) {
        DBG("Speed ratio must be between 0 and 100");
    } else {
        resampleSource.setResamplingRatio(ratio);
    }
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

void DJAudioPlayer::playFromPlaylist(juce::URL audioURL)
{
    loadURL(audioURL);
    play();
}

