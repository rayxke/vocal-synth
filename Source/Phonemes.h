/*
  ==============================================================================

    Phonemes.h
    Created: 20 Apr 2023 8:24:30am
    Author:  sks

  ==============================================================================
*/
#ifndef Phonemes_h
#define Phonemes_h
#endif /* Phonemes_h */
#pragma once

#include <JuceHeader.h>


//Takes in a word w and returns its pronunciation if 
//it is in the dictionary


inline juce::StringArray findPhoneme(const juce::String w)
{   
    juce::StringArray tokens;
    
    std::unique_ptr<juce::MemoryInputStream> inputStream = std::make_unique<juce::MemoryInputStream> (BinaryData::cmudict_txt, BinaryData::cmudict_txtSize, false);

    std::vector<juce::String> phonemes;
    
    while (!inputStream->isExhausted())
    {
        auto search = inputStream->readNextLine();
        juce::StringArray phonemes;
        phonemes.addTokens(search, " ", "\"");
        if (w.compareIgnoreCase(phonemes[0]) == 0)
        {
            phonemes.remove(0);
            return phonemes;
        }
    }
    tokens.add("Word not found");
    return tokens;
}
