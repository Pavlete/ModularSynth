#ifndef VCO_H
#define VCO_H

#include "common/uiaudionode.h"

#include "../soundProc/signal.h"

class VCO : public JuceAudioNode
{
public:
    VCO();

    void setActive(float freq) override;

    void setInactive() override { m_active = false; }

    bool isActive() override{ return m_active; }

    void process() override;

private:
    Signal m_signal;
    bool m_active;
};


class VCO_GUI : public UIAudioNode
{
public:
    VCO_GUI(NodeModel& model);

    virtual void paintContent(Graphics &g) override;

    GraphFactory getModule() override;
};

#endif // VCO_H
