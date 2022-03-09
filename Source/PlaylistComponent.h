#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include <fstream>

class PlaylistComponent  : public juce::Component,
                        public juce::TableListBoxModel,
                        public juce::Button::Listener,
                        public juce::DragAndDropContainer,
                        public juce::TableHeaderComponent::Listener,
                        public juce::TextEditor::Listener
{
public:
    PlaylistComponent(DJAudioPlayer* _player1, DJAudioPlayer* _player2, DeckGUI* _deck1, DeckGUI* _deck2, DJAudioPlayer* _playlistPlayer);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized () override;
    int getNumRows () override;
    void paintRowBackground (juce::Graphics& g,
                        int rowNumber,
                        int width,
                        int height,
                        bool rowIsSelected) override;
    void paintCell (  juce::Graphics& g,
                    int rowNumber,
                    int columnID,
                    int Width,
                    int height,
                    bool rowIsSelected) override;
    
    juce::Component* refreshComponentForCell (int rowNumber, int columnID, bool isRowDelected, Component *existingComponentToUpdate) override;
    void buttonClicked(juce::Button* button) override;
    void tableColumnsChanged (juce::TableHeaderComponent *tableHeader) override;
    void tableColumnsResized (juce::TableHeaderComponent *tableHeader) override;
    void tableSortOrderChanged (juce::TableHeaderComponent *tableHeader) override;
    
    /**called when return is pressed*/
    void textEditorReturnKeyPressed (juce::TextEditor &) override;
private:
    void deleteSingleTrackFromPlaylist(unsigned int row);
    /**gets the last char in the id string, converts it to an int value and returns*/
    int parseCol(juce::String buttonID);
    /**Drops the final char of the string and converst the rmaining into an int id and returns*/
    int parseRow(juce::String buttonID);
    void setTracks(juce::Array<juce::File> tracksFile);
    /** called on initialization, creates a playlist from the csv created by save Playlist*/
    void loadPlaylist();
    /**Clear the vectors holding the tracks to clear the playlist*/
    void clearPlaylistOfAllTracks();
    /** save playlist upon close or clicking the save button*/
    void savePlaylist();
    /**will load tracks into playlist when load button is clicked using juce file chooser*/
    void loadIntoPlaylist();
    /** send an audio URL to the playlist player and get the lenfth in seconds
     then converts the seconds into a minutes value and returns this as a juce::String*/
    juce::String getLengthInMinutes(juce::URL audioURL);
    
    //struct to hold each track for the playlist
    struct playlistTrack
    {
        juce::String title;
        juce::URL trackURL;
        juce::String length;
        juce::String file;
        bool searched;
    };
    
    
    juce::TableListBox tableComponent;
    //input elements for the playlist
    juce::TextEditor searchBox;
    juce::TextButton loadToPlaylist;
    juce::TextButton clearPlaylist;
    juce::TextButton savePlaylistButton;
    
    // vector of structs to hold the songs in the playlist
    // Will be saved when closing the application to a csv file
    std::vector<playlistTrack> playlistTracks;
    
    //audio players
    DJAudioPlayer* player1;
    DJAudioPlayer* player2;
    
    //GUIs
    DeckGUI* deck1;
    DeckGUI* deck2;
    
    // create instance of an ausio plkayer to parse meta data so not to intefer with audio playback on either deck
    DJAudioPlayer* playlistPlayer;
    
    // hold the tracks while being loaded
    juce::Array<juce::File> file;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
