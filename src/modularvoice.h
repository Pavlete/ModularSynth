#pragma once

#include "nodes/common/juceaudionode.h"

template <class T>
class AudioGraphObservable;

struct ModularSound : public SynthesiserSound
{
    bool appliesToNote    (int) override        { return true; }
    bool appliesToChannel (int) override        { return true; }
};


class ModularVoice: public SynthesiserVoice
{
public:
    using AudioGraphType = processGraph::AudioProcessGraph<AudioBufferWrapper>;
    using NodeType = processGraph::AudioProcessGraph<AudioBufferWrapper>::AudioNode;

    ModularVoice(AudioGraphObservable<AudioBufferWrapper>& observable);
    ~ModularVoice();

    bool canPlaySound(SynthesiserSound *sound) override;

    void startNote(int midiNoteNumber,
                   float velocity,
                   SynthesiserSound *sound,
                   int currentPitchWheelPosition) override;

    void stopNote(float velocity, bool allowTailOff) override;

    void pitchWheelMoved(int) override {}

    void controllerMoved(int, int) override {}

    void renderNextBlock(AudioBuffer<float> &outputBuffer,
                         int startSample,
                         int numSamples) override;

    bool isVoiceActive() const override;

private:
    processGraph::AudioProcessGraph<AudioBufferWrapper> m_graph;
    AudioGraphObservable<AudioBufferWrapper>& m_observable;

    float m_outAmpl;
};
