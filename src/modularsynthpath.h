#pragma once

#include "graph/audiographobservable.h"
#include "nodes/common/connection.h"
#include "nodes/common/nodemodel.h"
#include "nodes/common/uiaudionode.h"


class ModularSynthPath : public NodeModel
{
public:
    using ObservableAudioGraph = AudioGraphObservable<AudioBufferWrapper>;

    ObservableAudioGraph& getObservable();

    int addNode(std::unique_ptr<UIAudioNode> comp, const Point<int> &p) override;
    int addInitNode(std::unique_ptr<UIAudioNode> comp, const Point<int> &p, int outIndex) override;

    bool addConnection(const Connector& in, const Connector& out) override;

    Connection* startConnection(const Connector& con) override;
    bool endConnection(const Connector& con) override;

private:
    ObservableAudioGraph m_graph;
    std::vector<std::unique_ptr<UIAudioNode>> m_nodes;
    std::vector<std::unique_ptr<Connection>> m_connection;
    Connection* m_ongoingConnection;
};
