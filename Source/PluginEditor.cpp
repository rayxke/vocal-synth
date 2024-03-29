/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VocalSynthAudioProcessorEditor::VocalSynthAudioProcessorEditor (VocalSynthAudioProcessor& p)
: AudioProcessorEditor (&p), audioProcessor (p), keyboardComponent(p.keyboardState, juce::MidiKeyboardComponent::verticalKeyboardFacingRight), startTime (juce::Time::getMillisecondCounterHiRes() * 0.001)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    setSize (800, 600);
    setResizable(true, true);
    
    //Set Up SoundGrid & ViewPort
    viewport.setViewedComponent(&soundGrid, false);
    soundGrid.setViewPortDimensions(viewport.getHeight(), viewport.getWidth());
    addAndMakeVisible(viewport);
    
    //ToolBar Buttons & Sliders
    addAndMakeVisible(volumeSlider);
    volumeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    volumeSlider.setValue (volumeSlider.getMaximum());
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(addBarButton);
    addAndMakeVisible(removeBarButton);
    addAndMakeVisible(convertButton);
    addAndMakeVisible(clearButton);
    addAndMakeVisible(resetButton);
    
    //Lyric Editor Button
    addAndMakeVisible(lyricEditorLabel);
    lyricEditorLabel.setText("Lyric Editor:", juce::dontSendNotification);
    lyricEditorLabel.attachToComponent(&lyricEditor, true);
    lyricEditorLabel.setColour(juce::Label::textColourId, juce::Colours::orange);
    lyricEditorLabel.setJustificationType(juce::Justification::right);
    lyricEditorLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    addAndMakeVisible(lyricEditor);
    lyricEditor.setEditable(true);
    lyricEditor.setColour(juce::Label::backgroundColourId, juce::Colours::darkblue);
    
    //Keyboard
    keyboardComponent.setAvailableRange(60, 71);
    addAndMakeVisible(keyboardComponent);

    //Click Functions
    playButton.onClick = [this] {play();};
    stopButton.onClick = [this] {stop();};
    addBarButton.onClick = [this] {addBar();};
    removeBarButton.onClick = [this] {removeBar();};
    convertButton.onClick = [this] {convertToPhoneme(); };
    clearButton.onClick = [this] {clearPhonemes(); };
    resetButton.onClick = [this] {reset(); };
    volumeSlider.onValueChange = [this] {updateVolume(); };

    //Set Volume
    audioProcessor.volume = (float) volumeSlider.getValue();
}

VocalSynthAudioProcessorEditor::~VocalSynthAudioProcessorEditor()
{
}

//==============================================================================
void VocalSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void VocalSynthAudioProcessorEditor::resized()
{
    //Set Bounding Areas
    auto windowBounds = getLocalBounds();
    auto toolBarArea1 = windowBounds.removeFromTop(getHeight()/4);
    auto toolBarArea2 = toolBarArea1.removeFromBottom(toolBarArea1.getHeight()/2);
    auto gridArea = windowBounds.removeFromRight(getWidth()*4/5);
    auto pianoArea = windowBounds;
    
    //Tool Bar Area
    auto toolBarWidth = toolBarArea1.getWidth();
    auto toolBarHeight = toolBarArea1.getHeight();
    
    //Add Items to ToolBar
    juce::FlexBox toolBarFlexBox;
    toolBarFlexBox.flexWrap = juce::FlexBox::Wrap::wrap;
    toolBarFlexBox.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    toolBarFlexBox.alignContent = juce::FlexBox::AlignContent::spaceAround;
    toolBarFlexBox.flexDirection = juce::FlexBox::Direction::row;
    
    toolBarFlexBox.items.add(juce::FlexItem(addBarButton).withMinWidth(toolBarWidth / 12).withMinHeight(toolBarHeight / 8));
    toolBarFlexBox.items.add(juce::FlexItem(removeBarButton).withMinWidth(toolBarWidth / 12).withMinHeight(toolBarHeight / 8));
    toolBarFlexBox.items.add(juce::FlexItem(playButton).withMinWidth(toolBarWidth/12).withMinHeight(toolBarHeight/8));
    toolBarFlexBox.items.add(juce::FlexItem(stopButton).withMinWidth(toolBarWidth / 12).withMinHeight(toolBarHeight / 8));
    toolBarFlexBox.items.add(juce::FlexItem(resetButton).withMinWidth(toolBarWidth / 12).withMinHeight(toolBarHeight / 8));
    toolBarFlexBox.items.add(juce::FlexItem(volumeSlider).withMinWidth(toolBarWidth/6).withMinHeight(toolBarHeight/2));
    toolBarFlexBox.performLayout(toolBarArea1);
    
    
    juce::FlexBox toolBarFlexBox2;
    toolBarFlexBox2.flexWrap = juce::FlexBox::Wrap::wrap;
    toolBarFlexBox2.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    toolBarFlexBox2.alignContent = juce::FlexBox::AlignContent::spaceAround;
    toolBarFlexBox2.flexDirection = juce::FlexBox::Direction::row;
    
    toolBarFlexBox2.items.add(juce::FlexItem(lyricEditor).withMinWidth(toolBarWidth / 6).withMinHeight(toolBarHeight / 4));
    toolBarFlexBox2.items.add(juce::FlexItem(convertButton).withMinWidth(toolBarWidth / 12).withMinHeight(toolBarHeight / 8));
    toolBarFlexBox2.items.add(juce::FlexItem(clearButton).withMinWidth(toolBarWidth / 12).withMinHeight(toolBarHeight / 8));
    
    toolBarArea2.removeFromLeft(toolBarWidth/4);
    toolBarArea2.removeFromRight(toolBarWidth/4);
    toolBarFlexBox2.performLayout(toolBarArea2);
    
    //Piano & Grid Area
    keyboardComponent.setKeyWidth(pianoArea.getHeight()/7);
    keyboardComponent.setBounds(pianoArea);
    //keyboardComponent.getKeyWidth();
    
    //Grid & Viewport Sizing
    viewport.setBounds(gridArea);
    soundGrid.setViewPortDimensions(viewport.getHeight(), viewport.getWidth());
    
    soundGrid.setBounds(gridArea.getX(), gridArea.getY(), gridArea.getWidth() * audioProcessor.numBars, gridArea.getHeight());
    

}

void VocalSynthAudioProcessorEditor::play()
{
    for (int i = 0; i < soundGrid.getNumberOfKeys(); i++)
        for (int j = 0; j < totalNumBeats; j++)
        {
            audioProcessor.myBlocks[i][j] = soundGrid.getSoundBlocks(i, j);
            audioProcessor.myPhonemes[i][j] = soundGrid.getPhonemes(i, j);
        }
    audioProcessor.isPlaying = true;
}


void VocalSynthAudioProcessorEditor::stop()
{

    for (int i = 0; i < soundGrid.getNumberOfKeys(); i++)
        for (int j = 0; j < totalNumBeats; j++)
            audioProcessor.myBlocks[i][j] = false;
    audioProcessor.isPlaying = false;
}

void VocalSynthAudioProcessorEditor::updateVolume()
{
    audioProcessor.volume = (float) volumeSlider.getValue();
}

void VocalSynthAudioProcessorEditor::addBar()
{
    audioProcessor.numBars++;
    totalNumBeats = audioProcessor.numBars * audioProcessor.beatsPerBar;
    soundGrid.addBar();
    for (int i = 0; i < soundGrid.getNumberOfKeys(); i++)
    {
        audioProcessor.myBlocks[i].resize(totalNumBeats);
        audioProcessor.myPhonemes[i].resize(totalNumBeats);
    }
    resized();
}

void VocalSynthAudioProcessorEditor::removeBar()
{
    if (audioProcessor.numBars == 1)
    {
        return;
    }
    audioProcessor.numBars--;
    totalNumBeats = audioProcessor.numBars * audioProcessor.beatsPerBar;
    soundGrid.removeBar();
    for (int i = 0; i < soundGrid.getNumberOfKeys(); i++)
    {
        audioProcessor.myBlocks[i].resize(totalNumBeats);
        audioProcessor.myPhonemes[i].resize(totalNumBeats);
    }
    resized();
}

void VocalSynthAudioProcessorEditor::convertToPhoneme()
{
    auto phonemes = findPhoneme(lyricEditor.getText());
    soundGrid.setPhonemes(phonemes);
}

void VocalSynthAudioProcessorEditor::clearPhonemes()
{
    soundGrid.clearPhonemes();
    lyricEditor.setText("", juce::dontSendNotification);
}

void VocalSynthAudioProcessorEditor::reset()
{
    stop();
    soundGrid.clearPhonemes();
    soundGrid.clearSoundBlocks();
    lyricEditor.setText("", juce::dontSendNotification);
}
