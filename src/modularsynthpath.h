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

    std::shared_ptr<Connection> addConnection() override;
    bool removeConnection(const std::shared_ptr<Connection>&) override;

    bool connectNodes(int outputID, int outIndex,
                      int intputID, int inputIndex) override;
    bool disconnectNodes(int outputID, int outIndex,
                         int intputID, int inputIndex) override;

private:
    ObservableAudioGraph m_graph;
    std::vector<std::unique_ptr<UIAudioNode>> m_nodes;
    std::vector<std::shared_ptr<Connection>> m_connection;
};
