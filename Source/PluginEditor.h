/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GridComponent.h"
#include "Phonemes.h"
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
    void play();
    void stop();
    void incBar();
    void addBar();
    void removeBar();
    void convertToPhoneme();
    void clearPhonemes();
    void reset();
    void updateVolume();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VocalSynthAudioProcessor& audioProcessor;
    GridComponent soundGrid;
    juce::Viewport viewport;
    juce::Slider volumeSlider;
    juce::ShapeButton playButton {"Play", juce::Colours::green, juce::Colours::red, juce::Colours::blue};
    juce::ShapeButton stopButton {"Stop", juce::Colours::red, juce::Colours::green, juce::Colours::blue};
    juce::Slider incBarButton {juce::Slider::IncDecButtons, juce::Slider::TextBoxBelow};
    juce::TextButton addBarButton{ "+" };
    juce::TextButton removeBarButton{ "-" };
    juce::TextButton convertButton{ "Convert" };
    juce::TextButton clearButton{ "Clear" };
    juce::TextButton resetButton{ "Reset" };
    juce::MidiKeyboardComponent keyboardComponent;
    juce::Label lyricEditorLabel;
    juce::Label lyricEditor;
    double startTime;
    int totalNumBeats = 4;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VocalSynthAudioProcessorEditor)
};
