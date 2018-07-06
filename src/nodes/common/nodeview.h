#pragma once

#include "nodemodel.h"

template<class NodeType, class ConnectionType>
class NodeView
{
public:
    NodeView(NodeModel<NodeType,ConnectionType>& model)
    {
        model.m_nodeAdded = [&](NodeType* node, int x, int y)
        {
            onNodeAdded(node, x, y);
        };

        model.m_connectionAdded = [&](ConnectionType* conn)
        {
            onConnectionAdded(conn);
        };
    }

protected:
    virtual void onNodeAdded(NodeType* node, int x, int y) = 0;
    virtual void onConnectionAdded(ConnectionType* conn) = 0;
};
