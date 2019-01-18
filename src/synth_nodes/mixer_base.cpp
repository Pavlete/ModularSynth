#include "mixer.h"

namespace
{

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

std::function<std::unique_ptr<AudioNode> ()> Mixer_Model::getAudioFactory()
{
	return [&](){return std::make_unique<Mixer>(this->shared_from_this());};
}

std::function<std::unique_ptr<JuceAudioNode> ()> Mixer_Model::getUIFactory()
{
	return [&](){return std::make_unique<Mixer_GUI>(this->shared_from_this());};
}

//------------------//

Mixer::Mixer(const std::shared_ptr<Mixer_Model>& model)
	: AudioNode (2, 1)
	, m_model (model)
	, m_input1Amplitude (m_model->getInput1amplitude())
	, m_input2Amplitude (m_model->getInput2amplitude())
	, m_settingsChanged (true)
{
	m_model->addListener(this);
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

std::unique_ptr<AudioNode> Mixer::clone()
{
    return std::make_unique<Mixer>(*this);
}


