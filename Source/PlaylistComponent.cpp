#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include <climits>

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
    
    //save playilst
    addAndMakeVisible(savePlaylistButton);
    savePlaylistButton.setButtonText("Save  playlist");
    savePlaylistButton.addListener(this);
    
    //SEARCH BOX
    addAndMakeVisible(searchBox);
    searchBox.setMultiLine(false);
    searchBox.addListener(this);
    
    
    
    //add columns to table
    tableComponent.getHeader().addColumn("Track title", 1, 400);
    tableComponent.getHeader().addColumn("Track length", 2, 200);
    tableComponent.getHeader().addColumn("", 3, 100); //left deck
    tableComponent.getHeader().addColumn("", 4, 100); // rigth deck
    tableComponent.getHeader().addColumn("", 5, 100); // column for delete button
    
    loadPlaylist();
}

PlaylistComponent::~PlaylistComponent()
{
    savePlaylist();
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour{ 36, 24, 57 });   // clear the background
    
    g.setColour (juce::Colour{ 7, 123, 138 });
    g.drawRect (getLocalBounds(), 3);   // draw an outline around the component
    
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
    savePlaylistButton.setBounds(widthSection * 2, 0, widthSection, heightSection);
    searchBox.setBounds(widthSection * 3, 0, widthSection * 3, heightSection);
    tableComponent.setBounds(0, heightSection, getWidth(), getHeight() - heightSection);
}

int PlaylistComponent::getNumRows ()
{
    return playlistTracks.size() & INT_MAX;
}

void PlaylistComponent::paintRowBackground (juce::Graphics& g,
                                    int rowNumber,
                                    int width,
                                    int height,
                                    bool rowIsSelected)
{
    //hilight selected row
    if(rowIsSelected)
    {
        g.fillAll(juce::Colour{ 7, 123, 138 });
    }
    else
    {
        if(playlistTracks[rowNumber].searched == true)
        {
            g.fillAll(juce::Colour{ 255, 0, 0});
        }
        else
        {
            g.fillAll(juce::Colour{ 162, 213, 198 });
        }
    }
}

void PlaylistComponent::paintCell (juce::Graphics& g,
                            int rowNumber,
                            int columnID,
                            int Width,
                            int height,
                            bool rowIsSelected)
{
    //set the drawing colour
    if (rowIsSelected)
    {
        g.setColour(juce::Colour{ 162, 213, 198 });
    }
    else
    {
        g.setColour(juce::Colour{ 84, 15, 19 });
    }
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

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                                                    int columnID,
                                                    bool isRowDelected,
                                                    Component *existingComponentToUpdate)
{
    if (columnID == 3 || columnID == 4 || columnID == 5)
    {
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextButton* btn = new juce::TextButton("deck 1");
            // set button text
            switch (columnID) {
                case 3:
                    btn->setButtonText("Play Left");
                    break;
                case 4:
                    btn->setButtonText("Play right");
                    break;
                case 5:
                    btn->setButtonText("Delete");
                    break;
                default:
                    break;
            }
            btn->addListener(this);
            //give the button an id so we can recognize it in buttonClicked()
            //convert row and col coord into string value
            juce::String id{std::to_string(rowNumber) + std::to_string(columnID)};
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
    else if(button == &savePlaylistButton)
    {
        savePlaylist();
    }
    else
    {
    
        int row = parseRow(button->getComponentID());
        int col = parseCol(button->getComponentID());
        //play on left deck
        if(col == 3)
        {
            player1->playFromPlaylist(playlistTracks[row].trackURL);
            deck1->loadWaveformFromPlaylist(playlistTracks[row].trackURL);
            deck1->showTrackTitle(playlistTracks[row].title);
        }
        //play on right deck
        if(col == 4)
        {
            player2->playFromPlaylist(playlistTracks[row].trackURL);
            deck2->loadWaveformFromPlaylist(playlistTracks[row].trackURL);
            deck2->showTrackTitle(playlistTracks[row].title);
        }
        //delete track button
        if(col == 5)
        {
            deleteSingleTrackFromPlaylist(row);
        }
    }
}

//buttonID = row concat col
int PlaylistComponent::parseRow(juce::String buttonID)
{
    juce::String rowStr = buttonID.dropLastCharacters(1);
    return rowStr.getIntValue();
}

int PlaylistComponent::parseCol(juce::String buttonID)
{
    juce::String colStr = juce::String::charToString(buttonID.getLastCharacter());
    return colStr.getIntValue();
}

void PlaylistComponent::setTracks(juce::Array<juce::File> tracksFile)
{
    for( int i = 0; i < tracksFile.size(); ++i)
    {
        juce::String title = tracksFile[i].getFileName();
        juce::URL trackURL = juce::URL(tracksFile[i]);
        juce::String length = getLengthInMinutes(trackURL);
        
        playlistTrack track;
        track.title = title;
        track.trackURL = trackURL;
        track.length = length;
        track.file = tracksFile[i].getFullPathName();
        track.searched = false;
    
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

void PlaylistComponent::deleteSingleTrackFromPlaylist(unsigned int row)
{
    playlistTracks.erase(playlistTracks.begin() + row);
    
    tableComponent.updateContent();
}

void PlaylistComponent::savePlaylist()
{
    std::ofstream file("playlist.txt");
    
    if(! file.is_open())
    {
        DBG("error opening file");
    }
    else
    {
        for(playlistTrack& track : playlistTracks)
        {
            
            file << track.file << "\n";
        }
    }
    file.close();
}

void PlaylistComponent::loadPlaylist()
{
    std::ifstream savedPlaylist {"playlist.txt"};
    std::string line;
    juce::Array<juce::File> tempFiles;
    if(savedPlaylist.is_open())
    {
        while(std::getline(savedPlaylist, line))
        {
            juce::File file { line };
            tempFiles.add(file);
        }
    }
    file = tempFiles;
    setTracks(file);
    savedPlaylist.close();
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

void PlaylistComponent::tableColumnsChanged (juce::TableHeaderComponent *tableHeader)
{
    
}
void PlaylistComponent::tableColumnsResized (juce::TableHeaderComponent *tableHeader)
{
    
}
void PlaylistComponent::tableSortOrderChanged (juce::TableHeaderComponent *tableHeader)
{
    
}

void PlaylistComponent::textEditorReturnKeyPressed (juce::TextEditor &)
{
    juce::String searchBoxText = searchBox.getText();
    for(playlistTrack& t : playlistTracks)
    {
        //if no search made then all tracks searched should be false
        if(searchBoxText.isEmpty() == true)
        {
            t.searched = false;
        }
        else
        {
            // check if the title contains the search string, if so set searched to true
            // will be hilighted in red in paint
            if(t.title.contains(searchBoxText))
            {
                t.searched = true;
                tableComponent.updateContent();
            }
            else
            {
                t.searched = false;
                tableComponent.updateContent();
            }
        }
    }
 
}
