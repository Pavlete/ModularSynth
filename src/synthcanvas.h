#pragma once

#include "modulemenu.h"
#include "nodes/common/nodemodel.h"
#include "nodes/common/nodeview.h"


class SynthCanvas : public NodeView
{
public:
    SynthCanvas(NodeModel&);

    virtual void paint(Graphics &g) override;

    void mouseDown(const MouseEvent &event) override;

private:
    NodeModel& m_synthPath;
    ModuleMenu m_menu;
};
