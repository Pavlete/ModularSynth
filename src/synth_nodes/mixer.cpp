#include "mixer.h"

#include "../data_models/nodefactory.h"

namespace
{
REGISTER_FACTORY(Aux, Mixer)
}


void Mixer::updateSettings()
{
	if(m_settingsChanged)
	{        
        m_settingsChanged = false;
        m_input1attenuation = Decibels::decibelsToGain<float>(m_input1Amplitude);
        m_input2attenuation = Decibels::decibelsToGain<float>(m_input2Amplitude);
	}
}

void Mixer::process()
{
	updateSettings();

    auto Input1Buffer = getInputData(Input1BufferIndex);
    auto Input2Buffer = getInputData(Input2BufferIndex);

    auto SignalOutputBuffer = getOutputData(OutputBufferIndex);

    //Processing goes here

    for(int i = 0; i < SignalOutputBuffer->sampleCount(); i++)
    {
        auto input1Right = 0.0f;
        auto input1Left = 0.0f;
        if(Input1Buffer)
        {
            input1Right = Input1Buffer->sample(0, i) * m_input1attenuation;
            input1Left = Input1Buffer->sample(1, i) * m_input1attenuation;
        }

        auto input2Right = 0.0f;
        auto input2Left = 0.0f;
        if(Input2Buffer)
        {
            input2Right = Input2Buffer->sample(0, i) * m_input2attenuation;
            input2Left = Input2Buffer->sample(1, i) * m_input2attenuation;
        }

        SignalOutputBuffer->setSample(0,i, input1Right + input2Right);
        SignalOutputBuffer->setSample(1,i, input1Left + input2Left);
    }
}

//------------------//

Mixer_GUI::Mixer_GUI(const std::shared_ptr<Mixer_Model>& model)
	: JuceAudioNode (model, 2, 1)
    , m_model (model)
    , m_input1Slider ("Level 1",
                      {-40, 0, 1},
                      this,
                      Slider::RotaryHorizontalVerticalDrag,
                      m_model->getInput1amplitude())
    , m_input2Slider ("Level 2",
                      {-40, 0, 1},
                      this,
                      Slider::RotaryHorizontalVerticalDrag,
                      m_model->getInput2amplitude())
{
    using namespace measures;
    setSize(basic_module_width + rotary_width ,
            basic_module_height + 2 * rotary_height + controls_margin );

    addAndMakeVisible(m_input1Slider);
    addAndMakeVisible(m_input2Slider);
}

void Mixer_GUI::setContent(Rectangle<int> &r)
{
    m_input1Slider.setBounds(r.removeFromTop(measures::rotary_height));
    m_input2Slider.setBounds(r.removeFromBottom(measures::rotary_height));
}

void Mixer_GUI::sliderValueChanged(Slider *slider)
{
    if(slider == &m_input1Slider)
    {
        m_model->setInput1amplitude(slider->getValue());
    }
    else if(slider == &m_input2Slider)
    {
        m_model->setInput2amplitude(slider->getValue());
    }
}


