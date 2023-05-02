# Vocal Synthesizer

## Overview
A basic Vocal Synthesizer built using JUCE. 
Users can select notes on the sound grid to input a melody. 
Then using the lyric editor type in lyrics for the synth to sing.

<div align="center">
<img align="center" src="https://user-images.githubusercontent.com/8340687/235598501-44e27f80-4aae-40ce-9c87-695ddd81084e.jpg" border-radius="50%" width="800px" height="600px"/>
</div>

## Build Locally

### Mac OS
Step 1: Installations

- Install [JUCE](https://juce.com) from source
- Install [XCode](https://developer.apple.com/xcode/)

Step 2: Clone The Repo
```
git clone https://github.com/rayxke/JUCE-FM-plugin
```


Step 3: Using the [Projucer](https://juce.com/discover/projucer) open the "VocalSynth.jucer" file.

Step 4: Click the export button in the Projucer

Step 5: Click the build button in XCode

Step 6: Find the build file (Optional)
- Find build file in this subdirectory within your project folder:
```
cd Builds/MacOSX/build/Debug/
```
- "VocalSynth.app" is the name of the app

### Windows

Step 1: Installations

- Install [JUCE](https://juce.com) from source
- Install [Visual Studio](https://visualstudio.microsoft.com)

Step 2: Clone the Repo

```
git clone https://github.com/rayxke/JUCE-FM-plugin
```

Step 3: Creating Project

- Open Visual Studio
- continue without code --> file --> new --> project from existing code
- Select the folder in which your code is located in
- Build or press the green play arrow (this should throw an error at first, but we need to have a build file)

Step 4: Open in JUCE

- Open JUCE
- Click "Open"
- Select a file from your project folder
- Set "Selected exporter" to Visual Studio 
- Click the Visual Studio icon

Step 5: Run code

- Click the green play arrow

## Contributing

- Fork the repository
- Make your changes in a separate branch
- Open a pull request

## Download
Feel free to download the app
- [MacOS](https://drive.google.com/file/d/18qru1GqMEcxcVK34MrYZSoA2K8yopMTf/view?usp=share_link)
- [Windows](https://drive.google.com/file/d/19hKhKB1shqzzxtx0NtivVOxBa2lu08r6/view?usp=share_link)

