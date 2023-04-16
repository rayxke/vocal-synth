/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GridComponent.h"
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
    GridComponent piano;
    Component topMenu;
    juce::Viewport viewport;
    juce::Slider volumeSlider;
    juce::TextButton playButton {"Play"}; //("Play", juce::Color::green, juce::Color::green, juce::Color::green);
    juce::TextButton stopButton {"Stop"}; //("Stop", juce::Color::red, juce::Color::red, juce::Color::red);
    juce::PopupMenu popupMenu;
    VocalSynthAudioProcessor& audioProcessor;
    //juce::AudioSourcePlayer audioSourcePlayer;
    //SynthAudioSource synthAudioSource        { keyboardState };
    juce::MidiKeyboardComponent keyboardComponent;
    //juce::Grid grid;
    juce::Point<float> lastMousePosition;
    std::map<juce::Rectangle<float>, juce::Colour> clicked;
    juce::Rectangle<float> notes[48];
    juce::Colour gridColors[48];
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VocalSynthAudioProcessorEditor)
};
