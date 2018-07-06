#include "outvca.h"

namespace
{
const int SignalInput = 0;
const int SignalOutput= 0;
}


OutVCA::OutVCA()
    : AudioGraphNode (1,1)
    , m_amplitude (0.25f)
{}

void OutVCA::process()
{
    auto inBuffer = getInputData(SignalInput);
    auto outBuffer = getOutputData(SignalOutput);
    if(!outBuffer || !inBuffer)
    {
        return;
    }

    for(int i = 0; i < inBuffer->numberOfSamples(); i++)
    {
        auto nextLeftSample = inBuffer->buffer()->getSample(0,i + inBuffer->startSamples()) * m_amplitude;
        auto nextRightSample = inBuffer->buffer()->getSample(1,i + inBuffer->startSamples()) * m_amplitude;
        outBuffer->buffer()->addSample(0, i + outBuffer->startSamples(), nextLeftSample);
        outBuffer->buffer()->addSample(1, i + outBuffer->startSamples(), nextRightSample);
    }
}


OutVCA_GUI::OutVCA_GUI(JuceGraphModel& model)
    :JuceAudioNode (model, 1, 0)
{
    setSize(125, 125);
}

void OutVCA_GUI::setContent(Rectangle<int> &r)
{
}

GraphFactory OutVCA_GUI::getModule()
{
    return [](){return std::make_unique<OutVCA>();};
}
