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
    void play();
    void stop();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VocalSynthAudioProcessor& audioProcessor;
    GridComponent soundGrid;
    juce::Viewport viewport;
    juce::Slider volumeSlider;
    juce::TextButton playButton {"Play"};
    juce::TextButton stopButton {"Stop"};
    juce::TextButton recordButton{ "Record" };
    juce::PopupMenu popupMenu;
    juce::MidiKeyboardComponent keyboardComponent;
    juce::Point<float> lastMousePosition;
    std::map<juce::Rectangle<float>, juce::Colour> clicked;
    juce::Rectangle<float> notes[48];
    juce::Colour gridColors[48];
    int midiChannel = 10;
    double startTime;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VocalSynthAudioProcessorEditor)
};
