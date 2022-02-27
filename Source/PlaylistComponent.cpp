/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 17 Feb 2022 2:03:09pm
    Author:  Nathan Norton

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DJAudioPlayer* _player1,
                                DJAudioPlayer* _player2,
                                DeckGUI* _deck1,
                                DeckGUI* _deck2,
                                DJAudioPlayer* _playlistPlayer)
:   player1(_player1),
    player2(_player2),
    deck1(_deck1),
    deck2(_deck2),
    playlistPlayer(_playlistPlayer)
{
    //register as a tableListBoxModel
    tableComponent.setModel(this);
    
    addAndMakeVisible(tableComponent);
    
    //loadPlaylist Button
    addAndMakeVisible(loadToPlaylist);
    loadToPlaylist.setButtonText("Load to playlist");
    loadToPlaylist.addListener(this);
    
    //clear playlist button
    addAndMakeVisible(clearPlaylist);
    clearPlaylist.setButtonText("Clear playlist");
    clearPlaylist.addListener(this);
    
    
    
    //add columns to table
    tableComponent.getHeader().addColumn("Track title", 1, 400);
    tableComponent.getHeader().addColumn("Track length", 2, 200);
    tableComponent.getHeader().addColumn("", 3, 200);
    tableComponent.getHeader().addColumn("", 4, 200);
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    int heightSection = getHeight() / 5;
    int widthSection = getHeight() / 3;
    
    loadToPlaylist.setBounds(0, 0, widthSection, heightSection);
    clearPlaylist.setBounds(widthSection, 0, widthSection, heightSection);
    searchBox.setBounds(widthSection * 2, 0, widthSection, heightSection);
    tableComponent.setBounds(0, heightSection, getWidth(), getHeight() - heightSection);
}

int PlaylistComponent::getNumRows ()
{
    return playlistTracks.size();
}

void PlaylistComponent::paintRowBackground (
                         juce::Graphics& g,
                         int rowNumber,
                         int width,
                         int height,
                         bool rowIsSelected)
{
    //hilight selected row
    if(rowIsSelected)
    {
        g.fillAll(juce::Colours::orange);
    }
    else
    {
        g.fillAll(juce::Colours::darkgrey);
    }
}

void PlaylistComponent::paintCell (
                juce::Graphics& g,
                int rowNumber,
                int columnID,
                int Width,
                int height,
                bool rowIsSelected)
{
    //paint track titles
    if(columnID == 1)
    {
        g.drawText(playlistTracks[rowNumber].title, 2, 0, Width - 4, height, juce::Justification::centredLeft, true);
    }
    //paint track length
    if(columnID == 2)
    {
        g.drawText(playlistTracks[rowNumber].length, 2, 0, Width - 4, height, juce::Justification::centredLeft, true);
    }

}

juce::Component* PlaylistComponent::refreshComponentForCell(
                                                      int rowNumber,
                                                      int columnID,
                                                      bool isRowDelected,
                                                      Component *existingComponentToUpdate)
{
    if (columnID == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextButton* btn = new juce::TextButton("deck 1");
            btn->addListener(this);
            
            //give the button an id so we can recognize it in buttonClicked()
            juce::String id{std::to_string(rowNumber)};
            btn->setComponentID(id);
            
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

//use to see where we clicked (IMPORTANT for future)
void PlaylistComponent::cellClicked (int rowNumber, int columnID, const juce::MouseEvent&)
{
    std::cout << "row number: " << rowNumber << " columnID: " << columnID <<std::endl;
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    if (button == &loadToPlaylist)
    {
        loadIntoPlaylist();
    }
    else if(button == &clearPlaylist)
    {
        clearPlaylistOfAllTracks();
    }
    else
    {
        int id = std::stoi(button->getComponentID().toStdString());
        player1->playFromPlaylist(playlistTracks[id].trackURL);
        deck1->loadWaveformFromPlaylist(playlistTracks[id].trackURL);
        deck1->showTrackTitle(playlistTracks[id].title);
    }
}

void PlaylistComponent::setTracks(juce::Array<juce::File> tracksFile)
{
    for( int i = 0; i < tracksFile.size(); ++i)
    {
        juce::String title = tracksFile[i].getFileName();
        juce::URL trackURL = juce::URL(tracksFile[i]);
        juce::String length = getLengthInMinutes(trackURL);
        DBG(length);
        
        playlistTrack track;
        track.title = title;
        track.trackURL = trackURL;
        track.length = length;
        
        playlistTracks.push_back(track);
    }
    tableComponent.updateContent();
}

void PlaylistComponent::loadIntoPlaylist()
{
    juce::FileChooser chooser("Select audio files", juce::File::getSpecialLocation(juce::File::userHomeDirectory));

    if(chooser.browseForMultipleFilesToOpen())
    {
        file = chooser.getResults();
        this->setTracks(file);
    }
}

void PlaylistComponent::clearPlaylistOfAllTracks()
{
    playlistTracks.clear();
    tableComponent.updateContent();
}

void savePlaylist()
{
    std::ofstream playlistFile("playlist.csv");
}

juce::String PlaylistComponent::getLengthInMinutes(juce::URL audioURL)
{
    playlistPlayer->loadURL(audioURL);
    double lengthInSeconds = playlistPlayer->getLengthInSeconds();
 
    int seconds = std::round(lengthInSeconds);
    juce::String minutesString = std::to_string(seconds / 60);
    juce::String secondsString = std::to_string(seconds % 60);
    
    juce::String minutesAsString = minutesString + ":" + secondsString;
    
    return minutesAsString;
}


