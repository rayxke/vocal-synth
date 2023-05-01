/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class VocalSynthAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    VocalSynthAudioProcessor();
    ~VocalSynthAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    void initialiseSynth();
    void setUsingSineWaveSound();
    void setUsingSampledSound(int);
    
    //==============================================================================
    
    
    juce::Synthesiser synth;
    juce::MidiKeyboardState keyboardState;
    juce::MidiMessageCollector midiCollector;
    juce::MidiMessageSequence midiSequence;
    int time;
    float rate;
    int beatCount = 0;
    int barCount = 0;
    int numBars = 1;
    int beatsPerBar = 4;
    int numKeys = 12;
    bool isPlaying = false;
    std::map<juce::String, int> phonemeMap = {{"AA", 1},{"AE", 2}, {"AH", 3},{"AO", 4},{"AW", 5}, {"AY", 6},
                                              {"B", 7},{"CH", 8}, {"D", 9},{"DH", 10},{"EH", 11}, {"ER", 12},
                                              {"EY", 13},{"F", 14}, {"G", 15},{"HH", 16},{"IH", 17}, {"IY", 18},
                                              {"JH", 19},{"K", 20}, {"L", 21},{"M", 22},{"N", 23}, {"NG", 24},
                                              {"OW", 25},{"OY", 26}, {"P", 27},{"R", 28},{"S", 29}, {"SH", 30},
                                              {"T", 31},{"TH", 32}, {"UH", 33},{"UW", 34},{"V", 35}, {"W", 36},
                                              {"Y", 37},{"Z", 38}, {"ZH", 39}};
    //TODO: Remove Magic numbers by using value tree logic
    std::vector <std::vector<bool>> myBlocks= std::vector<std::vector<bool>>(numKeys, std::vector<bool>(4, false));
    std::vector <std::vector<juce::String>> myPhonemes= std::vector<std::vector<juce::String>>(numKeys, std::vector<juce::String>(4));
    float volume;
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VocalSynthAudioProcessor)
};
