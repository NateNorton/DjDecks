#include "WaveformDisplay.h"

WaveformDisplay::WaveformDisplay(DJAudioPlayer* _player,
                             juce::AudioFormatManager& formatManagerToUse,
                             juce::AudioThumbnailCache& cacheToUse)
: audioThumbnail(1000, formatManagerToUse, cacheToUse),
fileLoaded(false),
position(0),
player(_player)
{
    audioThumbnail.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay() {
    
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour{ 162, 213, 198 });
    g.setColour (juce::Colour{ 36, 24, 57 });
    g.drawRect(getLocalBounds(), 1);
    g.setColour (juce::Colour{ 36, 24, 57 });
    if(fileLoaded)
    {
        audioThumbnail.drawChannel(g, getLocalBounds(), 0, audioThumbnail.getTotalLength(), 0, 1);
        g.setColour(juce::Colour{ 7, 123, 138 });
        g.drawRect(position * getWidth(), 0, getWidth() / 50, getHeight());
    }
    else
    {
        g.setFont(24.0f);
        g.drawText("File not loaded...", getLocalBounds(), juce::Justification::centred, true);
    }
}

void WaveformDisplay::resized()
{
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    //repaint the waveform display whenever it processes new data
    repaint();
}

void WaveformDisplay::loadURL(juce::URL audioURL)
{
    audioThumbnail.clear();
    fileLoaded = audioThumbnail.setSource(new juce::URLInputSource(audioURL));
}

void WaveformDisplay::mouseDown(const juce::MouseEvent& event)
{
    //divide mouse x by the waveforms width to get a relative position
    //then send the relative position to the audio player to change the position
    double mouseX = event.getMouseDownX();
    double componentW = getWidth();
    double relativePosition =  mouseX / componentW;
    player->setPositionRelative(relativePosition);
}

void WaveformDisplay::setPositionRelative(double pos)
{
    if(pos != position && pos > 0)
    {
        position = pos;
        repaint();
    }
}
