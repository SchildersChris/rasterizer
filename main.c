#include <stdlib.h>
#include <memory.h>
#include <objpar.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define NEAR_CLIPPING 1
#include "src/include/rasterizer.h"

void* openFile(const char* p_file_name, size_t* p_file_size)
{
    FILE* p_file;
    void* p_file_data;
    size_t size;
    int err;

#if defined(_MSC_VER)
    fopen_s(&p_file, p_file_name, "rb");
#else
    p_file = fopen(p_file_name, "rb+");
#endif
    assert(p_file != NULL);
    fseek(p_file, 0L, SEEK_END);
    size = ftell(p_file);
    rewind(p_file);
    p_file_data = malloc(size);
#if defined(_MSC_VER)
    fread_s(p_file_data, size, size, 1, p_file);
#else
    fread(p_file_data, size, 1, p_file);
#endif
    err = ferror(p_file);
    assert(err == 0);
    fclose(p_file);
    *p_file_size = size;
    return p_file_data;
}

int main() {
    void* p_data;
    void* p_buffer;
    size_t file_size;
    objpar_data_t obj_data;

    p_data = openFile("../res/v2.obj", &file_size);
    p_buffer = malloc(objpar_get_size(p_data, file_size));
    objpar((const char*)p_data, file_size, p_buffer, &obj_data);
    free(p_data);

    // Map indices to new format [VertexIndex, TextureIndex, NormalIndex] => [VertexIndex]
    unsigned int* indices = malloc(obj_data.face_count * 3 * sizeof(unsigned int));
    for (int i = 0, j = 0; j < obj_data.face_count * 3 * 3; i++, j+=3) {
        indices[i] = obj_data.p_faces[j];
    }

    printf("Vertices Count: %u\n", obj_data.position_count);
    printf("Face Count: %u\n", obj_data.face_count);

    // Raster image dimensions
    int width = 2000;
    int height = 2000;

    // Allocate z-buffer on heap
    float* zBuffer = malloc( width * height * sizeof(float));

    // Allocate raster image on heap
    unsigned char* rasterImage = malloc(width * height * sizeof(unsigned char));

    float angle = 0;
    Matrix4x4 modelViewProjection = {
        { cosf(angle), 0, sinf(angle), 0 },
        { 0, 1, 0, 0 },
        { -sinf(angle), 0, cosf(angle), 0 },
        { 0, 0, 0, 1 }
    };

    // Rasterize triangles
    rasterize((Vector3*)obj_data.p_positions, indices, obj_data.face_count * 3, &modelViewProjection, zBuffer, rasterImage, width, height);

    // Write out result
    stbi_write_jpg("../output.jpg", width, height, 1, rasterImage, width * (int)sizeof(unsigned char));
    return 0;
}