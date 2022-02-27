/*
  ==============================================================================

    PlaylistComponent.h
    Created: 17 Feb 2022 2:03:09pm
    Author:  Nathan Norton

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include <fstream>

//struct to hold each track for the playlist
struct playlistTrack
{
    juce::String title;
    juce::URL trackURL;
    juce::String length;
};

class PlaylistComponent  : public juce::Component,
                        public juce::TableListBoxModel,
                        public juce::Button::Listener,
                        public juce::DragAndDropContainer
{
public:
    PlaylistComponent(DJAudioPlayer* _player1, DJAudioPlayer* _player2, DeckGUI* _deck1, DeckGUI* _deck2, DJAudioPlayer* _playlistPlayer);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized () override;
    
    int getNumRows () override;
    void paintRowBackground (
                             juce::Graphics& g,
                             int rowNumber,
                             int width,
                             int height,
                             bool rowIsSelected) override;
    void paintCell (
                    juce::Graphics& g,
                    int rowNumber,
                    int columnID,
                    int Width,
                    int height,
                    bool rowIsSelected) override;
    
    juce::Component* refreshComponentForCell (
                                        int rowNumber,
                                        int columnID,
                                        bool isRowDelected,
                                        Component *existingComponentToUpdate) override;
    
    void cellClicked (int rowNumber, int columnID, const juce::MouseEvent&) override;
    
    void buttonClicked(juce::Button* button) override;
    
    void setTracks(juce::Array<juce::File> tracksFile);
    
    /**will load tracks into playlist when load button is clicked using juce file chooser*/
    void loadIntoPlaylist();
    
    /**Clear the vectors holding the tracks to clear the playlist*/
    void clearPlaylistOfAllTracks();
    


private:
    /** send an audio URL to the playlist player and get the lenfth in seconds
     then converts the seconds into a minutes value and returns this as a juce::String*/
    juce::String getLengthInMinutes(juce::URL audioURL);
    
    
    juce::TableListBox tableComponent;
    
    //inputs for the playlist- load button, clear button and search box
    juce::TextEditor searchBox;
    juce::TextButton loadToPlaylist;
    juce::TextButton clearPlaylist;
    
    
    // vector of structs to hold the songs in the playlist
    // Will be saved when closing the application to a csv file
    std::vector<playlistTrack> playlistTracks;
    
    DJAudioPlayer* player1;
    DJAudioPlayer* player2;
    DeckGUI* deck1;
    DeckGUI* deck2;
    
    // create instance of an ausio plkayer to parse meta data so not to intefer with audio playback on either deck
    DJAudioPlayer* playlistPlayer;
    
    // hold the tracks while being loaded
    juce::Array<juce::File> file;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
