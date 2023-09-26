//
// Created by Aashiq Shaikh on 8/10/23.
//
// TODO: fix the red/blue mixup
// TODO: make char array a pixel struct array
// TODO: convert pixel struct array to a char array in save()
// TODO: add a get() method


#ifndef CPU_RASTERIZER_BITMAP_H
#define CPU_RASTERIZER_BITMAP_H

#include <cstdlib>
#include <cstdio>
#include <limits>

struct pixel
{
    unsigned char p[3];

    pixel(unsigned char r, unsigned char g, unsigned char b) {
        p[0] = r;
        p[1] = g;
        p[2] = b;
    }

    pixel() {
        p[0] = 255;
        p[1] = 255;
        p[2] = 255;
    }

    unsigned char r() { return p[0]; }
    unsigned char g() { return p[1]; }
    unsigned char b() { return p[2]; }

    inline static pixel white() { return {255, 255, 255}; }
    inline static pixel red() { return {255, 0, 0}; }
    inline static pixel blue() { return {0, 0, 255}; }
    inline static pixel green() { return {0, 255, 0}; }

};

class bitmap {
public :
    bitmap(int width, int height) : height(height), width(width){
        auto unpadded_width = (width * 3);
        padded_width = unpadded_width % 4 == 0 ? unpadded_width : unpadded_width - unpadded_width % 4 + 4;

        size = height * padded_width * 3;
        pixels = (char *) malloc(size * sizeof(char));

        for (int i = 0; i < size; i++) pixels[i] = 0;
    }

    void set(int x, int y, unsigned char r, unsigned char g, unsigned char b)
    {
        auto index = y * padded_width + x * 3;
        pixels[index] = r;
        pixels[index+1] = g;
        pixels[index+2] = b;
    }

    void set(int x, int y, pixel p)
    {
        auto index = y * padded_width + x * 3;
        pixels[index] = p.r();
        pixels[index+1] = p.g();
        pixels[index+2] = p.b();

    }

    void save(char *filename)
    {
        char tag[] = { 'B', 'M' };
        int header[] = {
                0, 0, 0x36, 0x28, width, height, 0x180001,
                0, 0, 0x002e23, 0x002e23, 0, 0
        };
        header[0] = sizeof(tag) + sizeof(header) + size;
        FILE *fp = fopen(filename, "w+");
        fwrite(&tag, sizeof(tag), 1, fp);
        fwrite(&header, sizeof(header), 1, fp);
        fwrite(pixels, size * sizeof(char), 1, fp);
        fclose(fp);
    }

    void free() const
    {
        ::free(pixels);
    }

public:
    char* pixels;
    int width, height, size;

private:
    int padded_width;
};


#endif //CPU_RASTERIZER_BITMAP_H
