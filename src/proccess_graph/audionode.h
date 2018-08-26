#pragma once

#include "processgraph.h"

class AudioNode
{
public:
    AudioNode(unsigned numIn, unsigned numOut);

    virtual ~AudioNode() = default;
    virtual void setActive(float, float) {}
    virtual void setInactive() {}
    virtual bool isActive() { return false; }

    virtual void process() = 0;

    bool isAvailable(unsigned port, PointDirection direction);

protected:
    AudioBufferWrapper* getInputData(unsigned index);
    AudioBufferWrapper* getOutputData(unsigned index);

private:
    std::vector<std::weak_ptr<ProcessGraph::Edge>> m_inEdges;
    std::vector<std::weak_ptr<ProcessGraph::Edge>> m_outEdges;

    friend class ProcessGraph;
};
