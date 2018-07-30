#pragma once

#include "common/juceaudionode.h"

#include "../soundProc/signal.h"

struct VCO_Model
        : public Node
        , public std::enable_shared_from_this<VCO_Model>
{
    class Listener: public Node::Listener
    {
    public:
        virtual void offsetChanged(float offset) = 0;
        virtual void waveIndexChanged(int index) = 0;
    private:
        void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    };

    VCO_Model(const ValueTree&);

    void setOffset(float value);
    float getOffset() const;

    void setWaveindex(int value);
    int getWaveindex() const;

    std::function<std::unique_ptr<AudioGraphNode> ()> getAudioFactory() override;
    std::function<std::unique_ptr<JuceAudioNode> ()> getUIFactory() override;
};

//------------------//

class VCO
        : public AudioGraphNode
        , public VCO_Model::Listener
{
public:
    VCO(const std::shared_ptr<VCO_Model>&);

    void setActive(float freq) override;
    void setInactive() override {m_frequency = 0.0f; }
    bool isActive() override{ return static_cast<int>(m_frequency) != 0; }

    void process() override;
private:
    std::shared_ptr<VCO_Model> m_model;
    std::atomic<float> m_offset;
    std::atomic<int> m_waveIndex;
    std::atomic_bool m_settingsChanged;

    void offsetChanged(float offset) override;
    void waveIndexChanged(int index) override;

    void updateSettings();

    Signal m_signal;
    float m_frequency;
    std::vector<std::function<float(float)>> m_waveforms;
};

//------------------//

class VCO_GUI
        : public JuceAudioNode
        , public Slider::Listener
{
public:
    VCO_GUI(const std::shared_ptr<VCO_Model>&);

    void setContent(Rectangle<int> &r) override;

    void sliderValueChanged(Slider *slider) override;
private:
    std::shared_ptr<VCO_Model> m_model;

    Slider m_offsetSlider;
    Slider m_waveformSlider;
};

