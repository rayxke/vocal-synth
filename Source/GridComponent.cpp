//
//  GridComponent.cpp
//  VocalSynth
//
//

#include "GridComponent.h"
    

GridComponent::GridComponent(){
    
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 4; j++){
            gridColors[i][j] = juce::Colours::black;
            notes[i][j]
            = juce::Rectangle<float> ((j * getWidth()/4) +keyboard.getRight(), ((11-i) * getHeight()/12), getWidth()/4, getHeight()/12);
        }
    }
}
GridComponent::~GridComponent(){}


void GridComponent::paint(juce::Graphics &g){
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 4; j++){
            
            auto rect = juce::Rectangle<float> ((j * getWidth()/4)+keyboard.getRight(), ((11-i) * getHeight()/12), getWidth()/4, getHeight()/12);
            g.setColour(juce::Colours::white);
            g.drawRect(rect);
                g.setColour(gridColors[i][j]);
                g.fillRect(rect);
                g.setColour(juce::Colours::white);
                g.drawRect(rect);
        }
    }
}

void GridComponent::mouseDown (const juce::MouseEvent& e)
{
    lastMousePosition = e.position;
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 4; j++){
            if (notes[i][j].contains(e.position))
            {
                soundBlocks[i][j] = !soundBlocks[i][j];
                gridColors[i][j]  = soundBlocks[i][j] ? juce::Colours::blue : juce::Colours::black;
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
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 4; j++){
            notes[i][j]
            = juce::Rectangle<float> ((j * getWidth()/4)+keyboard.getRight(), ((11-i) * getHeight()/12), getWidth()/4, getHeight()/12);
        }
    }
    
}


bool * GridComponent::getSoundBlocks()
{
    return soundBlocks;
}
