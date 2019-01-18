#include "outvca.h"

#include "../data_models/nodefactory.h"

namespace
{
REGISTER_FACTORY( , OutVCA)
}


void OutVCA::updateSettings()
{
    if(m_settingsChanged)
    {
        m_settingsChanged = false;
        m_amplitudeSetting = Decibels::decibelsToGain<float>(m_amplitude);
    }
}

void OutVCA::process()
{
    updateSettings();

    auto inBuffer = getInputData(InputBufferIndex);
    auto outBuffer = getOutputData(OutputBufferIndex);

    if(!outBuffer || !inBuffer)
    {
        return;
    }

    for(int i = 0; i < inBuffer->sampleCount(); i++)
    {
        auto nextLeftSample = inBuffer->sample(0,i) * m_amplitudeSetting;
        auto nextRightSample = inBuffer->sample(1,i) * m_amplitudeSetting;
        outBuffer->setSample(0, i, nextLeftSample);
        outBuffer->setSample(1, i, nextRightSample);
    }
}


//----------------------------------------------------------------------------------------//


OutVCA_GUI::OutVCA_GUI(const std::shared_ptr<OutVCA_Model>& model)
    : JuceAudioNode (model, 1, 0)
    , m_model (model)
    , m_volumeSlider ("dBs",
                      {-40, 0, 1},
                      this,
                      Slider::RotaryHorizontalVerticalDrag,
                      m_model->getAmplitude())
{
    using namespace measures;
    setSize(basic_module_width + rotary_width,
            basic_module_height + rotary_height );

    addAndMakeVisible(m_volumeSlider);
}

void OutVCA_GUI::setContent(Rectangle<int> &r)
{
    m_volumeSlider.setBounds(r);
}

void OutVCA_GUI::sliderValueChanged(Slider *slider)
{
    if(slider == &m_volumeSlider)
    {
        m_model->setAmplitude(slider->getValue());
    }
}
