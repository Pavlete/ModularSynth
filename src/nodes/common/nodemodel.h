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

    virtual bool addConnection(const Connector& in, const Connector& out) = 0;
    virtual Connection* startConnection(const Connector& con) = 0;
    virtual bool endConnection(const Connector& con) = 0;

protected:

    AddedCallback m_nodeAdded = [](Component*, Point<int>){};
    friend class NodeView;
};
