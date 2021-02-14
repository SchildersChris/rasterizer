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
    // Cube vertices
//    const Vector3 vertices[] =  {
//        {-0.5f, -0.5f, -0.5f},
//        {0.5f, -0.5f, -0.5f},
//        {-0.5f, 0.5f,-0.5f},
//        {0.5f, 0.5f, -0.5f},
//        {-0.5f, 0.5f, -1.5f},
//        {0.5f, 0.5f, -1.5f},
//        {-0.5f, -0.5f, -1.5f},
//        {0.5f, -0.5f, -1.5f},
//    };
//
//    // Cube triangle indices
//    const unsigned int indices[] = {
//        1,0,0, 2,0,0, 3,0,0,
//        3,0,0, 2,0,0, 4,0,0,
//        3,0,0, 4,0,0, 5,0,0,
//        5,0,0, 4,0,0, 6,0,0,
//        5,0,0, 6,0,0, 7,0,0,
//        7,0,0, 6,0,0, 8,0,0,
//        7,0,0, 8,0,0, 1,0,0,
//        1,0,0, 8,0,0, 2,0,0,
//        2,0,0, 8,0,0, 4,0,0,
//        4,0,0, 8,0,0, 6,0,0,
//        7,0,0, 1,0,0, 5,0,0,
//        5,0,0, 1,0,0, 3,0,0
//    };

    void* p_data;
    void* p_buffer;
    size_t file_size;
    objpar_data_t obj_data;

    p_data = openFile("../res/v2.obj", &file_size);
    p_buffer = malloc(objpar_get_size(p_data, file_size));
    objpar((const char*)p_data, file_size, p_buffer, &obj_data);
    free(p_data);

    printf("Vertices Count: %u\n", obj_data.position_count);
    printf("Face Count: %u\n\n", obj_data.face_count);

    // Raster image dimensions
    int width = 2000;
    int height = 2000;

    // Allocate z-buffer on heap
    float* zBuffer = malloc( width * height * sizeof(float));

    // Allocate raster image on heap
    unsigned char* rasterImage = malloc(width * height * sizeof(unsigned char));

    float angle = 0.1f;
    Matrix4x4 transform = {
        { cosf(angle), -sinf(angle), 0, 0 },
        { sinf(angle), cosf(angle), 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 }
    };

    // Rasterize triangles
    rasterize((Vector3*)obj_data.p_positions, obj_data.p_faces, obj_data.face_count, &transform, zBuffer, width, height, rasterImage);

    // Write out result
    stbi_write_jpg("../output.jpg", width, height, 1, rasterImage, width * (int)sizeof(unsigned char));
    return 0;
}