/*
 ==============================================================================
 
 DeckGUI.cpp
 Created: 3 Feb 2022 4:15:40pm
 Author:  Nathan Norton
 
 ==============================================================================
 */

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 juce::AudioFormatManager& formatManagerToUse,
                 juce::AudioThumbnailCache& cacheToUse)
: levelMeterL(_player, 0),
levelMeterR(_player, 1),
player(_player),
waveformDisplay(_player, formatManagerToUse, cacheToUse)
{
    //playButton
    addAndMakeVisible(playButton);
    playButton.setButtonText("PLAY");
    playButton.addListener(this);
    
    //stop button
    addAndMakeVisible(stopButton);
    stopButton.setButtonText("STOP");
    stopButton.addListener(this);
    
    //load button
    addAndMakeVisible(loadButton);
    loadButton.setButtonText("LOAD");
    loadButton.addListener(this);
    
    //volume slider
    addAndMakeVisible(volumeSlider);
    volumeSlider.setRange(0.0f, 1.0f);
    volumeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    volumeSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    volumeSlider.addListener(this);
    
    //position slider
    addAndMakeVisible(positionSlider);
    positionSlider.setRange(0.0f, 1.0f);
    positionSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    positionSlider.addListener(this);
    positionLabel.setText("Track Position", juce::dontSendNotification);
    positionLabel.attachToComponent(&positionSlider, false);
    positionLabel.setJustificationType(juce::Justification::centred);
    positionSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    
    //speed slider
    addAndMakeVisible(speedSlider);
    speedSlider.setRange(0.01, 2);
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    speedSlider.addListener(this);
    speedLabel.setText("Track Speed", juce::dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, false);
    speedLabel.setJustificationType(juce::Justification::centred);
    speedSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    
    //track title
    addAndMakeVisible(trackTitle);
    trackTitle.setText("No track loaded", juce::NotificationType::dontSendNotification);
    trackTitle.setJustificationType(juce::Justification::centred);
    
    //waveform display
    addAndMakeVisible(waveformDisplay);
    waveformDisplay.addMouseListener(&waveformDisplay, true);
    
    //level meters
    addAndMakeVisible(levelMeterL);
    addAndMakeVisible(levelMeterR);
    
    //begin timer
    startTimer(100);
}

DeckGUI::~DeckGUI() {
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void DeckGUI::resized() {
    float rowH = getHeight() / 12;
    float rowW = getWidth() / 10;
    
    // title of song playing - whole width and 1 height
    trackTitle.setBounds(0, 0, getWidth(), rowH);
    //waveform display - whole width and 3 height
    waveformDisplay.setBounds(0, rowH, getWidth(), rowH * 3);
    //level meters
    levelMeterR.setBounds(rowW * 9, rowH * 4, rowW, rowH * 8);
    levelMeterL.setBounds(rowW * 8, rowH * 4, rowW, rowH * 8);
    //volume slider
    volumeSlider.setBounds(rowW * 7, rowH * 4, rowW, rowH * 8);
    //position slider
    positionSlider.setBounds(0, rowH * 9, rowW * 4, rowH * 3);
    //speed Slider
    speedSlider.setBounds(rowH * 4, rowH * 9, rowW * 4, rowH * 3);
    
    
    // TODO change position od these when iomplemented other things
    playButton.setBounds(0, rowH * 4, rowW * 3, rowH * 2);
    stopButton.setBounds(rowW * 4, rowH * 4, rowW * 3, rowH * 2);
    loadButton.setBounds(0,  rowH * 6, rowW * 7, rowH * 2);
}

void DeckGUI::buttonClicked(juce::Button* button) {
    if(button == &playButton) {
        player->setPosition(0);
        player->play();
    }
    
    if(button == &stopButton) {
        player->stop();
    }
    
    if(button == &loadButton) {
        auto dlgFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
        
        this -> chooser.launchAsync(dlgFlags, [this](const juce::FileChooser& chooser){
            auto audioURL = chooser.getURLResult();
            //send file url to player to be played
            player->loadURL(audioURL);
            //send file URL to waveformDisplay to be drawn
            waveformDisplay.loadURL(audioURL);
        });
    }
}

void DeckGUI::loadWaveformFromPlaylist(juce::URL audioURL)
{
    waveformDisplay.loadURL(audioURL);
}

void DeckGUI::sliderValueChanged(juce::Slider* slider) {
    if(slider == &volumeSlider) {
        player->setGain(slider->getValue());
    }
    
    if(slider == &positionSlider) {
        player ->setPositionRelative(slider->getValue());
    }
    
    if(slider == &speedSlider) {
        player->setSpeed(slider->getValue());
    }
}

void DeckGUI::showTrackTitle(juce::String track)
{
    trackTitle.setText(track, juce::NotificationType::dontSendNotification);
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files) {
    DBG("DeckGUI::isInterestedInFileDrag");
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y) {
    for(juce::String filename : files) {
        DBG("DeckGUI::filesDropped " << filename);
        juce::URL fileURL = juce::URL{juce::File{filename}};
        player->loadURL(fileURL);
        waveformDisplay.loadURL(fileURL);
        return;
    }
}

bool DeckGUI::isInterestedInDragSource(const SourceDetails &dragSourceDetails)
{
    return true;
}

void DeckGUI::itemDropped(const SourceDetails &dragSourceDetails)
{
    DBG("item dropped");
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}
