#pragma once

#include "common/uiaudionode.h"
#include "../soundProc/signal.h"

namespace
{
const int SignalInput = 0;
const int SignalOutput= 0;
}


class OutVCA: public JuceAudioNode
{
public:
    OutVCA();

    void process() override;

private:
    float m_amplitude;
};


class OutVCA_GUI : public UIAudioNode
{
public:
    OutVCA_GUI(NodeModel& model);

    void paintContent(Graphics &g) override;

    GraphFactory getModule() override;
};
