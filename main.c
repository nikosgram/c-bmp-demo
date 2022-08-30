#include <stdio.h>
#include <stdlib.h>

#define BMP_MAGIC 19778
#define BMP_GET_PADDING(a) ((a) % 4)

// Results
#define BMP_FILE_NOT_OPENED -2
#define BMP_HEADER_NOT_INITIALIZED -1
#define BMP_OK 0

typedef struct BMP_HEADER {
    unsigned int BufferSize;
    unsigned int BufferReserved;
    unsigned int BufferOffBits;

    unsigned int BitmapSize;
    int BitmapWidth;                 // Width of the bitmap in pixels
    int BitmapHeight;                // Height of the bitmap in pixels. Positive for bottom to top pixel order.
    unsigned short BitmapPlanes;     // Number of color planes being used
    unsigned short BitmapBitCount;   // Number of bits per pixel
    unsigned int BitmapCompression;  // BI_RGB, no pixel array compression used
    unsigned int BitmapSizeImage;    // Size of the raw bitmap data (including padding)
    int BitmapXPelsPerMeter;         // Print resolution of the image, pixels/metre horizontal
    int BitmapYPelsPerMeter;         // pixels/metre vertical
    unsigned int BitmapClrUsed;      // Number of colors in the palette
    unsigned int BitmapClrImportant; // Important colors - 0 means all colors are important
} BMP_HEADER;

typedef struct BMP_PIXEL {
    unsigned char Blue;
    unsigned char Green;
    unsigned char Red;
} BMP_PIXEL;

typedef struct BMP_IMAGE {
    BMP_HEADER ImageHeader;
    BMP_PIXEL **ImagePixels;
} BMP_IMAGE;

void BMPHeaderInitDef(BMP_HEADER *imageHeader, const int width, const int height) {
    imageHeader->BufferSize = (sizeof(BMP_PIXEL) * width + BMP_GET_PADDING (width)) * abs(height);
    imageHeader->BufferReserved = 0;
    imageHeader->BufferOffBits = 54;
    imageHeader->BitmapSize = 40;
    imageHeader->BitmapWidth = width;
    imageHeader->BitmapHeight = height;
    imageHeader->BitmapPlanes = 1;
    imageHeader->BitmapBitCount = 24;
    imageHeader->BitmapCompression = 0;
    imageHeader->BitmapSizeImage = 0;
    imageHeader->BitmapXPelsPerMeter = 0;
    imageHeader->BitmapYPelsPerMeter = 0;
    imageHeader->BitmapClrUsed = 0;
    imageHeader->BitmapClrImportant = 0;
}

int BMPHeaderWrite(const BMP_HEADER *header, FILE *imageFile) {
    if (header == NULL) {
        return BMP_HEADER_NOT_INITIALIZED;
    } else if (imageFile == NULL) {
        return BMP_FILE_NOT_OPENED;
    }

    const unsigned short magic = BMP_MAGIC;

    fwrite(&magic, sizeof(magic), 1, imageFile);
    fwrite(header, sizeof(BMP_HEADER), 1, imageFile);

    return BMP_OK;
}

void BMPImageAlloc(BMP_IMAGE *image) {
    const size_t h = abs(image->ImageHeader.BitmapHeight);

    image->ImagePixels = malloc(sizeof(BMP_PIXEL *) * h);

    for (size_t y = 0; y < h; y++) {
        image->ImagePixels[y] = malloc(sizeof(BMP_PIXEL) * image->ImageHeader.BitmapWidth);
    }
}

void BMPImageFree(BMP_IMAGE *image) {
    const size_t height = abs(image->ImageHeader.BitmapHeight);

    for (size_t y = 0; y < height; y++) {
        free(image->ImagePixels[y]);
    }

    free(image->ImagePixels);
}

int BMPImageWrite(const BMP_IMAGE *image, const char *filename) {
    FILE *imageFile = fopen(filename, "wb");

    if (imageFile == NULL) {
        return BMP_FILE_NOT_OPENED;
    }

    const int err = BMPHeaderWrite(&image->ImageHeader, imageFile);

    if (err != BMP_OK) {
        fclose(imageFile);

        return err;
    }

    const size_t height = abs(image->ImageHeader.BitmapHeight);
    const size_t offset = (image->ImageHeader.BitmapHeight > 0 ? height - 1 : 0);

    const unsigned char padding[3] = {'\0', '\0', '\0'};

    for (size_t y = 0; y < height; y++) {
        fwrite(
                image->ImagePixels[(offset - y)],
                sizeof(BMP_PIXEL),
                image->ImageHeader.BitmapWidth,
                imageFile
        );
        fwrite(
                padding,
                sizeof(unsigned char),
                BMP_GET_PADDING (image->ImageHeader.BitmapWidth),
                imageFile
        );
    }

    fclose(imageFile);

    return BMP_OK;
}

int main() {
    const int width = 1024;
    const int height = 1024;

    BMP_IMAGE image;

    BMPHeaderInitDef(&image.ImageHeader, width, height);
    BMPImageAlloc(&image);

    for (size_t y = 0, x; y < height; y++) {
        for (x = 0; x < width; x++) {
            image.ImagePixels[y][x].Red = ((x + y) * x) / (512);
            image.ImagePixels[y][x].Green = ((x + y) * y) / (512);
            image.ImagePixels[y][x].Blue = ((x + y) * (x - y)) / (512);
        }
    }

    int result = BMPImageWrite(&image, "test.bmp");

    BMPImageFree(&image);

    return result;
}
