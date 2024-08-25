# Vocal Synthesizer
[![](https://github.com/rayxke/vocal-synth/actions/workflows/build.yml/badge.svg)](https://github.com/rayxke/vocal-synth/actions)

## Overview
A basic Vocal Synthesizer built using JUCE. 
Users can select notes on the sound grid to input a melody. 
Then using the lyric editor type in lyrics for the synth to sing.

<div align="center">
<img align="center" src="https://user-images.githubusercontent.com/8340687/235598501-44e27f80-4aae-40ce-9c87-695ddd81084e.jpg" border-radius="50%" width="800px" height="600px"/>
</div>

## Build

[CMake](https://cmake.org) is required to build. Please download if not already installed.

Clone the repo

```
git clone https://github.com/rayxke/vocal-synth
```

Change to project directory

```
cd vocal-synth
```

Populate [JUCE](https://juce.com)

```
git submodule update --init
```

Then build with CMake

```
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```
This will result in a release build in the build directory.

For more detailed instructions please refer to the [JUCE CMake API](https://github.com/juce-framework/JUCE/blob/master/docs/CMake%20API.md)

## Contributing
- Fork the repository
- Make your changes in a separate branch
- Open a pull request
