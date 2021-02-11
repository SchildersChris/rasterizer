#include <stdlib.h>
#include <memory.h>
#include <limits.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "src/rasterizer.h"


int main() {
    // Cube vertices
    const Vector3 vertices[] =  {
        {-0.5f, -0.5f, -0.5f},
        {0.5f, -0.5f, -0.5f},
        {-0.5f, 0.5f,-0.5f},
        {0.5f, 0.5f, -0.5f},
        {-0.5f, 0.5f, -1.5f},
        {0.5f, 0.5f, -1.5f},
        {-0.5f, -0.5f, -1.5f},
        {0.5f, -0.5f, -1.5f},
    };

    // Cube triangle indices
    const int indices[] = {
        1, 2, 3,
        3, 2, 4,
        3, 4, 5,
        5, 4, 6,
        5, 6, 7,
        7, 6, 8,
        7, 8, 1,
        1, 8, 2,
        2, 8, 4,
        4, 8, 6,
        7, 1, 5,
        5, 1, 3
    };

    // Raster image dimensions
    int width = 600;
    int height = 600;
    int channels = 3;

    // Allocate z-buffer on heap
    size_t zBuffSize = width * height * sizeof(float);
    float* zBuffer = malloc(zBuffSize);
    memset(zBuffer, CHAR_MAX, zBuffSize);

    // Allocate raster image on heap
    size_t rasterImageSize = width * height * channels * sizeof(unsigned char);
    unsigned char* rasterImage = malloc(rasterImageSize);
    memset(rasterImage, 0, rasterImageSize);

    // Rasterize triangles
    rasterize(vertices, indices, 12, zBuffer, width, height, rasterImage);

    // Write out result
    stbi_write_jpg("./output.jpg", width, height, 1, rasterImage, width * (int)sizeof(unsigned char));
    return 0;
}