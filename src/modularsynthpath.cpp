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
        m_nodes.back()->setNodeID(nodeId);
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

std::shared_ptr<Connection> ModularSynthPath::addConnection()
{
    m_connection.emplace_back(std::make_shared<Connection>(*this));
    m_connectionAdded(m_connection.back().get(), {0,0});
    return m_connection.back();
}

bool ModularSynthPath::removeConnection(const std::shared_ptr<Connection> &con)
{
    auto connection = std::find(std::begin(m_connection), std::end(m_connection), con);
    if (connection != m_connection.end())
    {
        m_connection.erase(connection);
        return true;
    }
    return false;
}

bool ModularSynthPath::connectNodes(int outputID, int outIndex, int intputID, int inputIndex)
{
    return m_graph.addConnection(outputID, outIndex, intputID, inputIndex);
}

bool ModularSynthPath::disconnectNodes(int outputID, int outIndex, int intputID, int inputIndex)
{
    return m_graph.removeConnection(outputID, outIndex, intputID, inputIndex);
}
