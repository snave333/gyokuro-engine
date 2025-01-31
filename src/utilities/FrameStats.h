#ifndef FRAME_STATS_H
#define FRAME_STATS_H

namespace gyo {

struct FrameStats {
    float fps = 0;
    
    float geometryMs = 0;
    float vfcMs = 0;
    float opaqueMs = 0;
    float alphaMs = 0;

    unsigned int drawCalls = 0;
    unsigned int tris = 0;

    void Reset() {
        geometryMs = 0;
        geometryMs = 0;
        vfcMs = 0;
        opaqueMs = 0;
        alphaMs = 0;
        drawCalls = 0;
        tris = 0;
    }
};

} // namespace gyo

#endif // FRAME_STATS_H
