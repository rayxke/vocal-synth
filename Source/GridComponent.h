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
    int getNumberOfKeys();
    void addBar();
    void removeBar();
    void setViewPortDimensions(int, int);
    void setPhonemes(juce::StringArray);
    
    
    
    private:
    juce::Point<float> lastMousePosition;
    int viewPortHeight;
    int viewPortWidth;
    int numBeats = 4;
    int numKeys = 12;
    std::vector<std::vector<juce::String>> phonemes = std::vector<std::vector<juce::String>>(numKeys, std::vector<juce::String>(numBeats));
    std::vector<std::vector<juce::Rectangle<float>>> notes = std::vector<std::vector<juce::Rectangle<float>>>(numKeys, std::vector<juce::Rectangle<float>>(numBeats));
    std::vector <std::vector<bool>> soundBlocks = std::vector<std::vector<bool>>(numKeys, std::vector<bool>(numBeats, false));
    std::vector <std::vector<juce::Colour>> gridColors = std::vector<std::vector<juce::Colour>>(numKeys, std::vector<juce::Colour>(numBeats));
};

