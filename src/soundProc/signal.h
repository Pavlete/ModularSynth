#pragma once

#include <mutex>

class Signal
{
public:
    using WaveFunction = std::function< float(float) >;

    Signal(int samplerate);

    float nextSample( );

    void setFrequency(int freq);

    void setWaveFunction(WaveFunction func);

private:
    const int m_samplerate;

    int m_currentFreq;

    float m_currentPhase;
    float m_phaseJump;
    WaveFunction m_currentWaveFunc;

    void updatePhaseJump( );
};
