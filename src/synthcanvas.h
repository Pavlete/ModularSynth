#pragma once

#include "modulemenu.h"
#include "nodes/common/juceaudionode.h"


class SynthCanvas : public Component
                  , public SynthModel::Listener
{
public:
    SynthCanvas(SynthModel synth);

    void paint(Graphics &g) override;
    void mouseDown(const MouseEvent &event) override;

    void nodeAdded(Node mod) override;
    void connectionAdded(Connection connection) override;

    void connectionRemoved(Connection connection) override;

private:
    SynthModel m_synth;
    ModuleMenu m_menu;

    std::vector<std::unique_ptr<JuceAudioNode>> m_nodes;
    std::vector<std::shared_ptr<JuceConnection>> m_connections;

    std::vector<std::unique_ptr<JuceAudioNode>>::iterator findNodeByID(int id);
    std::vector<std::shared_ptr<JuceConnection>>::iterator findConnection(int outID, int outPort);
};
