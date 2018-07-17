#pragma once

#include "common/juceaudionode.h"
#include "../soundProc/signal.h"



class OutVCA: public AudioGraphNode
{
public:
    OutVCA();

    void process() override;

private:
    float m_amplitude;
};


class OutVCA_GUI : public JuceAudioNode
{
public:
    OutVCA_GUI(const Node& model);

    void setContent(Rectangle<int> &r) override;

    GraphFactory getModule() override;
};
