#include "modularsynthpath.h"

ModularSynthPath::ObservableAudioGraph &ModularSynthPath::getObservable()
{
    return m_graph;
}

int ModularSynthPath::addNode(std::unique_ptr<UIAudioNode> comp, const Point<int> &p)
{
    auto nodeId = m_graph.addModule(comp->getModule());
    if(nodeId != processGraph::INVALID_NODE_ID)
    {
        m_nodes.push_back(std::move(comp));
        m_nodeAdded(m_nodes.back().get(), p);
        m_graph.connectModule(nodeId,0, nodeId - 1, 0);
        return nodeId;
    }

    return processGraph::INVALID_NODE_ID;
}

int ModularSynthPath::addInitNode(std::unique_ptr<UIAudioNode> comp,
                                   const Point<int> &p,
                                   int outIndex)
{
    auto nodeAdded = addNode(std::move(comp), p);
    if(nodeAdded != processGraph::INVALID_NODE_ID)
    {
        m_graph.setInitialModule(nodeAdded, outIndex);
    }
    return nodeAdded;
}

bool ModularSynthPath::addConnection(const Connector& in, const Connector& out)
{

}

Connection* ModularSynthPath::startConnection(const Connector& con)
{

}

bool ModularSynthPath::endConnection(const Connector& con)
{

}
