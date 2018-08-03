#pragma once

#include "common/juceaudionode.h"
#include "../graph/audionode.h"

#include "../soundProc/onepolefilter.h"

class VCF: public AudioNode
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
    VCF_GUI(SharedNode model);

    void setContent(Rectangle<int> &r) override;

    TextButton m_button;
};
