/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VocalSynthAudioProcessorEditor::VocalSynthAudioProcessorEditor (VocalSynthAudioProcessor& p)
: AudioProcessorEditor (&p), audioProcessor (p), keyboardComponent(p.keyboardState, juce::MidiKeyboardComponent::verticalKeyboardFacingRight)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    setSize (400, 300);
    setResizable(true, true);
    addAndMakeVisible(piano);
    viewport.setViewedComponent(&piano);
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
    //repaint();
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
    piano.setBounds(gridArea);
    viewport.setBounds(gridArea);
}
