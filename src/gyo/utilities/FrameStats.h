#ifndef FRAME_STATS_H
#define FRAME_STATS_H

#include <algorithm>

namespace gyo {

class SmoothedStat {
public:
    // smaller alpha = smoother (slower changes)
    // maxSample: optional clamp for incoming samples (e.g. to ignore crazy spikes)
    explicit SmoothedStat(float alpha = 0.1f, float maxSample = 500.0f)
        : alpha(alpha)
        , maxSample(maxSample)
        , value(0.0f)
        , isInitialized(false) {}

    void PushSample(float sample) {
        // clamp spikes so they don't dominate the average
        if (maxSample > 0.0f) {
            sample = std::min(sample, maxSample);
        }

        if (!isInitialized) {
            value = sample;
            isInitialized = true;
            return;
        }

        // Exponential moving average:
        // newValue = oldValue + alpha * (sample - oldValue)
        value += (sample - value) * alpha;
    }

    float Get() const {
        return value;
    }

    bool HasValue() const {
        return isInitialized;
    }

private:
    float alpha;
    float maxSample;
    float value;
    bool  isInitialized;
};

struct FrameStats {
    SmoothedStat frameMs;
    SmoothedStat cpuMs;
    SmoothedStat gpuMs;
    unsigned int drawCalls = 0;
    unsigned int tris = 0;
    
    float geometryMs = 0;
    float uiMs = 0; // previous frame
    float postProcessMs = 0;

    void Reset() {
        drawCalls = 0;
        tris = 0;
    }
};

} // namespace gyo

#endif // FRAME_STATS_H
