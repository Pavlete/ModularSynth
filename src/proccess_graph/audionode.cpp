#include "audionode.h"


AudioNode::AudioNode(uint32_t numIn, uint32_t numOut)
    : m_inEdges(numIn)
    , m_outEdges(numOut)
{}


bool AudioNode::isAvailable(unsigned port,
                                 PointDirection direction)
{
    auto edges = direction == PointDirection::Input?
                m_inEdges : m_outEdges;
    return edges.size() > port && !edges.at(port);
}

const AudioBufferWrapper* AudioNode::getInputData(unsigned index) const
{
    if(auto ptr = m_inEdges[index])
    {
        return ptr->getData();
    }

    return nullptr;
}

AudioBufferWrapper* AudioNode::getOutputData(unsigned index) const
{
    if(auto ptr = m_outEdges[index])
    {
        return ptr->getData();
    }

    return nullptr;
}
