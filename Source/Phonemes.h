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
    
    juce::MemoryInputStream* inputStream = new juce::MemoryInputStream (BinaryData::cmudict_txt, BinaryData::cmudict_txtSize, false);

    std::vector<juce::String> phonemes;
    
    while (!inputStream->isExhausted())
    {
        auto search = inputStream->readNextLine();
        juce::StringArray phonemes;
        phonemes.addTokens(search, " ", "\"");
        if (w == phonemes[0])
        {
            phonemes.remove(0);
            delete inputStream;
            return phonemes;
        }
    }
    delete inputStream;
    tokens.add("Word not found");
    return tokens;
}
