#pragma once

#include "processgraph.h"

namespace  processGraph{

template <class DataType>
class AudioProcessGraph
{
public:
    class AudioNode: public ProcessGraph<DataType>::Node
    {
    public:
        AudioNode(int inNumber, int outNumber)
            : ProcessGraph<DataType>::Node(inNumber,outNumber)
        {}
        virtual ~AudioNode() = default;
        virtual void setActive(float) {}
        virtual void setInactive() {}
        virtual bool isActive() { return false; }
    };

    void setNodesOn(float frequency)
    {
        std::for_each(m_graphIds.begin(), m_graphIds.end(), [&](int id)
        {
            auto node = m_graph.getNode(id);
            if(node)
            {
                static_cast<AudioNode*>(node)->setActive(frequency);
            }
        });
    }

    void setNodesOff()
    {
        std::for_each(m_graphIds.begin(), m_graphIds.end(), [&](int id)
        {
            auto node = m_graph.getNode(id);
            if(node)
            {
                static_cast<AudioNode*>(node)->setInactive();
            }
        });
    }

    bool isActive() const
    {
        auto element = std::find_if(m_graphIds.begin(), m_graphIds.end(), [&](int id)
        {
            auto node = m_graph.getNode(id);
            if(node)
            {
                return static_cast<AudioNode*>(node)->isActive();
            }
        });

        return element != m_graphIds.end();
    }

    bool addNode(int nodeID, std::unique_ptr<AudioNode> node)
    {
        auto ret = m_graph.addNode(nodeID, std::move(node));
        if(ret)
        {
            m_graphIds.insert(nodeID);
        }
        return ret;
    }

    bool removeNode(int nodeId)
    {
        auto ret = m_graph.removeNode(nodeId);
        if(ret)
        {
            m_graphIds.erase(nodeId);
        }
        return ret;
    }

    bool addConnection(ConnectionPoint outputPoint,
                       ConnectionPoint inputPoint)
    {
        return m_graph.addConnection(outputPoint, inputPoint);
    }

    bool removeConnection(ConnectionPoint outputPoint,
                          ConnectionPoint inputPoint)
    {
        return m_graph.removeConnection(outputPoint, inputPoint);
    }

    bool setInitNode(ConnectionPoint outputPoint)
    {
        return m_graph.setInitNode(outputPoint);
    }

    inline void proccessData(DataType& outData)
    {
        m_graph.proccessData(outData);
    }

private:
    processGraph::ProcessGraph<DataType> m_graph;
    std::set<int32_t> m_graphIds;
};
}
