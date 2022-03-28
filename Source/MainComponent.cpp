#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    //use custom look and feel
    juce::LookAndFeel::setDefaultLookAndFeel(&customLookAndFeel);
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (1000, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    //Decks
    addAndMakeVisible(deck1);
    addAndMakeVisible(deck2);
    
    //playlist
    addAndMakeVisible(playlistComponent);
    
    //formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    mixerSource.removeAllInputs();
    mixerSource.releaseResources();
    player1.releaseResources();
    player2.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colour{ 36, 24, 57 });
}

void MainComponent::resized()
{
    float widthSection = getWidth() / 2;
    float heightSection = getHeight() / 6;
    
    //each deck to take half the width
    deck1.setBounds(0, 0, widthSection, heightSection * 4);
    deck2.setBounds(widthSection, 0, widthSection, heightSection * 4);
    
    
    //playlist component - takes whoel width and 2 sections high
    playlistComponent.setBounds(0, heightSection * 4, getWidth(), heightSection * 2);
}
