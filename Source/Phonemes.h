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
    juce::File fileToRead(juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory().getParentDirectory().getParentDirectory().getParentDirectory().getParentDirectory().getChildFile("Assets").getChildFile("cmudict.txt"));
    auto path = juce::File::getSpecialLocation(juce::File::currentApplicationFile);//fileToRead.getFullPathName();

    if (!fileToRead.existsAsFile())
    {
        tokens.add("file doesn't exist");
        return tokens;  // file doesn't exist
    }

    juce::FileInputStream inputStream(fileToRead); // [2]

    if (!inputStream.openedOk())
    {
        tokens.add("failed to open");
        return tokens;  // failed to open
    }

    juce::String word; //dictonary search declarations
    std::vector<juce::String> phonemes;
    while (!inputStream.isExhausted()) // [3]
    {
        auto search = inputStream.readNextLine();
        juce::StringArray phonemes;
        phonemes.addTokens(search, " ", "\"");
        if (w == phonemes[0])
        {
            phonemes.remove(0);
            return phonemes;
        }
    }
    tokens.add("Word not found");
    return tokens;
}
