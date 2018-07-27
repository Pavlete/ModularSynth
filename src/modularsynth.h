#pragma once

#include <JuceHeader.h>

#include "nodes/common/synthmodel.h"


class AudioBufferWrapper;

class ModularSynth : public AudioSource
{
public:
    ModularSynth (MidiKeyboardState& keyState,
                  SynthModel& model);

    void prepareToPlay (int, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;

    MidiMessageCollector* getMidiCollector();

private:
    MidiKeyboardState& m_keyboardState;
    Synthesiser m_synth;
    MidiMessageCollector m_midiCollector;
};

//==============================================================================
