/*
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org>
*/

#pragma once

#include "platform.h"
#include "platform_math.h"
#include "platform_graphics.h"
#include "platform_physics.h"

#define PL_MAX_MODEL_BONES      1024
#define PL_MAX_MODEL_MESHES     32
#define PL_MAX_MODEL_FRAMES     512
#define PL_MAX_MODEL_LODS       10

typedef struct PLAnimation {
    char name[64];

    unsigned int num_frames;

    float framerate;
};

typedef struct PLModelBoneWeight {
    unsigned int bone_index;
    float bone_weight;

    unsigned int vertex_index;

    PLVector3 direction;
} PLModelBoneWeight;

// Standard mesh
typedef struct PLModel {
    char name[64];

    unsigned int flags;

    unsigned int num_animations;

    struct {
        PLMesh *meshes;
        unsigned int num_meshes;

        PLModelBoneWeight *bone_weights;
        unsigned int num_bone_weights;

        float distance;
    } lods[PL_MAX_MODEL_LODS];
    unsigned int num_lods;

    PLVector3 origin;

    float radius;
    PLAABB bounds;

    struct {
        unsigned int current_lod;
    } internal;
} PLModel;

// Per-vertex animated mesh.
typedef struct PLModelFrame {
    unsigned int num_meshes;

    PLMesh *meshes[PL_MAX_MODEL_MESHES];

    PLAABB bounds;
} PLModelFrame;

typedef struct PLAnimatedModel {
    unsigned int num_triangles;
    unsigned int num_vertices;
    unsigned int num_frames;

    PLMeshPrimitive primitive;

    PLModelFrame frames[PL_MAX_MODEL_FRAMES];
} PLAnimatedModel;

PL_EXTERN_C

PLModel *plLoadModel(const char *path);

enum {
    PL_SERIALIZE_MODEL_BASE,
    PL_SERIALIZE_MODEL_VERTICES,

    PL_SERIALIZE_MODEL_COMPLETE,

    PL_SERIALIZE_MODEL_END
};

uint8_t *plSerializeModel(PLModel *model, unsigned int type);

///////////////////////////////////////////////////////////////////

void plDeleteModel(PLModel *model);
void plDrawModel(PLModel *model);

void plRegisterModelLoader(const char *ext, PLModel*(*LoadFunction)(const char *path));

void plGenerateModelNormals(PLModel *model);
void plGenerateModelAABB(PLModel *model);
void plGenerateAnimatedModelNormals(PLAnimatedModel *model);

PL_EXTERN_C_END
