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
    
    
    private:
    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboard { keyboardState, juce::MidiKeyboardComponent::verticalKeyboardFacingRight};;
    juce::Point<float> lastMousePosition;
    juce::Viewport viewport;
    std::map<juce::Rectangle<float>, juce::Colour> clicked;
    juce::Rectangle<float> notes[48];
    juce::Colour gridColors[48];
};

