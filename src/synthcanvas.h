#pragma once

#include "modulemenu.h"
#include "nodes/common/juceaudionode.h"
#include "nodes/common/nodeview.h"


class SynthCanvas : public NodeView<JuceAudioNode, JuceConnection>
                  , public Component
{
public:
    SynthCanvas(JuceGraphModel& model);

    void paint(Graphics &g) override;

    void mouseDown(const MouseEvent &event) override;

    void onNodeAdded(JuceAudioNode *node, int x, int y) override;
    void onConnectionAdded(JuceConnection *conn) override;

private:
    ModuleMenu m_menu;
};
