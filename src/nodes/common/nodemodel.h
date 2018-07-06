#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

template<class NodeType, class ConnectionType>
class NodeView;

template<class NodeType, class ConnectionType>
class NodeModel
{
    using AddedCallback = std::function<void(NodeType*, int x, int y)>;
    using AddedConnection = std::function<void(ConnectionType*)>;

public:
    virtual ~NodeModel() = default;

    virtual int addNode(std::unique_ptr<NodeType> comp,
                        int x, int y);
    virtual int addInitNode(std::unique_ptr<NodeType> comp,
                            int x, int y, int outIndex) = 0;

    virtual std::shared_ptr<ConnectionType> addConnection(std::shared_ptr<ConnectionType>);
    virtual bool removeConnection(const std::shared_ptr<ConnectionType>&);

protected:
    AddedCallback m_nodeAdded = [](NodeType*, int, int){};
    AddedConnection m_connectionAdded = [](ConnectionType*){};

    std::vector<std::unique_ptr<NodeType>> m_nodes;
    std::vector<std::shared_ptr<ConnectionType>> m_connection;

    friend class NodeView<NodeType, ConnectionType>;
};



//------------------------------------------------------------------//



template<class NodeType, class ConnectionType>
int NodeModel<NodeType, ConnectionType>::addNode(std::unique_ptr<NodeType> comp,
                                  int x, int y)
{
    m_nodes.push_back(std::move(comp));
    m_nodeAdded(m_nodes.back().get(), x, y);
}

template<class NodeType, class ConnectionType>
std::shared_ptr<ConnectionType> NodeModel<NodeType, ConnectionType>::addConnection(std::shared_ptr<ConnectionType> con)
{
    m_connection.push_back(con);
    m_connectionAdded(m_connection.back().get());
    return m_connection.back();
}

template<class NodeType, class ConnectionType>
bool NodeModel<NodeType, ConnectionType>::removeConnection(const std::shared_ptr<ConnectionType> &con)
{
    auto connection = std::find(std::begin(m_connection), std::end(m_connection), con);
    if (connection != m_connection.end())
    {
        m_connection.erase(connection);
        return true;
    }
    return false;
}
