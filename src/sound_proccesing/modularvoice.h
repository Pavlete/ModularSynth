#pragma once

#include "../proccess_graph/processgraph.h"
#include "../data_models/synthmodel.h"


struct ModularSound : public SynthesiserSound
{
    bool appliesToNote    (int) override        { return true; }
    bool appliesToChannel (int) override        { return true; }
};


class ModularVoice
        : public SynthesiserVoice
        , public SynthModel::Listener
{
public:
    ModularVoice(SynthModel& model);

    bool canPlaySound(SynthesiserSound *sound) override;

    void startNote(int midiNoteNumber,
                   float velocity,
                   SynthesiserSound *sound,
                   int currentPitchWheelPosition) override;

    void stopNote(float velocity, bool allowTailOff) override;

    void pitchWheelMoved(int) override;

    void controllerMoved(int, int) override;

    void renderNextBlock(AudioBuffer<float> &outputBuffer,
                         int startSample,
                         int numSamples) override;

    bool isVoiceActive() const override;

private:
    ProcessGraph m_graph;

    void nodeAdded(const SharedNode& node) override;
    void connectionAdded(const Connection&) override;
    void connectionRemoved(const Connection&) override;
};
