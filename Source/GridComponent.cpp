//
//  GridComponent.cpp
//  VocalSynth
//
//

#include "GridComponent.h"
    

GridComponent::GridComponent()
{   
 
    for (int i = 0; i < notes.size(); i++)
    {
        for (int j = 0; j < notes[i].size(); j++){
            notes[i][j]
            = juce::Rectangle<float> ((j * getWidth()/4), ((11-i) * getHeight()/12), getWidth()/4, getHeight()/12);
            gridColors[i][j] = getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId);
        }
    }
    
}
GridComponent::~GridComponent(){}


void GridComponent::paint(juce::Graphics &g){
    for (int i = 0; i < notes.size(); i++)
    {
        for (int j = 0; j < notes[i].size(); j++) {
            
           //auto rect = juce::Rectangle<float> ((j * getWidth()/4), ((11-i) * getHeight()/12), getWidth()/4, getHeight()/12);
            g.setColour(juce::Colours::white);
            g.drawRect(notes[i][j]);
            g.setColour(gridColors[i][j]);
            g.fillRect(notes[i][j]);
            g.setColour(juce::Colours::white);
            g.drawRect(notes[i][j]);
            g.drawText(std::to_string(i), notes[i][j], juce::Justification::left);
        }
    }
}

void GridComponent::mouseDown (const juce::MouseEvent& e)
{
    lastMousePosition = e.position;
    for (int i = 0; i < soundBlocks.size(); i++)
    {
        for (int j = 0; j < soundBlocks[i].size(); j++){
            if (notes[i][j].contains(e.position))
            {
                soundBlocks[i][j] = !soundBlocks[i][j];
                gridColors[i][j] = soundBlocks[i][j] ? juce::Colours::blue : getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId);//juce::Colours::backgroundColor;
                return;
            }
        }
    }
}


void GridComponent::mouseUp (const juce::MouseEvent&)
{
    repaint();
}


void GridComponent::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto localWindow = getLocalBounds();
    auto keyboardArea = localWindow.removeFromLeft(getWidth()/5);
    auto gridArea = localWindow.removeFromRight(getWidth()* 4/5);
    for (int i = 0; i < notes.size(); i++)
    {
        for (int j = 0; j < notes[i].size(); j++) {
            notes[i][j]
            = juce::Rectangle<float> ((j * getWidth()/4), ((11-i) * getHeight()/12), getWidth()/4, getHeight()/12);
        }
    }
    
}

bool GridComponent::getSoundBlocks(int i, int j)
{
    return soundBlocks[i][j];
}
