#pragma once

#include <atomic>

class Glide
{
public:
    Glide( );

    double nextSample( );

    void setGoalFreq(double goalFreq);

    double glidingFactor( ) const;
    void setGlidingFactor(const double glidingFactor);

private:
    std::atomic< double > m_glidingFactor;
    std::atomic< double > m_goalFreq;
    std::atomic< double > m_ramp;
    double m_currentFreq;

    int counter;
};
