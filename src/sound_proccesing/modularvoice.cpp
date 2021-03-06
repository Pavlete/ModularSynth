#include "modularvoice.h"
#include "../proccess_graph/audionode.h"


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
    m_graph.setNodesOn(static_cast<float>(MidiMessage::getMidiNoteInHertz(midiNoteNumber)),
                       velocity);
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
    if(isVoiceActive())
    {
        AudioBufferWrapper buff = {outputBuffer, numSamples, startSample};
        m_graph.proccessData(buff);
    }
}

bool ModularVoice::isVoiceActive() const
{
    return m_graph.isActive();
}

void ModularVoice::nodeAdded(const SharedNode &node)
{
    m_graph.addNode(node->ID(), node->getAudioFactory()());
    if (node->isInitModule())
    {
        m_graph.setInitNode({node->ID(), static_cast<unsigned>(node->initPort())});
    }
}

void ModularVoice::nodeRemoved(int nodeID)
{
    m_graph.removeNode(nodeID);
}

void ModularVoice::connectionAdded(const Connection &conn)
{
    m_graph.addConnection({conn.outputNode(), conn.outputPort()},
                          {conn.inputNode(), conn.inputPort()});
}


void ModularVoice::connectionRemoved(int outputID, unsigned int outputPort,
                                     int inputID, unsigned int inputPort)
{
    m_graph.removeConnection({outputID, outputPort},
                             {inputID,inputPort});
}
