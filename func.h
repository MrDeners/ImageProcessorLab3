#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <Windows.h>

#pragma pack(push, 1)
// Определение структуры для заголовка файла BMP
typedef struct
{
    unsigned short type;      // Тип файла; должен быть BM
    unsigned int size;        // Размер файла BMP в байтах
    unsigned short reserved1; // Зарезервировано; должно быть 0
    unsigned short reserved2; // Зарезервировано; должно быть 0
    unsigned int offset;      // Смещение в байтах от начала структуры BMPHeader до битов растра
} BMPHeader;

// Определение структуры для информационного заголовка BMP
typedef struct
{
    unsigned int size;            // Размер этого заголовка в байтах (40)
    int width;                    // Ширина растра в пикселях
    int height;                   // Высота растра в пикселях
    unsigned short planes;        // Количество плоскостей для целевого устройства; должно быть 1
    unsigned short bitsPerPixel;  // Количество бит на пиксель (глубина цвета)
    unsigned int compression;     // Тип сжатия (0 = нет)
    unsigned int imageSize;       // Размер данных изображения в байтах
    int xPixelsPerMeter;          // Горизонтальное разрешение в пикселях на метр
    int yPixelsPerMeter;          // Вертикальное разрешение в пикселях на метр
    unsigned int colorsUsed;      // Количество используемых цветов
    unsigned int colorsImportant; // Количество важных цветов
} BMPInfoHeader;

typedef struct
{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
} Pixel;
#pragma pack(pop)

float color();
void applyNegativeFilter(Pixel *imageData, int imgDataSize);
void applyBlackAndWhiteFilter(Pixel *imageData, int imgDataSize);
void applyMedianFilter(Pixel *imageData, int width, int height);
void applyGammaCorrectionFilter(Pixel *imageData, int imgDataSize);
FILE *openBMPFile(char *filename);
void saveBMPFile(unsigned char *bmpHeader, unsigned char *bmpInfoHeader, Pixel *imageData, int imgDataSize);
void menu();
