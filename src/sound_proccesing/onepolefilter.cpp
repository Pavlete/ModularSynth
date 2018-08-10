#include "onepolefilter.h"

#define _USE_MATH_DEFINES

#include <math.h>

OnePoleFilter::OnePoleFilter( )
  : m_a0(1)
  , m_b1(0)
  , m_memCell(0)
{}

void
OnePoleFilter::setCutFrequency(int freq)
{
    m_b1 = exp(-2.0 * M_PI * freq / 44000);
    m_a0 = 1.0f - m_b1;
}

float
OnePoleFilter::filter(float sample)
{
    return m_memCell = sample * m_a0 + m_memCell * m_b1;
}
