#pragma once

#include "audiobufferwrapper.h"
#include "socket.h"

#include "../../graph/audioprocessgraph.h"

using AudioGraphNode = processGraph::AudioProcessGraph<AudioBufferWrapper>::AudioNode;
using GraphFactory = std::function<std::unique_ptr<AudioGraphNode>()>;

class JuceAudioNode: public Component
{
public:
    JuceAudioNode(Node model,int inputNumber, int outputNumber);

    virtual ~JuceAudioNode() = default;

    virtual void setContent(Rectangle<int> &r) = 0;
    virtual std::string getConnectorName(int );

    virtual GraphFactory getModule() = 0;

    int getNodeID() const;

    void mouseDown(const MouseEvent &event) override;
    void mouseDrag(const MouseEvent &event) override;

    void setConnection(std::shared_ptr<JuceConnection> con, Socket::Direction dir);

private:
    void paint(Graphics &g) override;
    void resized() override;
    void moved() override;
    void parentHierarchyChanged() override;

    std::vector<std::unique_ptr<Socket>> m_inConnectors;
    std::vector<std::unique_ptr<Socket>> m_outConnectors;

    Node m_model;

    ComponentDragger m_dragger;
    OngoingConnection m_ongoing;

};

