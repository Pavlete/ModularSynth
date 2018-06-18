#include "adsrenvelope.h"

#include <math.h>

ADSREnvelope::ADSREnvelope(int samplerate)
  : m_envelopeState(IDLE)
  , m_samplerate(samplerate)
  , m_attackOvershot(0.3f)
  , m_decayOvershot(0.01f)
  , m_releaseOvershot(0.01f)
  , m_envelopeValue(0.0f)
{}

float
ADSREnvelope::getEnvelope( )
{
    switch (m_envelopeState) {
        case ATTACK:
            m_envelopeValue = m_attackBase + m_attackStep * m_envelopeValue;
            if (m_envelopeValue >= 1.0f) {
                m_envelopeState = DECAY;
                m_envelopeValue = 1.0f;
            }
            break;
        case DECAY:
            m_envelopeValue = m_decayBase + m_decayStep * m_envelopeValue;
            if (m_envelopeValue <= m_currentSettings.m_sustainLevel) {
                m_envelopeState = SUSTAIN;
                m_envelopeValue = m_currentSettings.m_sustainLevel;
            }
            break;
        case RELEASE:
            m_envelopeValue = m_releaseBase + m_releaseStep * m_envelopeValue;
            if (m_envelopeValue <= 0.0f) {
                m_envelopeState = IDLE;
                m_envelopeValue = 0.0;
            }
            break;
        case SUSTAIN:
        case IDLE:
        default:
            break;
    }
    return m_envelopeValue;
}

void
ADSREnvelope::setParameters(const ADSRSettings& settings)
{
    if (m_currentSettings.m_sustainLevel != settings.m_sustainLevel) {
        m_currentSettings.m_sustainLevel = settings.m_sustainLevel;
        setDecayParameters(settings.m_decayTime);
    }

    if (m_currentSettings.m_attackTime != settings.m_attackTime) {
        setAttackParameters(settings.m_attackTime);
    }

    if (m_currentSettings.m_decayTime != settings.m_decayTime) {
        setDecayParameters(settings.m_decayTime);
    }

    if (m_currentSettings.m_releaseTime != settings.m_releaseTime) {
        setReleaseParameters(settings.m_releaseTime);
    }
}

void
ADSREnvelope::setActive( )
{
    m_envelopeValue = 0.0f;
    m_envelopeState = State::ATTACK;
}

void
ADSREnvelope::setInactive( )
{
    m_envelopeState = State::RELEASE;
    m_envelopeValue = m_currentSettings.m_sustainLevel;
}

bool
ADSREnvelope::isActive( ) const
{
    return m_envelopeState != State::IDLE;
}

void
ADSREnvelope::setAttackParameters(int attackTime)
{
    m_attackStep = getStep(m_samplerate * attackTime / 1000, m_attackOvershot);
    m_attackBase = (1.0f + m_attackOvershot) * (1.0f - m_attackStep);
    m_currentSettings.m_attackTime = attackTime;
}

void
ADSREnvelope::setDecayParameters(int decayTime)
{
    m_decayStep = getStep(m_samplerate * decayTime / 1000, m_decayOvershot);
    m_decayBase = (m_currentSettings.m_sustainLevel - m_decayOvershot) * (1.0f - m_decayStep);
    m_currentSettings.m_decayTime = decayTime;
}

void
ADSREnvelope::setReleaseParameters(int releaseTime)
{
    m_releaseStep = getStep(m_samplerate * releaseTime / 1000, m_releaseOvershot);
    m_releaseBase = (-m_releaseOvershot) * (1.0f - m_releaseStep);
    m_currentSettings.m_releaseTime = releaseTime;
}

float
ADSREnvelope::getStep(int sampleNumber, float overshoot)
{
    return (sampleNumber <= 0) ? 0.0f : exp(-log((1.0f + overshoot) / overshoot) / sampleNumber);
}
