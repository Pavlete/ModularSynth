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
    const AudioBufferWrapper* getInputData(unsigned index) const;
    AudioBufferWrapper* getOutputData(unsigned index) const;

private:
    std::vector<std::shared_ptr<ProcessGraph::Edge>> m_inEdges;
    std::vector<std::shared_ptr<ProcessGraph::Edge>> m_outEdges;

    virtual std::unique_ptr<AudioNode> clone() = 0;

    friend class ProcessGraph;
};
