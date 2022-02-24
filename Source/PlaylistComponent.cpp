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
PlaylistComponent::PlaylistComponent(DJAudioPlayer* _player1, DJAudioPlayer* _player2, DeckGUI* _deck1, DeckGUI* _deck2)
:   player1(_player1),
    player2(_player2),
    deck1(_deck1),
    deck2(_deck2)
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
    tableComponent.getHeader().addColumn("", 2, 200);
    tableComponent.getHeader().addColumn("", 3, 200);
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
    return trackTitles.size();
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
    g.drawText(
               trackTitles[rowNumber],
               2, 0,
               Width - 4,
               height,
               juce::Justification::centredLeft, true);
}

juce::Component* PlaylistComponent::refreshComponentForCell(
                                                      int rowNumber,
                                                      int columnID,
                                                      bool isRowDelected,
                                                      Component *existingComponentToUpdate)
{
    if (columnID == 2)
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
        player1->playFromPlaylist(trackURLs[id]);
        deck1->loadWaveformFromPlaylist(trackURLs[id]);
        deck1->showTrackTitle(trackTitles[id]);
    }
}

//juce::var PlaylistComponent::getDragSourceDescription (const juce::SparseSet<int>& selectedRows)
//{
//    int rowIndex = selectedRows[0];
//    return trackURLs[rowIndex];
//}

void PlaylistComponent::setTracks(juce::Array<juce::File> tracksFile)
{
    for( int i = 0; i < tracksFile.size(); ++i)
    {
        trackTitles.push_back(tracksFile[i].getFileName());
        juce::URL trackURL = juce::URL(tracksFile[i]);
        trackURLs.push_back(trackURL);
        //trackFiles.push_back(tracksFile[0]);
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
    trackTitles.clear();
    trackURLs.clear();
    tableComponent.updateContent();
}


