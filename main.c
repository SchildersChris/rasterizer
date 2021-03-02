#include <stdlib.h>
#include <memory.h>
#include <objpar.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

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

    // Map indices to new format, from [VertexIndex, TextureIndex, NormalIndex] => [VertexIndex]
    unsigned int* indices = malloc(obj_data.face_count * 3 * sizeof(unsigned int));
    for (int i = 0, j = 0; j < obj_data.face_count * 3 * 3; i++, j+=3) {
        indices[i] = obj_data.p_faces[j];
    }

    printf("Vertices Count: %u\n", obj_data.position_count);
    printf("Face Count: %u\n", obj_data.face_count);

    // Raster image dimensions
    int width = 1920;
    int height = 1080;

    int size = width * height;

    // Allocate z-buffer on heap
    float* zBuffer = malloc(size * sizeof(float));

    // Allocate raster image on heap
    unsigned char* frameBuffer = malloc(size * sizeof(unsigned char));

    // Define transformation matrix
    float angle = 0;
    Matrix4x4 modelViewProjection = {
        { cosf(angle), 0, sinf(angle), 0 },
        { 0, 1, 0, 0 },
        { -sinf(angle), 0, cosf(angle), 0 },
        { 0, 0, 0, 1 }
    };

    unsigned char backgroundColor = 0;

    // Rasterize triangles
    rasterize((Vector3*)obj_data.p_positions, indices, obj_data.face_count * 3, &modelViewProjection, zBuffer, frameBuffer, backgroundColor, width, height);

    // Convert zBuffer to image
    unsigned char* zBufferImage = malloc(size * sizeof(unsigned char*));
    for (int i = 0; i < size; ++i) {
        zBufferImage[i] = (unsigned char)(zBuffer[i]);
    }

    stbi_write_jpg("../output.jpg", width, height, 1, frameBuffer, width * (int)sizeof(unsigned char));
    stbi_write_jpg("../zBuffer.jpg", width, height, 1, zBufferImage, width * (int)sizeof(float));

    free(frameBuffer);
    free(zBufferImage);
    free(zBuffer);
    free(p_buffer);
    return 0;
}