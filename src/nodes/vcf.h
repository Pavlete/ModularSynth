#pragma once

#include "common/uiaudionode.h"

#include "../soundProc/onepolefilter.h"

class VCF: public JuceAudioNode
{
public:
    VCF();

    void setActive(float freq) override;

    void process() override;

private:
    OnePoleFilter m_filter;
};


class VCF_GUI : public UIAudioNode
{
public:
    VCF_GUI(NodeModel& model);

    virtual void paintContent(Graphics &g) override;

    GraphFactory getModule() override;
};
