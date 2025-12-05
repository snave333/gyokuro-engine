#ifndef TEXTURE_DEFINES_H
#define TEXTURE_DEFINES_H

#include <cstdint>

namespace gyo {

#define TEX_NORMAL              "TEX_NORMAL"
#define TEX_EMISSIVE            "TEX_EMISSIVE"

// Phong
#define TEX_DIFFUSE             "TEX_DIFFUSE"
#define TEX_SPECULAR            "TEX_SPECULAR"

// PBR
#define TEX_ALBEDO              "TEX_ALBEDO"
#define TEX_METALLIC            "TEX_METALLIC"
#define TEX_ROUGHNESS           "TEX_ROUGHNESS"
#define TEX_METALLIC_ROUGHNESS  "TEX_METALLIC_ROUGHNESS"
#define TEX_AO                  "TEX_AO"

// bit mask

#define TEX_MASK_NORMAL             1ULL << 1
#define TEX_MASK_EMISSIVE           1ULL << 2
#define TEX_MASK_DIFFUSE            1ULL << 3
#define TEX_MASK_SPECULAR           1ULL << 4
#define TEX_MASK_ALBEDO             1ULL << 5
#define TEX_MASK_METALLIC           1ULL << 6
#define TEX_MASK_ROUGHNESS          1ULL << 7
#define TEX_MASK_METALLIC_ROUGHNESS 1ULL << 8
#define TEX_MASK_AO                 1ULL << 9

} // namespace gyo

#endif // TEXTURE_DEFINES_H
