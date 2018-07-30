#include "vcf.h"

namespace
{
//REGISTER_FACTORY(Filters, VCF)
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

    outBuffer->clear();

    for(int i = 0; i < inBuffer->sampleCount(); i++)
    {
        auto nextLeft = m_filter.filter(inBuffer->sample(0,i));
        auto nextRight = m_filter.filter(inBuffer->sample(1,i));
        outBuffer->addSample(0, i, nextLeft);
        outBuffer->addSample(1, i, nextRight);
    }
}


//---------------------------------------------------------------------//


VCF_GUI::VCF_GUI(SharedNode model)
    :JuceAudioNode (model, 1, 1)
{
    addAndMakeVisible(m_button);

    setSize(125, 125);
}

void VCF_GUI::setContent(Rectangle<int> &r)
{
    m_button.setBounds(r);
}
