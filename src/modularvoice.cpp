#include "modularvoice.h"

ModularVoice::ModularVoice(SynthModel& model)
{
    model.addListener(this);
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

void ModularVoice::pitchWheelMoved(int) {}

void ModularVoice::controllerMoved(int, int) {}

void ModularVoice::renderNextBlock(AudioBuffer<float> &outputBuffer,
                                   int startSample,
                                   int numSamples)
{
    AudioBufferWrapper buff = {outputBuffer, numSamples, startSample};
    m_graph.proccessData(buff);
    outputBuffer.applyGain(m_outAmpl);
}

bool ModularVoice::isVoiceActive() const
{
    return m_graph.isActive();
}

void ModularVoice::nodeAdded(const SharedNode &node)
{
    m_graph.addNode(node->moduleId, node->getAudioFactory()());
    if (node->isInitModule())
    {
        m_graph.setInitNode({node->moduleId, static_cast<unsigned>(node->initPort())});
    }
}

void ModularVoice::connectionAdded(const Connection &conn)
{
    m_graph.addConnection({conn.connectionOutID(),
                           static_cast<unsigned>(conn.connectionOutPort())},
                          {conn.connectionInID(),
                           static_cast<unsigned>(conn.connectionInPort())});
}

void ModularVoice::connectionRemoved(const Connection &conn)
{
    m_graph.removeConnection({conn.connectionOutID(),
                              static_cast<unsigned>(conn.connectionOutPort())},
                             {conn.connectionInID(),
                              static_cast<unsigned>(conn.connectionInPort())});
}
