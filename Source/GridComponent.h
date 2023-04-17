//
//  GridComponent.hpp
//  VocalSynth
//
//

#ifndef GridComponent_h
#define GridComponent_h
#endif /* GridComponent_h */

#pragma once

#include <JuceHeader.h>


class GridComponent  : public juce::Component
{
public:
    GridComponent ();
    ~GridComponent() override;
    void paint (juce::Graphics& g) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent&) override;
    void mouseUp (const juce::MouseEvent&) override;
    bool getSoundBlocks(int, int);
    
    
    
    private:
    juce::Point<float> lastMousePosition;
    juce::Viewport viewport;
    juce::Rectangle<float> notes[12][4];
    bool soundBlocks[12][4] = {false};
    juce::Colour gridColors[12][4] = { juce::Colours::black };
};

