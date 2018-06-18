#ifndef ADSRENVELOPE_H
#define ADSRENVELOPE_H

class ADSREnvelope
{
public:
    struct ADSRSettings
    {
        int m_attackTime;
        int m_decayTime;
        float m_sustainLevel;
        int m_releaseTime;
    };

    ADSREnvelope(int samplerate);

    float getEnvelope( );

    void setParameters(const ADSRSettings& settings);

    void setActive( );
    void setInactive( );

    bool isActive( ) const;

private:
    enum State
    {
        IDLE,
        ATTACK,
        DECAY,
        SUSTAIN,
        RELEASE
    } m_envelopeState;

    const int m_samplerate;
    const float m_attackOvershot;
    const float m_decayOvershot;
    const float m_releaseOvershot;

    ADSRSettings m_currentSettings;

    float m_attackStep;
    float m_attackBase;

    float m_decayStep;
    float m_decayBase;

    float m_releaseStep;
    float m_releaseBase;

    float m_envelopeValue;

    void setAttackParameters(int attackTime);
    void setDecayParameters(int decayTime);
    void setReleaseParameters(int releaseTime);

    static float getStep(int sampleNumber, float overshoot);
};

#endif // ADSRENVELOPE_H
