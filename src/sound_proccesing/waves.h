#ifndef WAVES_H
#define WAVES_H

#define _USE_MATH_DEFINES

#include <math.h>

namespace waveforms {
static float
square(float phase)
{
    return phase <= M_PI ? -1.0f : 1.0f;
}

static float
triangle(float phase)
{
    return phase < M_PI ? phase / M_PI : 2 + (-phase / M_PI);
}

static float
sawtooth(float phase)
{
    return (phase / M_PI) - 1;
}

static float
sin(float phase)
{
    return sinf(phase);
}
} // namespace waveforms

#endif // WAVES_H
