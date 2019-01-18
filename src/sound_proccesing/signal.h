#pragma once

#include <functional>
#include <mutex>

class Signal
{
public:
    using WaveFunction = std::function< float(float) >;

    Signal(int samplerate);

    float nextSample( );

    void setFrequency(float freq);

    void setWaveFunction(WaveFunction func);

private:
    const int m_samplerate;

    float m_currentFreq;

    float m_currentPhase;
    float m_phaseJump;
    WaveFunction m_currentWaveFunc;

    void updatePhaseJump( );
};
