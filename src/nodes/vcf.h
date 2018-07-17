#pragma once

#include "common/juceaudionode.h"

#include "../soundProc/onepolefilter.h"

class VCF: public AudioGraphNode
{
public:
    VCF();

    void setActive(float freq) override;

    void process() override;

private:
    OnePoleFilter m_filter;
};


class VCF_GUI :public JuceAudioNode
{
public:
    VCF_GUI(const Node& model);

    void setContent(Rectangle<int> &r) override;

    GraphFactory getModule() override;

    TextButton m_button;
};
