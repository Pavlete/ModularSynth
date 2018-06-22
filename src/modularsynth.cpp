#include "modularsynth.h"
#include "modularvoice.h"


ModularSynth::ModularSynth(MidiKeyboardState &keyState,
                           AudioGraphObservable<AudioBufferWrapper>& path)
    : m_keyboardState (keyState)
    , m_synthPath (path)
{
    for (auto i = 0; i < 4; ++i)
        m_synth.addVoice (new ModularVoice(m_synthPath));

    m_synth.addSound (new ModularSound());
}

void ModularSynth::prepareToPlay(int, double sampleRate)
{
    m_synth.setCurrentPlaybackSampleRate (sampleRate);
    m_midiCollector.reset (sampleRate);
}

void ModularSynth::releaseResources() {}

void ModularSynth::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();

    MidiBuffer incomingMidi;
    m_midiCollector.removeNextBlockOfMessages (incomingMidi, bufferToFill.numSamples);
    m_keyboardState.processNextMidiBuffer (incomingMidi, bufferToFill.startSample,
                                         bufferToFill.numSamples, true);

    m_synth.renderNextBlock (*bufferToFill.buffer, incomingMidi,
                           bufferToFill.startSample, bufferToFill.numSamples);
}

MidiMessageCollector *ModularSynth::getMidiCollector()
{
    return &m_midiCollector;
}


