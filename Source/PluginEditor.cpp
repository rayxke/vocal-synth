/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VocalSynthAudioProcessorEditor::VocalSynthAudioProcessorEditor (VocalSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
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
    //topMenu.addAndMakeVisible(&volumeSlider);
    //addAndMakeVisible(topMenu);
    
}

VocalSynthAudioProcessorEditor::~VocalSynthAudioProcessorEditor()
{
}

//==============================================================================
void VocalSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    //auto x = getLocalBounds().getX();
    //auto y = getLocalBounds().getCentreY();
    //g.drawRect(x, y, 10, 10);
    //g.setColour(juce::Colours::white);
    //g.drawRect(0, 0, getWidth(), getHeight()/8);
    //g.drawRect(0, getHeight()/8, getWidth(), getHeight()/8);
    //g.setColour(juce::Colours::white);
    //g.drawRect(0, getHeight()/4, getWidth()/5, getHeight()*3/4);
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
    //auto pianoArea = windowBounds.removeFromRight(getWidth());
    auto gridArea = windowBounds.removeFromRight(getWidth()*4/5);
    auto pianoArea = windowBounds;
    auto x = getWidth()/ 5;
    auto y = getHeight()/4;
    
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
    
    //Piano Grid Area
    keyboardComponent.setKeyWidth(pianoArea.getHeight()/7);
    keyboardComponent.setBounds(pianoArea);
    piano.setBounds(gridArea);
    //keyboardComponent.setKeyWidth(piano.getHeight()/12);
    viewport.setBounds(gridArea);
    //keyboardComponent.setBounds(panelArea);
}
