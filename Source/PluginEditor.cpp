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
    for (int i = 0; i < 12; i++)
    {
        for (int j = 1; j < 5; j++){
            gridColors[(i)+((j-1)*12)] = juce::Colours::black;
            notes[(i)+((j-1)*12)]
            = juce::Rectangle<float> ((j * getWidth()/5), ((11-i) * getHeight()/12), getWidth()/5, getHeight()/12);
        }
    }
}

VocalSynthAudioProcessorEditor::~VocalSynthAudioProcessorEditor()
{
}

//==============================================================================
void VocalSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 5; j++){
            
            auto rect = juce::Rectangle<float> ((j * getWidth()/5), ((11-i) * getHeight()/12), getWidth()/5, getHeight()/12);
            g.setColour(juce::Colours::white);
            g.drawRect(rect);
            if (j < 1)
            {
                g.drawText(std::to_string(i+1), rect, juce::Justification::centred, false);
            }
            else
            {
                g.setColour(gridColors[(i)+((j-1)*12)]);
                g.fillRect(rect);
                g.setColour(juce::Colours::white);
                g.drawRect(rect);
            }
        }
    }
}
void VocalSynthAudioProcessorEditor::mouseDown (const juce::MouseEvent& e)
{
    lastMousePosition = e.position;
    for (int i = 0; i < 12; i++)
    {
        for (int j = 1; j < 5; j++){
            if (notes[(i)+((j-1)*12)].contains(e.position))
            {
                if (gridColors[(i)+((j-1)*12)] == juce::Colours::black)
                {
                    gridColors[(i)+((j-1)*12)] = juce::Colours::blue;
                }
                else
                {
                    gridColors[(i)+((j-1)*12)] = juce::Colours::black;
                }
                return;
            }
        }
    }
    
    
}
void VocalSynthAudioProcessorEditor::mouseUp (const juce::MouseEvent&)
{
    repaint();
}

void VocalSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    for (int i = 0; i < 12; i++)
    {
        for (int j = 1; j < 5; j++){
            //gridColors[(i)+((j-1)*12)] = juce::Colours::black;
            notes[(i)+((j-1)*12)]
            = juce::Rectangle<float> ((j * getWidth()/5), ((11-i) * getHeight()/12), getWidth()/5, getHeight()/12);
        }
    }
}
