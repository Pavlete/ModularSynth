#pragma once

#include <JuceHeader.h>

#include "audiobufferwrapper.h"
#include "nodemodel.h"

#include "../../graph/audioprocessgraph.h"
#include "../../graph/audiographobservable.h"

using JuceAudioNode = processGraph::AudioProcessGraph<AudioBufferWrapper>::AudioNode;
using GraphFactory = AudioGraphObservable<AudioBufferWrapper>::GraphFactory;


class Connector: public Component
{
public:
    Connector(NodeModel& model, int index);

    void mouseDrag(const MouseEvent &event) override;
    void paint(Graphics &g) override;
    void resized() override;

    void setNodeId(int id) { m_nodeId = id; }

    processGraph::ConnectionPoint getPoint();

private:
    NodeModel& m_parentModel;

    int m_index;
    int m_nodeId;
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
