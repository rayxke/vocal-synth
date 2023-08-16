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
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    initialiseSynth();
    barCount.setValue(0);
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
    time = 0;
    beatCount = 0;
    volume = 100.0f;
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

    if (isPlaying)
    {
    //Change note after noteDuration
    if ((time + numSamples) >= noteDuration)
    {
        auto offset = juce::jmax(0, juce::jmin((int)(noteDuration - time), numSamples - 1));
        auto beatNum = 4 * (int) barCount.getValue() + beatCount;
        for (int i = 0; i < myBlocks.size(); i++)
        {
            auto noteNum = i + 72;
            if (myBlocks[i][beatNum])
            {
                midiMessages.addEvent(juce::MidiMessage::noteOn(1, noteNum, (juce::uint8)127), offset);
                auto temp = myPhonemes[i][beatNum];
                setUsingSampledSound(phonemeMap[myPhonemes[i][beatNum]]);
            }
            else
            {
                midiMessages.addEvent(juce::MidiMessage::noteOff(1, noteNum), offset);
            }
        }
        beatCount++;
        if (((beatCount%4) == 0) && (beatCount > 0))
        {
            barCount.setValue((int)barCount.getValue() + 1);
            barCount.setValue((int) barCount.getValue() % numBars);
        }
        beatCount = beatCount % 4;
        
    }

    time = (time + numSamples) % noteDuration;
    }
    
    else
    {
        for (int i = 0; i < myBlocks.size(); i++)
        {
            auto offset = juce::jmax(0, juce::jmin((int)(noteDuration - time), numSamples - 1));
            auto noteNum = i + 72;
                midiMessages.addEvent(juce::MidiMessage::noteOff(1, noteNum), offset);
        }
        setUsingSineWaveSound();
        keyboardState.processNextMidiBuffer (midiMessages, 0, numSamples, true);
    }

    synth.renderNextBlock(buffer, midiMessages, 0, numSamples);

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        for (auto sample = 0; sample < numSamples; ++sample)
            channelData[sample] = channelData[sample] * volume;
    }
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
    bool useSin = false;
    switch(phonemeId)
    {
        case 1:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemeaa_wav, BinaryData::phonemeaa_wavSize, false);
            break;
        }
        case 2:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemeae_wav, BinaryData::phonemeae_wavSize, false);
            break;
        }
        case 3:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemeah_wav, BinaryData::phonemeah_wavSize, false);
            break;
        }
        case 4:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemeao_wav, BinaryData::phonemeao_wavSize, false);
            break;
        }
        case 5:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemeaw_wav, BinaryData::phonemeaw_wavSize, false);
            break;
        }
        case 6:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemeay_wav, BinaryData::phonemeay_wavSize, false);
            break;
        }
        case 7:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemeb_wav, BinaryData::phonemeb_wavSize, false);
            break;
        }
        case 8:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemech_wav, BinaryData::phonemech_wavSize, false);
            break;
        }
        case 9:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemed_wav, BinaryData::phonemed_wavSize, false);
            break;
        }
        case 10:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemedh_wav, BinaryData::phonemedh_wavSize, false);
            break;
        }
        case 11:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemeeh_wav, BinaryData::phonemeeh_wavSize, false);
            break;
        }
        case 12:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemeer_wav, BinaryData::phonemeer_wavSize, false);
            break;
        }
        case 13:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemeey_wav, BinaryData::phonemeey_wavSize, false);
            break;
        }
        case 14:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemef_wav, BinaryData::phonemef_wavSize, false);
            break;
        }
        case 15:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemeg_wav, BinaryData::phonemeg_wavSize, false);
            break;
        }
        case 16:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemehh_wav, BinaryData::phonemehh_wavSize, false);
            break;
        }
        case 17:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemeih_wav, BinaryData::phonemeih_wavSize, false);
            break;
        }
        case 18:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemeiy_wav, BinaryData::phonemeiy_wavSize, false);
            break;
        }
        case 19:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemejh_wav, BinaryData::phonemejh_wavSize, false);
            break;
        }
        case 20:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemek_wav, BinaryData::phonemek_wavSize, false);
            break;
        }
        case 21:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemel_wav, BinaryData::phonemel_wavSize, false);
            break;
        }
        case 22:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemem_wav, BinaryData::phonemem_wavSize, false);
            break;
        }
        case 23:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemen_wav, BinaryData::phonemen_wavSize, false);
            break;
        }
        case 24:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemeng_wav, BinaryData::phonemeng_wavSize, false);
            break;
        }
        case 25:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemeow_wav, BinaryData::phonemeow_wavSize, false);
            break;
        }
        case 26:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemeoy_wav, BinaryData::phonemeoy_wavSize, false);
            break;
        }
        case 27:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemep_wav, BinaryData::phonemep_wavSize, false);
            break;
        }
        case 28:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemer_wav, BinaryData::phonemer_wavSize, false);
            break;
        }
        case 29:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemes_wav, BinaryData::phonemes_wavSize, false);
            break;
        }
        case 30:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemesh_wav, BinaryData::phonemesh_wavSize, false);
            break;
        }
        case 31:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemet_wav, BinaryData::phonemet_wavSize, false);
            break;
        }
        case 32:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemeth_wav, BinaryData::phonemeth_wavSize, false);
            break;
        }
        case 33:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemeuh_wav, BinaryData::phonemeuh_wavSize, false);
            break;
        }
        case 34:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemeuw_wav, BinaryData::phonemeuw_wavSize, false);
            break;
        }
        case 35:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemev_wav, BinaryData::phonemev_wavSize, false);
            break;
        }
        case 36:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemew_wav, BinaryData::phonemew_wavSize, false);
            break;
        }
        case 37:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemey_wav, BinaryData::phonemey_wavSize, false);
            break;
        }
        case 38:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemez_wav, BinaryData::phonemez_wavSize, false);
            break;
        }
        case 39:
        {
            input = new juce::MemoryInputStream (BinaryData::phonemezh_wav, BinaryData::phonemezh_wavSize, false);
            break;
        }
        default:
        {
            input = NULL;
            useSin = true;
            break;
        }
    }
    if (!useSin)
    {
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
    else
    {
        setUsingSineWaveSound();
    }
}
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VocalSynthAudioProcessor();
}
