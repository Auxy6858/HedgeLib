#ifndef HL_HH_MATERIAL_H_INCLUDED
#define HL_HH_MATERIAL_H_INCLUDED
#include "hl_material.h"
#include "../textures/hl_hh_texture.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct HlHHMaterialParameter
{
    /** @brief Always 2 or 0? */
    HlU8 flag1;
    /** @brief Always 0? */
    HlU8 flag2;
    /** @brief How many values are in the array pointed to by valuesOffset. */
    HlU8 valueCount;
    /** @brief Always 0? */
    HlU8 flag4;
    HL_OFF32_STR nameOffset;
    HL_OFF32(void) valuesOffset;
}
HlHHMaterialParameter;

HL_STATIC_ASSERT_SIZE(HlHHMaterialParameter, 12);

/* Thanks to Skyth for helping crack alphaThreshold and int4/bool4 param types! */

typedef struct HlHHMaterialV1
{
    HL_OFF32_STR shaderNameOffset;
    HL_OFF32_STR subShaderNameOffset;
    HL_OFF32_STR texsetNameOffset;
    HlU32 reserved1;
    HlU8 alphaThreshold;
    HlU8 noBackfaceCulling;
    HlU8 useAdditiveBlending;
    HlU8 unknownFlag1;
    HlU8 vec4ParamCount;
    HlU8 int4ParamCount;
    HlU8 bool4ParamCount;
    HlU8 reserved2;
    HL_OFF32(HL_OFF32(HlHHMaterialParameter)) vec4ParamsOffset;
    HL_OFF32(HL_OFF32(HlHHMaterialParameter)) int4ParamsOffset;
    HL_OFF32(HL_OFF32(HlHHMaterialParameter)) bool4ParamsOffset;
}
HlHHMaterialV1;

HL_STATIC_ASSERT_SIZE(HlHHMaterialV1, 0x24);

typedef struct HlHHMaterialV3
{
    HL_OFF32_STR shaderNameOffset;
    HL_OFF32_STR subShaderNameOffset;
    HL_OFF32(HL_OFF32_STR) hhTextureNamesOffset;
    HL_OFF32(HL_OFF32(HlHHTextureV1)) texturesOffset;
    HlU8 alphaThreshold;
    HlU8 noBackfaceCulling;
    HlU8 useAdditiveBlending;
    HlU8 unknownFlag1;
    HlU8 vec4ParamCount;
    HlU8 int4ParamCount;
    HlU8 bool4ParamCount;
    HlU8 textureCount;
    HL_OFF32(HL_OFF32(HlHHMaterialParameter)) vec4ParamsOffset;
    HL_OFF32(HL_OFF32(HlHHMaterialParameter)) int4ParamsOffset;
    HL_OFF32(HL_OFF32(HlHHMaterialParameter)) bool4ParamsOffset;
}
HlHHMaterialV3;

HL_STATIC_ASSERT_SIZE(HlHHMaterialV3, 0x24);

HL_API void hlHHMaterialParameterSwap(HlHHMaterialParameter* param, HlBool swapOffsets);
HL_API void hlHHMaterialV1Swap(HlHHMaterialV1* mat, HlBool swapOffsets);
HL_API void hlHHMaterialV3Swap(HlHHMaterialV3* mat, HlBool swapOffsets);

HL_API void hlHHMaterialV1Fix(HlHHMaterialV1* mat);
HL_API void hlHHMaterialV3Fix(HlHHMaterialV3* mat);

HL_API HlResult hlHHMaterialV1Parse(const HlHHMaterialV1* HL_RESTRICT hhMat,
    const char* HL_RESTRICT name, HlMaterial* HL_RESTRICT * HL_RESTRICT hlMat);

HL_API HlResult hlHHMaterialV3Parse(const HlHHMaterialV3* HL_RESTRICT hhMat,
    const char* HL_RESTRICT name, HlMaterial* HL_RESTRICT * HL_RESTRICT hlMat);

HL_API HlResult hlHHMaterialRead(void* HL_RESTRICT rawData,
    const char* HL_RESTRICT name, HlMaterial* HL_RESTRICT * HL_RESTRICT hlMat);

#ifdef __cplusplus
}
#endif
#endif
