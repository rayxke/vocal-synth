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
    std::vector<std::vector<juce::Rectangle<float>>> notes = std::vector<std::vector<juce::Rectangle<float>>>(12, std::vector<juce::Rectangle<float>>(4));
    std::vector <std::vector<bool>> soundBlocks = std::vector<std::vector<bool>>(12, std::vector<bool>(4));
    std::vector <std::vector<juce::Colour>> gridColors = std::vector<std::vector<juce::Colour>>(12, std::vector<juce::Colour>(4));
};

