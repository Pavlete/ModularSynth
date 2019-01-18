#include "outvca.h"

namespace
{

const static String Amplitude = "amplitude";
const static float DefaultAmplitude = -20;

}

//------------------//

void OutVCA_Model::Listener::valueTreePropertyChanged(ValueTree &tree, const Identifier &id)
{
    if(id.toString() == Amplitude)
    {
        amplitudeChanged(tree.getProperty(id));
    }
}

OutVCA_Model::OutVCA_Model(const ValueTree &tree)
    : Node(tree)
{ }

void OutVCA_Model::setAmplitude(float val)
{
    m_tree.setProperty(Amplitude, val, nullptr);
}

float OutVCA_Model::getAmplitude() const
{
    return m_tree.getProperty(Amplitude, DefaultAmplitude);
}

std::function<std::unique_ptr<AudioNode> ()> OutVCA_Model::getAudioFactory()
{
    return [&](){return std::make_unique<OutVCA>(this->shared_from_this());};
}

std::function<std::unique_ptr<JuceAudioNode> ()> OutVCA_Model::getUIFactory()
{
    return [&](){return std::make_unique<OutVCA_GUI>(this->shared_from_this());};
}

//------------------//

OutVCA::OutVCA(const std::shared_ptr<OutVCA_Model>& model)
    : AudioNode (1, 1)
    , m_model (model)
    , m_amplitude (m_model->getAmplitude())
    , m_settingsChanged (true)
{
    m_model->addListener(this);
}

void OutVCA::amplitudeChanged(float amplitude)
{
    m_amplitude = amplitude;
    m_settingsChanged = true;
}

std::unique_ptr<AudioNode> OutVCA::clone()
{
    return std::make_unique<OutVCA>(*this);
}


