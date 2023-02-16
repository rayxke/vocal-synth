/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class VocalSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    VocalSynthAudioProcessorEditor (VocalSynthAudioProcessor&);
    ~VocalSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent&) override;
    void mouseUp (const juce::MouseEvent&) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VocalSynthAudioProcessor& audioProcessor;
    juce::MidiKeyboardState keyboardState;
    juce::AudioSourcePlayer audioSourcePlayer;
    //SynthAudioSource synthAudioSource        { keyboardState };
    juce::MidiKeyboardComponent keyboardComponent  { keyboardState, juce::MidiKeyboardComponent::verticalKeyboardFacingRight};
    //juce::Grid grid;
    juce::Point<float> lastMousePosition;
    std::map<juce::Rectangle<float>, juce::Colour> clicked;
    juce::Rectangle<float> notes[48];
    juce::Colour gridColors[48];
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VocalSynthAudioProcessorEditor)
};
