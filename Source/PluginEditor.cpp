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
    
    setSize (400, 300);
    setResizable(true, true);
    addAndMakeVisible(soundGrid);
    viewport.setViewedComponent(&soundGrid);
    addAndMakeVisible(viewport);
    volumeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    keyboardComponent.setAvailableRange(60, 71);
    addAndMakeVisible(keyboardComponent);
    
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
void VocalSynthAudioProcessorEditor::mouseDown (const juce::MouseEvent& e)
{
}
void VocalSynthAudioProcessorEditor::mouseUp (const juce::MouseEvent&)
{
    
}

void VocalSynthAudioProcessorEditor::resized()
{
    //Set Bounding Areas
    auto windowBounds = getLocalBounds();
    auto toolBarArea = windowBounds.removeFromTop(getHeight()/4);
    auto gridArea = windowBounds.removeFromRight(getWidth()*4/5);
    auto pianoArea = windowBounds;
    
    //Tool Bar Area
    auto toolBarWidth = toolBarArea.getWidth();
    auto toolBarHeight = toolBarArea.getHeight();
    toolBarArea.removeFromLeft(toolBarWidth/4);
    toolBarArea.removeFromRight(toolBarWidth/4);
    
    
    juce::FlexBox toolBarFlexBox;
    toolBarFlexBox.flexWrap = juce::FlexBox::Wrap::wrap;
    toolBarFlexBox.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    toolBarFlexBox.alignContent = juce::FlexBox::AlignContent::spaceAround;
    toolBarFlexBox.flexDirection = juce::FlexBox::Direction::row;
    
    toolBarFlexBox.items.add(juce::FlexItem(stopButton).withMinWidth(toolBarWidth/12).withMinHeight(toolBarHeight/8));
    toolBarFlexBox.items.add(juce::FlexItem(playButton).withMinWidth(toolBarWidth/12).withMinHeight(toolBarHeight/8));
    toolBarFlexBox.items.add(juce::FlexItem(volumeSlider).withMinWidth(toolBarWidth/6).withMinHeight(toolBarHeight/2));
    toolBarFlexBox.performLayout(toolBarArea);
    
    //Piano & Grid Area
    keyboardComponent.setKeyWidth(pianoArea.getHeight()/7);
    keyboardComponent.setBounds(pianoArea);
    soundGrid.setBounds(gridArea);
    viewport.setBounds(gridArea);
}

void VocalSynthAudioProcessorEditor::addMessageToList (const juce::MidiMessage& message)
{
    auto time = message.getTimeStamp();

    auto hours   = ((int) (time / 3600.0)) % 24;
    auto minutes = ((int) (time / 60.0)) % 60;
    auto seconds = ((int) time) % 60;
    auto millis  = ((int) (time * 1000.0)) % 1000;

    auto timecode = juce::String::formatted ("%02d:%02d:%02d.%03d",
                                             hours,
                                             minutes,
                                             seconds,
                                             millis);

    //logMessage (timecode + "  -  " + getMidiMessageDescription (message));
}


void VocalSynthAudioProcessorEditor::setNoteNumber (int noteNumber)
{
    auto message = juce::MidiMessage::noteOn (midiChannel, noteNumber, (juce::uint8) 100);
    message.setTimeStamp (juce::Time::getMillisecondCounterHiRes() * 0.001 - startTime);
    addMessageToList (message);
}
