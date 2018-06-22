#pragma once

#include <functional>
#include <JuceHeader.h>

class NodeView;
class Connector;
class Connection;
class UIAudioNode;

class NodeModel
{
    using AddedCallback = std::function<void(juce::Component*, const Point<int>&)>;
public:
    virtual ~NodeModel() = default;

    virtual int addNode(std::unique_ptr<UIAudioNode> comp, const Point<int>& p) = 0;
    virtual int addInitNode(std::unique_ptr<UIAudioNode> comp, const Point<int>& p, int outIndex) = 0;

    virtual std::shared_ptr<Connection> addConnection() = 0;
    virtual bool connectNodes(int outputID, int outIndex,
                              int intputID, int inputIndex) = 0;
    virtual bool removeConnection(const std::shared_ptr<Connection>&) = 0;
    virtual bool disconnectNodes(int outputID, int outIndex,
                                 int intputID, int inputIndex) = 0;

protected:

    AddedCallback m_nodeAdded = [](Component*, Point<int>){};
    AddedCallback m_connectionAdded = [](Component*, Point<int>){};
    friend class NodeView;
};
