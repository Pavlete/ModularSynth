#include "vco.h"

namespace
{

const static String Offset = "offset";
const static float DefaultOffset = 0;

const static String Waveindex = "waveIndex";
const static int DefaultWaveindex = 0;

}

//------------------//

void VCO_Model::Listener::valueTreePropertyChanged(ValueTree &tree, const Identifier &id)
{
	if(id.toString() == Offset)
	{
		offsetChanged(tree.getProperty(id));
	}
	if(id.toString() == Waveindex)
	{
		waveIndexChanged(tree.getProperty(id));
	}
}

VCO_Model::VCO_Model(const ValueTree &tree)
	: Node(tree)
{ }

void VCO_Model::setOffset(float val)
{
	m_tree.setProperty(Offset, val, nullptr);
}

float VCO_Model::getOffset() const
{
	return m_tree.getProperty(Offset, DefaultOffset);
}

void VCO_Model::setWaveindex(int val)
{
	m_tree.setProperty(Waveindex, val, nullptr);
}

int VCO_Model::getWaveindex() const
{
	return m_tree.getProperty(Waveindex, DefaultWaveindex);
}

std::function<std::unique_ptr<AudioNode> ()> VCO_Model::getAudioFactory()
{
	return [&](){return std::make_unique<VCO>(this->shared_from_this());};
}

std::function<std::unique_ptr<JuceAudioNode> ()> VCO_Model::getUIFactory()
{
	return [&](){return std::make_unique<VCO_GUI>(this->shared_from_this());};
}

//------------------//


void VCO::offsetChanged(float offset)
{
	m_offset = offset;
	m_settingsChanged = true;
}

void VCO::waveIndexChanged(int waveIndex)
{
	m_waveIndex = waveIndex;
	m_settingsChanged = true;
}


