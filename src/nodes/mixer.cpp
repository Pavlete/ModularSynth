#include "mixer.h"

namespace
{

REGISTER_FACTORY(Aux, Mixer)

const static String ModuleName = "Mixer";

const static int Input1 = 0;
const static int Input2 = 1;

const static int SignalOutput = 0;

const static String Input1amplitude = "input1Amplitude";
const static int DefaultInput1amplitude = 0;

const static String Input2amplitude = "input2Amplitude";
const static int DefaultInput2amplitude = 0;

}

//------------------//

void Mixer_Model::Listener::valueTreePropertyChanged(ValueTree &tree, const Identifier &id)
{
	if(id.toString() == Input1amplitude)
	{
		input1AmplitudeChanged(tree.getProperty(id));
	}
	if(id.toString() == Input2amplitude)
	{
		input2AmplitudeChanged(tree.getProperty(id));
	}
}

Mixer_Model::Mixer_Model(int x, int y)
	: Node(x, y, ModuleName)
{ }

Mixer_Model::Mixer_Model(const ValueTree &tree)
	: Node(tree)
{ }

void Mixer_Model::setInput1amplitude(int val)
{
	m_tree.setProperty(Input1amplitude, val, nullptr);
}

int Mixer_Model::getInput1amplitude() const
{
	return m_tree.getProperty(Input1amplitude, DefaultInput1amplitude);
}

void Mixer_Model::setInput2amplitude(int val)
{
	m_tree.setProperty(Input2amplitude, val, nullptr);
}

int Mixer_Model::getInput2amplitude() const
{
	return m_tree.getProperty(Input2amplitude, DefaultInput2amplitude);
}

std::function<std::unique_ptr<AudioGraphNode> ()> Mixer_Model::getAudioFactory()
{
	return [&](){return std::make_unique<Mixer>(this->shared_from_this());};
}

std::function<std::unique_ptr<JuceAudioNode> ()> Mixer_Model::getUIFactory()
{
	return [&](){return std::make_unique<Mixer_GUI>(this->shared_from_this());};
}

//------------------//

Mixer::Mixer(const std::shared_ptr<Mixer_Model>& model)
	: AudioGraphNode (2, 1)
	, m_model (model)
	, m_input1Amplitude (m_model->getInput1amplitude())
	, m_input2Amplitude (m_model->getInput2amplitude())
    , m_settingsChanged (true)
{
	m_model->addListener(this);
}

void Mixer::updateSettings()
{
	if(m_settingsChanged)
	{        
        m_input1attenuation = Decibels::decibelsToGain<float>(m_input1Amplitude.load());
        m_input2attenuation = Decibels::decibelsToGain<float>(m_input2Amplitude.load());
		m_settingsChanged = false;
	}
}

void Mixer::input1AmplitudeChanged(int input1Amplitude)
{
	m_input1Amplitude = input1Amplitude;
	m_settingsChanged = true;
}

void Mixer::input2AmplitudeChanged(int input2Amplitude)
{
	m_input2Amplitude = input2Amplitude;
	m_settingsChanged = true;
}

void Mixer::process()
{
	updateSettings();

	auto Input1Buffer = getInputData(Input1);
	auto Input2Buffer = getInputData(Input2);

	auto SignalOutputBuffer = getOutputData(SignalOutput);

    if(!SignalOutputBuffer)
	{
		return;
	}

    SignalOutputBuffer->buffer()->clear();

	//Processing goes here

    for(int i = 0; i < SignalOutputBuffer->numberOfSamples(); i++)
    {
        auto input1Right = 0.0f;
        auto input1Left = 0.0f;
        if(Input1Buffer)
        {
            input1Right = Input1Buffer->buffer()->getSample(0, i + SignalOutputBuffer->startSamples()) * m_input1attenuation;
            input1Left = Input1Buffer->buffer()->getSample(1, i + SignalOutputBuffer->startSamples()) * m_input1attenuation;
        }

        auto input2Right = 0.0f;
        auto input2Left = 0.0f;
        if(Input2Buffer)
        {
            input2Right = Input2Buffer->buffer()->getSample(0, i + SignalOutputBuffer->startSamples()) * m_input2attenuation;
            input2Left = Input2Buffer->buffer()->getSample(1, i + SignalOutputBuffer->startSamples()) * m_input2attenuation;
        }

        SignalOutputBuffer->buffer()->addSample(0,i + SignalOutputBuffer->startSamples(), input1Right + input2Right);
        SignalOutputBuffer->buffer()->addSample(1,i + SignalOutputBuffer->startSamples(), input1Left + input2Left);
    }
}

//------------------//

Mixer_GUI::Mixer_GUI(const std::shared_ptr<Mixer_Model>& model)
	: JuceAudioNode (model, 2, 1)
    , m_model (model)
{
    setSize(300, 125);

    addAndMakeVisible(m_input1Slider);
    m_input1Slider.addListener(this);
    m_input1Slider.setRange(-23, 0, 1);
    m_input1Slider.setValue(m_model->getInput1amplitude());

    addAndMakeVisible(m_input2Slider);
    m_input2Slider.addListener(this);
    m_input2Slider.setRange(-23, 0, 1);
    m_input2Slider.setValue(m_model->getInput2amplitude());
}

void Mixer_GUI::setContent(Rectangle<int> &r)
{
    m_input1Slider.setBounds(r.removeFromTop(getHeight()/2));
    m_input2Slider.setBounds(r);
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


