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
            = juce::Rectangle<float> ((j * getWidth()/8), ((numKeys-1-i) * getHeight()/numKeys), getWidth()/8, getHeight()/numKeys);
            gridColors[i][j] = getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId);
            phonemes[i][j] = "";
        }
    }
    
}
GridComponent::~GridComponent(){}


void GridComponent::paint(juce::Graphics &g){
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    for (int i = 0; i < numKeys; i++)
    {
        for (int j = 0; j < numBeats; j++) {
            
            g.setColour(juce::Colours::white);
            g.drawRect(notes[i][j]);
            g.setColour(gridColors[i][j]);
            g.fillRect(notes[i][j]);
            g.setColour(juce::Colours::white);
            g.drawRect(notes[i][j]);
            g.drawText(phonemes[i][j], notes[i][j], juce::Justification::left);
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
            = juce::Rectangle<float>((j * viewPortWidth / 8), ((numKeys-1 - i) * viewPortHeight / numKeys), viewPortWidth / 8, viewPortHeight / numKeys);
        }
    }
    
}

bool GridComponent::getSoundBlocks(int i, int j)
{
    return soundBlocks[i][j];
}

void GridComponent::clearSoundBlocks()
{
    for (int i = 0; i < numKeys; i++)
    {
        for (int j = 0; j < numBeats; j++) {
            soundBlocks[i][j] = false;
            gridColors[i][j] = getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId);
        }
    }
    repaint();
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
        phonemes[i].resize(numBeats);
    }
}

void GridComponent::removeBar()
{
    numBeats -= 4;
    for (auto i = 0; i < soundBlocks.size(); i++)
    {
        soundBlocks[i].resize(numBeats);
        notes[i].resize(numBeats);
        gridColors[i].resize(numBeats);
        phonemes[i].resize(numBeats);
    }
}

void GridComponent::setViewPortDimensions(int height, int width)
{
    viewPortHeight = height;
    viewPortWidth = width;
}
void GridComponent::clearPhonemes()
{
    for (int j = 0; j < numBeats; j++)
    {
        for (int i = 0; i < numKeys; i++)
        {
                phonemes[i][j] = "";
        }
    }
    repaint();
}
void GridComponent::setPhonemes(juce::StringArray phonemeArray)
{
    int count = 0;
    bool phonemeSet = false;
    
    //do a check for phoneme Array has phonemes
    
    for (int j = 0; j < numBeats; j++)
    {
        for (int i = 0; i < numKeys; i++)
        {
            if (soundBlocks[i][j])
            {
                phonemes[i][j]
                = phonemeArray[count].removeCharacters("0123456789");
                phonemeSet = true;
            }
        }
        if (phonemeSet)
        {
            count++;
            phonemeSet = false;
        }
    }
    repaint();
}

juce::String GridComponent::getPhonemes(int i, int j)
{
    return phonemes[i][j];
}


