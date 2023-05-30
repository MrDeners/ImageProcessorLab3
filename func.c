#include "func.h"

float color()
{
    float col = 0;
    while (!scanf("%f", &col) || getchar() != '\n')
    {
        printf("Error!Try again:");
        rewind(stdin);
    }
    return col;
}

void applyNegativeFilter(Pixel *imageData, int imgDataSize)
{
    for (int i = 0; i < imgDataSize; i++)
    {
        imageData[i].blue = 255 - imageData[i].blue;
        imageData[i].green = 255 - imageData[i].green;
        imageData[i].red = 255 - imageData[i].red;
    }
}

void applyBlackAndWhiteFilter(Pixel *imageData, int imgDataSize)
{
    for (int i = 0; i < imgDataSize; i++)
    {
        // Вычисление среднего значения цветовых каналов пикселя
        unsigned char gray = (imageData[i].blue + imageData[i].green + imageData[i].red) / 3;
        imageData[i].blue = gray;
        imageData[i].green = gray;
        imageData[i].red = gray;
    }
}

void applyMedianFilter(Pixel *imageData, int width, int height)
{
    // Объявление массивов для хранения отсортированных значений цветовых каналов пикселей в окрестности 3x3 вокруг каждого пикселя
    unsigned char sortedR[9];
    unsigned char sortedG[9];
    unsigned char sortedB[9];
    printf("Введите значение медианной фильтрации:");
    float count = color();
    // Цикл по количеству итераций медианного фильтра
    for (int i = 0; i < count; i++)
    {
        for (int y = 1; y < height - 1; y++)
        {
            for (int x = 1; x < width - 1; x++)
            {
                int i = 0;
                for (int yy = -1; yy <= 1; yy++)
                {
                    for (int xx = -1; xx <= 1; xx++)
                    {
                        // Вычисление индекса пикселя в окрестности
                        int index = (y + yy) * width + (x + xx);
                        sortedR[i] = imageData[index].red;
                        sortedG[i] = imageData[index].green;
                        sortedB[i] = imageData[index].blue;
                        i++;
                    }
                }

                // Сортировка массивов значений цветовых каналов по возрастанию
                for (i = 0; i < 9; i++)
                {
                    for (int j = i + 1; j < 9; j++)
                    {
                        if (sortedR[i] > sortedR[j])
                        {
                            unsigned char temp = sortedR[i];
                            sortedR[i] = sortedR[j];
                            sortedR[j] = temp;
                        }
                        if (sortedG[i] > sortedG[j])
                        {
                            unsigned char temp = sortedG[i];
                            sortedG[i] = sortedG[j];
                            sortedG[j] = temp;
                        }
                        if (sortedB[i] > sortedB[j])
                        {
                            unsigned char temp = sortedB[i];
                            sortedB[i] = sortedB[j];
                            sortedB[j] = temp;
                        }
                    }
                }

                // Вычисление индекса текущего пикселя
                int index = y * width + x;
                // Установка значений цветовых каналов текущего пикселя в медианные значения (пятый элемент) отсортированных массивов значений цветовых каналов
                imageData[index].red = sortedR[4];
                imageData[index].green = sortedG[4];
                imageData[index].blue = sortedB[4];
            }
        }
    }
}

void applyGammaCorrectionFilter(Pixel *imageData, int imgDataSize)
{
    printf("\033[1;32mВведите значение гамма для синего:\033[0m");
    float gamma_blue = color();
    printf("\033[1;33mВведите значение гамма для зеленого: \033[0m");
    float gamma_green = color();
    printf("\033[1;31mВведите значение гамма для красного: \033[0m");
    float gamma_red = color();

    for (int i = 0; i < imgDataSize; i++)
    {
        imageData[i].blue = pow(imageData[i].blue / 255.0, gamma_blue) * 255;
        imageData[i].green = pow(imageData[i].green / 255.0, gamma_green) * 255;
        imageData[i].red = pow(imageData[i].red / 255.0, gamma_red) * 255;
    }
}

FILE *openBMPFile(char *filename)
{
    FILE *fp = NULL;
    while (fp == NULL)
    {
        printf("\033[1;1mВведите имя файла BMP для обработки: \033[0m");
        scanf("%s", filename);
        fp = fopen(filename, "rb");
        if (fp == NULL)
        {
            printf("\033[1;31mОШИБКА:\033[0m \033[2;29mНе удалось открыть файл\033[0m \033[1;29m\"%s\"\033[0m\n", filename);
            Sleep(2000);
            system("cls");
        }
    }
    return fp;
}

void saveBMPFile(unsigned char *bmpHeader, unsigned char *bmpInfoHeader, Pixel *imageData, int imgDataSize)
{
    char form[] = ".bmp";
    char new_name[100];
    // Запрос у пользователя имени нового файла для сохранения
    printf("Введите имя нового файла для сохранения:");
    scanf("%s", &new_name);

    // Добавление расширения ".bmp" к имени файла
    strcat(new_name, form);
    // Открытие файла в двоичном режиме записи
    FILE *fo = fopen(new_name, "wb");

    // Запись заголовка BMP, информационного заголовка BMP и данных изображения в файл
    fwrite(bmpHeader, sizeof(BMPHeader), 1, fo);
    fwrite(bmpInfoHeader, sizeof(BMPInfoHeader), 1, fo);
    fwrite(imageData, sizeof(Pixel), imgDataSize, fo);

    // Закрытие файла
    fclose(fo);
    printf("Пока!");
}

// Определение функции меню
void menu()
{
    char filename[50];
    // Открытие файла BMP с помощью функции openBMPFile
    FILE *fp = openBMPFile(filename);

    BMPHeader bmpHeader;
    BMPInfoHeader bmpInfoHeader;
    // Чтение заголовка и информационного заголовка BMP из файла с помощью fread
    fread(&bmpHeader, sizeof(BMPHeader), 1, fp);
    fread(&bmpInfoHeader, sizeof(BMPInfoHeader), 1, fp);

    int width = bmpInfoHeader.width;
    int height = bmpInfoHeader.height;
    int bitDepth = bmpInfoHeader.bitsPerPixel;
    system("cls");
    if (bitDepth != 24)
    {
        printf("Ошибка: поддерживаются только 24-битные изображения\n");
        fclose(fp);
        exit(1);
    }

    int imgDataSize = width * height;
    Pixel *imageData = (Pixel *)calloc(imgDataSize, sizeof(Pixel));
    fread(imageData, sizeof(Pixel), imgDataSize, fp);
    fclose(fp);

    char *newFilename = (char *)calloc(100, sizeof(char));
    system("cls");
    printf("\033[1;1mВыберите опцию фильтра:\n1 - Негатив.\n2 - Черно-белый.\n3 - Медианная фильтрация\n4 - Гамма-коррекция\n0 - Выход.\nВаш выбор: \033[0m");
    int var = 0;
    scanf("%d", &var);

    system("cls");

    switch (var)
    {
    case 1:
        applyNegativeFilter(imageData, imgDataSize);
        system("cls");
        break;
    case 2:
        applyBlackAndWhiteFilter(imageData, imgDataSize);
        system("cls");
        break;
    case 3:
        applyMedianFilter(imageData, width, height);
        system("cls");
        break;
    case 4:
        applyGammaCorrectionFilter(imageData, imgDataSize);
        system("cls");
        break;
    default:
        break;
    }
    saveBMPFile((unsigned char *)&bmpHeader, (unsigned char *)&bmpInfoHeader, imageData, imgDataSize);
    free(imageData);
}
