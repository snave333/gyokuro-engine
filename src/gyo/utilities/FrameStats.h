#ifndef FRAME_STATS_H
#define FRAME_STATS_H

namespace gyo {

struct FrameStats {
    float frame = 0;
    float cpuMs = 0;
    float gpuMs = 0;
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
