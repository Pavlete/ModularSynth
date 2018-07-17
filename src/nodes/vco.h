#pragma once

#include "common/juceaudionode.h"
#include "common/settingsobserver.h"

#include "../soundProc/signal.h"
#include "../soundProc/waves.h"

struct VCOSettings
{
    float pitchOfset = 1;
    int signalWave = 0;
};

class VCO : public AudioGraphNode
          , public SettingsObserver<VCOSettings>
{
public:
    VCO(SettingsSubject<VCOSettings>& subject);

    void setActive(float freq) override;

    void setInactive() override {m_frequency = 0.0f; }

    bool isActive() override{ return static_cast<int>(m_frequency) != 0; }

    void process() override;    

private:
    Signal m_signal;
    float m_frequency;
    std::vector<std::function<float(float)>> m_waveforms;

    void updateSettings();
};



//-----------------------------------------------//



class VCO_GUI : public JuceAudioNode
              , public Slider::Listener
              , public SettingsSubject<VCOSettings>
{
public:
    VCO_GUI(Node& model);

    void setContent(Rectangle<int> &r) override;

    GraphFactory getModule() override;

    void sliderValueChanged(Slider *slider) override;

private:
    Slider m_offsetSlider;
    Slider m_waveformSlider;
};

