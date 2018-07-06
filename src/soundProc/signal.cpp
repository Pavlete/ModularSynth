#include "signal.h"

//#define _USE_MATH_DEFINES

#include <math.h>

namespace  {
    float M_PIF = static_cast<float>(M_PI);
}

Signal::Signal(int samplerate)
  : m_samplerate(samplerate)
  , m_currentFreq(440)
  , m_currentPhase(0)
  , m_phaseJump(2 * M_PIF * m_currentFreq / static_cast<float>(m_samplerate))
  , m_currentWaveFunc([](float) { return 0.0f; })
{}

float
Signal::nextSample( )
{
    float sample = m_currentWaveFunc(m_currentPhase);
    m_currentPhase = fmodf(m_currentPhase + m_phaseJump, 2 * M_PIF);
    return sample;
}

void
Signal::setFrequency(float freq)
{
    if (freq != m_currentFreq) {
        m_currentFreq = freq;
        updatePhaseJump( );
    }
}

void
Signal::setWaveFunction(Signal::WaveFunction func)
{
    m_currentWaveFunc = func;
}

void
Signal::updatePhaseJump( )
{
    m_phaseJump =
      2 * M_PIF * m_currentFreq / static_cast< float >(m_samplerate);
}
