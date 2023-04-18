/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SineWaveVoice.h"

//==============================================================================
VocalSynthAudioProcessor::VocalSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    initialiseSynth();
}

VocalSynthAudioProcessor::~VocalSynthAudioProcessor()
{
}

//==============================================================================
const juce::String VocalSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VocalSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool VocalSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool VocalSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double VocalSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VocalSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int VocalSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void VocalSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String VocalSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void VocalSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void VocalSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    midiCollector.reset (sampleRate);
    synth.setCurrentPlaybackSampleRate (sampleRate);
    //notes.clear();
    //currentNote = 0;
    //lastNoteValue = -1;
    time = 0;
    beatCount = 0;
    mytime = 1;
    rate = static_cast<float> (sampleRate);
}

void VocalSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    keyboardState.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VocalSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void VocalSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto numSamples = buffer.getNumSamples();

    // get note duration
    auto noteDuration = static_cast<int> (std::ceil(rate * 0.25f * (0.1f + (1.0f))));

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, numSamples);

    //midiCollector.removeNextBlockOfMessages(midiMessages, numSamples);

    
    /*for (const auto metadata : midiMessages)
    {
        const auto msg = metadata.getMessage();
        auto msgtime = msg.getTimeStamp();
        //if (msg.getTimeStamp() == mytime)
        //{
            if (msg.isNoteOn())  notes.add(msg.getNoteNumber());
            else if (msg.isNoteOff()) notes2.add(msg.getNoteNumber());
        //}  
    }*/

    //midiMessages.clear();

    if ((time + numSamples) >= noteDuration)
    {
        auto offset = juce::jmax(0, juce::jmin((int)(noteDuration - time), numSamples - 1));
        
        for (int i = 0; i < 12; i++)
        {
            if (myBlocks[i][beatCount])
            {
                midiMessages.addEvent(juce::MidiMessage::noteOn(1, i+60, (juce::uint8)127), offset);
            }
            if (myBlocks[i][(beatCount - 1) % 4])
            {
                midiMessages.addEvent(juce::MidiMessage::noteOff(1, i+60), offset);
            }
        }
        
        /*for (auto note2 : notes2)
        {
            midiMessages.addEvent(juce::MidiMessage::noteOff(1, note2), offset);
        }
        notes2.clear();
        for (auto note : notes)
        {
            midiMessages.addEvent(juce::MidiMessage::noteOn(1, note, (juce::uint8)127), offset);
            notes2.add(note);
        }*/
        beatCount++;
        beatCount = beatCount % 4;
    }

    time = (time + numSamples) % noteDuration;


    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
    }
    //auto numEvents = midiSequence.getNumEvents();
    //for (auto event = 0;  event<numEvents; event++)
    //    midiCollector.addMessageToQueue(output.getMessage());
    //keyboardState.processNextMidiBuffer (midiMessages, 0, numSamples, true);
    synth.renderNextBlock (buffer, midiMessages, 0, numSamples);
}

//==============================================================================
bool VocalSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* VocalSynthAudioProcessor::createEditor()
{
    return new VocalSynthAudioProcessorEditor (*this);
}

//==============================================================================
void VocalSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void VocalSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}
//==============================================================================
void VocalSynthAudioProcessor::initialiseSynth()
{
    auto numVoices = 8;

    // Add some voices...
    for (auto i = 0; i < numVoices; ++i)
    {
        synth.addVoice (new SineWaveVoice());
        synth.addVoice (new juce::SamplerVoice());
    }

    setUsingSineWaveSound();
    //setUsingSampledSound(2);
}

void VocalSynthAudioProcessor::setUsingSineWaveSound()
{
    synth.clearSounds();
    synth.addSound (new SineWaveSound());
}

void VocalSynthAudioProcessor::setUsingSampledSound(int phonemeId)
{
    juce::WavAudioFormat wavFormat;
    juce::MemoryInputStream* input;
    switch(phonemeId)
    {
        case 2:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemeb_wav, BinaryData::phonemeb_wavSize, false);
            break;
        }
        case 3:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemed_wav, BinaryData::phonemed_wavSize, false);
            break;
        }
        case 4:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemef_wav, BinaryData::phonemef_wavSize, false);
            break;
        }
        default:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemeb_wav, BinaryData::phonemeb_wavSize, false);
            break;
        }
    }
    std::unique_ptr<juce::AudioFormatReader> audioReader (wavFormat.createReaderFor (input, true));

    juce::BigInteger allNotes;
    allNotes.setRange (0, 128, true);

    synth.clearSounds();
    synth.addSound (new juce::SamplerSound ("demo sound",
                                      *audioReader,
                                      allNotes,
                                      74,   // root midi note
                                      0.1,  // attack time
                                      0.1,  // release time
                                      10.0  // maximum sample length
                                      ));
}
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VocalSynthAudioProcessor();
}
