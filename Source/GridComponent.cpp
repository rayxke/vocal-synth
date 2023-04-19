//
//  GridComponent.cpp
//  VocalSynth
//
//

#include "GridComponent.h"
    

GridComponent::GridComponent()
{
    for (int i = 0; i < numKeys; i++)
    {
        for (int j = 0; j < numBeats; j++){
            notes[i][j]
            = juce::Rectangle<float> ((j * getWidth()/4), ((numKeys-1-i) * getHeight()/numKeys), getWidth()/4, getHeight()/numKeys);
            gridColors[i][j] = getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId);
        }
    }
    
}
GridComponent::~GridComponent(){}


void GridComponent::paint(juce::Graphics &g){
    for (int i = 0; i < numKeys; i++)
    {
        for (int j = 0; j < numBeats; j++) {
            
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
    for (int i = 0; i < numKeys; i++)
    {
        for (int j = 0; j < numBeats; j++){
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
    // Layout Grid Blocks
    for (int i = 0; i < numKeys; i++)
    {
        for (int j = 0; j < numBeats; j++) {
            notes[i][j]
            = juce::Rectangle<float>((j * viewPortWidth / 4), ((numKeys-1 - i) * viewPortHeight / numKeys), viewPortWidth / 4, viewPortHeight / numKeys);
        }
    }
    
}

bool GridComponent::getSoundBlocks(int i, int j)
{
    return soundBlocks[i][j];
}

int GridComponent::getNumberOfKeys()
{
    return numKeys;
}

void GridComponent::addBar()
{
    numBeats += 4;
    for (auto i = 0; i < soundBlocks.size(); i++)
    {
        soundBlocks[i].resize(numBeats);
        notes[i].resize(numBeats);
        gridColors[i].resize(numBeats);
    }
}

void GridComponent::setViewPortDimensions(int height, int width)
{
    viewPortHeight = height;
    viewPortWidth = width;
}
