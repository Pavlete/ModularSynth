#pragma once

#include <JuceHeader.h>

#include "graph/audiographobservable.h"
#include "soundProc/signal.h"
#include "soundProc/waves.h"

class AudioBufferWrapper;

class ModularSynth   : public AudioSource
{
public:
    ModularSynth (MidiKeyboardState& keyState,
                  AudioGraphObservable<AudioBufferWrapper>& path);

    void prepareToPlay (int, double sampleRate) override;

    void releaseResources() override;

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;

    MidiMessageCollector* getMidiCollector();

private:
    MidiKeyboardState& m_keyboardState;
    Synthesiser m_synth;
    MidiMessageCollector m_midiCollector;
    AudioGraphObservable<AudioBufferWrapper>& m_synthPath;
};

//==============================================================================
