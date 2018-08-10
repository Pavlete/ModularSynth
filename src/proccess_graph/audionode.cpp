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
    return edges.size() > port && edges.at(port).expired();
}

AudioBufferWrapper *AudioNode::getInputData(unsigned index)
{
    if(m_inEdges.size() < index)
    {
        return nullptr;
    }

    auto ptr = m_inEdges[index].lock();
    if(!ptr)
    {
        return nullptr;
    }

    return ptr->getData();
}

AudioBufferWrapper *AudioNode::getOutputData(unsigned index)
{
    if(m_outEdges.size() < index)
    {
        return nullptr;
    }

    auto ptr = m_outEdges[index].lock();
    if(!ptr)
    {
        return nullptr;
    }

    return ptr->getData();
}
