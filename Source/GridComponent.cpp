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
            gridColors[(i)+((j)*12)] = juce::Colours::black;
            notes[(i)+((j)*12)]
            = juce::Rectangle<float> ((j * getWidth()/4) +keyboard.getRight(), ((11-i) * getHeight()/12), getWidth()/4, getHeight()/12);
        }
    }
    //addAndMakeVisible(keyboard);
    //addAndMakeVisible(viewport);
    //viewport.setViewedComponent(piano);
    //keyboard.setAvailableRange(60, 72);
}
GridComponent::~GridComponent(){}


void GridComponent::paint(juce::Graphics &g){
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 4; j++){
            
            auto rect = juce::Rectangle<float> ((j * getWidth()/4)+keyboard.getRight(), ((11-i) * getHeight()/12), getWidth()/4, getHeight()/12);
            g.setColour(juce::Colours::white);
            g.drawRect(rect);
            //if (j < 1)
            //{
            //    g.drawText(std::to_string(i+1), rect, juce::Justification::centred, false);
            //}
            //else
            //{
                g.setColour(gridColors[(i)+((j)*12)]);
                g.fillRect(rect);
                g.setColour(juce::Colours::white);
                g.drawRect(rect);
            //}
        }
    }
}

void GridComponent::mouseDown (const juce::MouseEvent& e)
{
    lastMousePosition = e.position;
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 4; j++){
            if (notes[(i)+((j)*12)].contains(e.position))
            {
                if (gridColors[(i)+((j)*12)] == juce::Colours::black)
                {
                    gridColors[(i)+((j)*12)] = juce::Colours::blue;
                }
                else
                {
                    gridColors[(i)+((j)*12)] = juce::Colours::black;
                }
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
    //keyboard.setBounds(keyboardArea);
    //keyboard.setKeyWidth(keyboardArea.getHeight()/12);
    //viewport.setBounds(localWindow);
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 4; j++){
            //gridColors[(i)+((j-1)*12)] = juce::Colours::black;
            notes[(i)+((j)*12)]
            = juce::Rectangle<float> ((j * getWidth()/4)+keyboard.getRight(), ((11-i) * getHeight()/12), getWidth()/4, getHeight()/12);
        }
    }
    
}
