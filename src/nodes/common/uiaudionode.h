#pragma once

#include <JuceHeader.h>

#include "audiobufferwrapper.h"
#include "nodemodel.h"

#include "../../graph/audioprocessgraph.h"
#include "../../graph/audiographobservable.h"

using JuceAudioNode = processGraph::AudioProcessGraph<AudioBufferWrapper>::AudioNode;
using GraphFactory = AudioGraphObservable<AudioBufferWrapper>::GraphFactory;

enum class Direction
{
    Input,
    Output
};

class Connector: public Component
{
public:
    Connector(NodeModel& model, uint32_t index, Direction dir);

    void mouseDrag(const MouseEvent &event) override;    
    void mouseDown(const MouseEvent &event) override;
    void mouseUp(const MouseEvent &event) override;

    void paint(Graphics &g) override;

    void setNodeId(int id) { m_nodeId = id; }

    processGraph::ConnectionPoint getPoint() {return {m_nodeId, m_index};}
    Direction getDirection() {return {m_direction};}

    Point<int> getCenterInCanvas();

private:
    NodeModel& m_parentModel;

    std::weak_ptr<Connection> m_currentConnection;

    int m_nodeId;    

    Point<int> m_mouseDownPoint;
    Point<int> mapToCanvas(const Point<int>& p);

    const uint32_t m_index;
    const Direction m_direction;

};

class UIAudioNode: public Component
{
public:
    UIAudioNode(NodeModel&  model,int inputNumber, int outputNumber);

    virtual ~UIAudioNode() = default;

    virtual void paintContent(Graphics &g) = 0;
    virtual GraphFactory getModule() = 0;
    virtual std::string getConnectorName(int ) {return "";}

    void setNodeID(int id);

    void mouseDrag(const MouseEvent &event) override;

private:
    void paint(Graphics &g) override;
    void resized() override;

    std::vector<std::unique_ptr<Connector>> m_inConnectors;
    std::vector<std::unique_ptr<Connector>> m_outConnectors;
};
