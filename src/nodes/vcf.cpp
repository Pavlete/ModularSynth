#include "vcf.h"

namespace
{
const int SignalInput = 0;
const int SignalOutput= 0;
}

VCF::VCF() : AudioGraphNode (1,1)
{}

void VCF::setActive(float freq)
{
   m_filter.setCutFrequency(freq * 50.0f);
}

void VCF::process()
{
    auto inBuffer = getInputData(SignalInput);
    auto outBuffer = getOutputData(SignalOutput);
    if(!outBuffer || !inBuffer)
    {
        return;
    }

    outBuffer->buffer()->clear();

    for(int i = 0; i < inBuffer->numberOfSamples(); i++)
    {
        auto next = m_filter.filter(inBuffer->buffer()->getSample(0,i + outBuffer->startSamples()));
        outBuffer->buffer()->addSample(0, i + inBuffer->startSamples(), next);
        outBuffer->buffer()->addSample(1, i + inBuffer->startSamples(), next);
    }
}


//---------------------------------------------------------------------//


VCF_GUI::VCF_GUI(JuceGraphModel &model)
    :JuceAudioNode (model, 1, 1)
{
    addAndMakeVisible(m_button);

    setSize(125, 125);
}

void VCF_GUI::setContent(Rectangle<int> &r)
{
    m_button.setBounds(r);
}

GraphFactory VCF_GUI::getModule()
{
    return [](){return std::make_unique<VCF>();};
}
