#pragma once

#include <atomic>

#include "../UI/common/juceaudionode.h"
#include "../UI/common/customslider.h"
#include "../proccess_graph/audionode.h"
#include "../sound_proccesing/signal.h"
#include "../sound_proccesing/waves.h"

struct VCO_Model
    : public Node
    , public std::enable_shared_from_this<VCO_Model>
{
    class Listener : public Node::Listener
    {
    public:
        virtual void offsetChanged(float offset) = 0;
        virtual void waveIndexChanged(int waveIndex) = 0;
    private:
        void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    };

    VCO_Model(const ValueTree&);

    void setOffset(float value);
    float getOffset() const;

    void setWaveindex(int value);
    int getWaveindex() const;

    std::function<std::unique_ptr<AudioNode> ()> getAudioFactory() override;
    std::function<std::unique_ptr<JuceAudioNode> ()> getUIFactory() override;
};

//------------------//

class VCO
    : public AudioNode
    , public VCO_Model::Listener
{
public:
    VCO(const std::shared_ptr<VCO_Model>&);
    VCO(const VCO&);
    virtual ~VCO();

    virtual void setActive(float freq, float velocity) override;
    virtual void setInactive() override {m_frequency = 0.0f; }
    virtual bool isActive() override{ return static_cast<int>(m_frequency) != 0; }

    virtual void process() override;
private:
    std::shared_ptr<VCO_Model> m_model;

    std::atomic<float> m_offset;
    std::atomic_int m_waveIndex;
    std::atomic_bool m_settingsChanged;

    void offsetChanged(float offset) override;
    void waveIndexChanged(int waveIndex) override;

    void updateSettings();

    //Place AND initialize here your own variables
    Signal m_signal = Signal(44100);
    float m_frequency = 0.0f;
    float m_velocity = 0.0f;
    std::vector<std::function<float(float)>> m_waveforms = {waveforms::sin,
            waveforms::square,
            waveforms::sawtooth,
            waveforms::triangle};

    std::unique_ptr<AudioNode> clone() override;
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

    CustomSlider m_offsetSlider;
    CustomSlider m_waveformSlider;

    Style m_style;
};
