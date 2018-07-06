#include "modularvoice.h"

#include "graph/audiographobservable.h"

ModularVoice::ModularVoice(AudioGraphObservable<AudioBufferWrapper>& observable)
    : m_observable(observable)
{
    m_observable.registerGraph(&m_graph);
}

ModularVoice::~ModularVoice()
{
    m_observable.unregisterGraph(&m_graph);
}

bool ModularVoice::canPlaySound(SynthesiserSound *sound)
{
    return dynamic_cast<ModularSound*> (sound) != nullptr;
}

void ModularVoice::startNote(int midiNoteNumber,
                             float velocity,
                             SynthesiserSound *,
                             int)
{
    m_graph.setNodesOn(static_cast<float>(MidiMessage::getMidiNoteInHertz(midiNoteNumber)));
    m_outAmpl = velocity;
}

void ModularVoice::stopNote(float , bool )
{
    m_graph.setNodesOff();
}

void ModularVoice::renderNextBlock(AudioBuffer<float> &outputBuffer,
                                   int startSample,
                                   int numSamples)
{
    AudioBufferWrapper buff = {&outputBuffer, numSamples, startSample};
    m_graph.proccessData(buff);
    outputBuffer.applyGain(m_outAmpl);
}

bool ModularVoice::isVoiceActive() const
{
    return m_graph.isActive();
}
