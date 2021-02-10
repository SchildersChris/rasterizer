#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <limits.h>

#include "Sources/rasterizer.h"

static void draw(int width, int height, char* frame) {
    printf("\x1b[%dA", height);
    size_t size = sizeof(char);

    for(int y = 0; y < height; y++) {
        fwrite(frame + size * y * width, size, width, stdout);
        printf("%c", '\n');
    }
}

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
    int width = 120;
    int height = 50;

    // Allocate z-buffer on heap
    size_t zBuffSize = width * height * sizeof(float);
    float* zBuffer = malloc(zBuffSize);
    memset(zBuffer, CHAR_MAX, zBuffSize);

    // Allocate raster image on heap
    size_t rasterImageSize = width * height * sizeof(char);
    char* rasterImage = malloc(rasterImageSize);
    memset(rasterImage, ' ', rasterImageSize);

    // Rasterize triangles
    rasterize(vertices, indices, 12, zBuffer, width, height, rasterImage);

    // Draw raster image
    draw(width, height, rasterImage);
    return 0;
}