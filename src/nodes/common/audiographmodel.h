#pragma once

#include "../../graph/audiographobservable.h"
#include "nodemodel.h"

template <class NodeType, class ConnectionType, class DataType>
class AudioGraphModel : public NodeModel<NodeType, ConnectionType>
{
public:
    using ObservableAudioGraph = AudioGraphObservable<DataType>;

    ObservableAudioGraph& getObservable();

    int addNode(std::unique_ptr<NodeType> comp,
                int x, int y) override;
    int addInitNode(std::unique_ptr<NodeType> comp,
                    int x, int y, int outIndex) override;

    bool connectNodes(const processGraph::ConnectionPoint& out,
                      const processGraph::ConnectionPoint& in);
    bool disconnectNodes(const processGraph::ConnectionPoint& out,
                         const processGraph::ConnectionPoint& in);

private:
    ObservableAudioGraph m_graph;
};




//------------------------------------------------------------------//




template <class NodeType, class ConnectionType, class DataType>
typename AudioGraphModel<NodeType, ConnectionType, DataType>::ObservableAudioGraph
&AudioGraphModel<NodeType, ConnectionType, DataType>::getObservable()
{
    return m_graph;
}

template <class NodeType, class ConnectionType, class DataType>
int
AudioGraphModel<NodeType, ConnectionType, DataType>::addNode(std::unique_ptr<NodeType> comp,
                              int x, int y)
{
    auto nodeId = m_graph.addModule(comp->getModule());
    comp->setNodeID(nodeId);
    if(nodeId != processGraph::INVALID_NODE_ID)
    {
        NodeModel<NodeType, ConnectionType>::addNode(std::move(comp), x, y);
        return nodeId;
    }

    return processGraph::INVALID_NODE_ID;
}

template <class NodeType, class ConnectionType, class DataType>
int
AudioGraphModel<NodeType, ConnectionType, DataType>::addInitNode(std::unique_ptr<NodeType> comp,
                                  int x, int y,
                                  int outIndex)
{
    auto nodeAdded = addNode(std::move(comp), x, y);
    if(nodeAdded != processGraph::INVALID_NODE_ID)
    {
        m_graph.setInitialModule(nodeAdded, outIndex);
    }
    return nodeAdded;
}

template <class NodeType, class ConnectionType, class DataType>
bool
AudioGraphModel<NodeType, ConnectionType, DataType>::connectNodes(const processGraph::ConnectionPoint& out,
                                                                  const processGraph::ConnectionPoint& in)
{
    return m_graph.addConnection(out, in);
}

template <class NodeType, class ConnectionType, class DataType>
bool
AudioGraphModel<NodeType, ConnectionType, DataType>::disconnectNodes(const processGraph::ConnectionPoint& out,
                                                                     const processGraph::ConnectionPoint& in)
{
    return m_graph.removeConnection(out, in);
}
