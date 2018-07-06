#pragma once

#include "audiobufferwrapper.h"
#include "socket.h"

#include "../../graph/audioprocessgraph.h"

using AudioGraphNode = processGraph::AudioProcessGraph<AudioBufferWrapper>::AudioNode;
using GraphFactory = std::function<std::unique_ptr<AudioGraphNode>()>;

class JuceAudioNode: public Component
{
public:
    JuceAudioNode(JuceGraphModel& model,int inputNumber, int outputNumber);

    virtual ~JuceAudioNode() = default;

    virtual void setContent(Rectangle<int> &r) = 0;
    virtual std::string getConnectorName(int ) {return "";}

    virtual GraphFactory getModule() = 0;

    void setNodeID(int id);

    void mouseDown(const MouseEvent &event) override;
    void mouseDrag(const MouseEvent &event) override;

private:
    void paint(Graphics &g) override;
    void resized() override;

    std::vector<std::unique_ptr<Socket>> m_inConnectors;
    std::vector<std::unique_ptr<Socket>> m_outConnectors;

    ComponentDragger m_dragger;
};

