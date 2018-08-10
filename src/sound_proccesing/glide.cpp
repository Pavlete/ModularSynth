#include "glide.h"

#include <QDebug>
#include <thread>

Glide::Glide( )
  : m_glidingFactor(10)
  , m_goalFreq(0)
  , m_ramp(0)
  , m_currentFreq(440)
{}

double
Glide::nextSample( )
{
    if ((m_ramp.load( ) > 0) && (m_currentFreq < m_goalFreq.load( )) ||
        (m_ramp.load( ) < 0) && (m_currentFreq > m_goalFreq.load( ))) {
        m_currentFreq += m_ramp.load( );
    }

    return m_currentFreq;
}

void
Glide::setGoalFreq(double goalFreq)
{
    m_ramp.store((goalFreq - m_currentFreq) / (m_glidingFactor.load( ) * 44.1));

    m_goalFreq.store(goalFreq);
}

double
Glide::glidingFactor( ) const
{
    return m_glidingFactor.load( );
}

void
Glide::setGlidingFactor(const double glidingFactor)
{
    m_glidingFactor.store(glidingFactor);
}
