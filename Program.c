// include files
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#if defined(_WIN32) || defined(WIN32)
#define WINDOWS
#include <direct.h>
#elif defined(__unix__)
#include <unistd.h>
#endif
#include <errno.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "./iom361_r2/iom361_r2.h"
#include "WeatherSearchTree.h"

void shuffle(void* array, int count, size_t elementSize);

int main() {
    // display a greeting and the current working directory
    printf("greeting\n\n");
    errno = 0;
    char* buf = getcwd(NULL, 0);    // allocates a buffer large enough to hold the path
    if (buf == NULL) {
        perror("getcwd");
        printf("Could not display the path\n");
    }
    else {
        printf("Current working directory: %s\n", buf);
        free(buf);
    }
    printf("\n");

    struct weather_data* novemberWeather[30];

    for (int i = 0; i < 30; i++) {
        struct weather_data* weather = malloc(sizeof(struct weather_data));
        weather->humidity = (rand() / (float)RAND_MAX) * 100;
        weather->temperature = 32 + (rand() / (float)RAND_MAX) * 60;

        struct tm* time = calloc(1, sizeof(struct tm));
        time->tm_year = 124; //2024
        time->tm_mon = 10; //november
        time->tm_mday = i + 1;
        weather->timestamp = mktime(time);
        novemberWeather[i] = weather;
    }
    shuffle(novemberWeather, 30, sizeof(struct weather_data*));
    for (int i = 0; i < 30; i++) {
        insertWeatherDataIntoTree(novemberWeather[i]);
    }
    printTreeInOrder();

    return 0;
}

void testTree() {
    for (int i = 0; i < 20; i++) {
        struct weather_data* weather = malloc(sizeof(struct weather_data));
        weather->humidity = (rand() / (float)RAND_MAX) * 100;
        weather->temperature = 32 + (rand() / (float)RAND_MAX) * 60;
        weather->timestamp = 1704070800 + (rand() / (float)RAND_MAX) * 31622400; //random unix timestamp in 2024
        insertWeatherDataIntoTree(weather);
    }
    printTreeInOrder();
    deleteTree();
    insertWeatherDataIntoTree(NULL); //should result in error message
    printTreeInOrder(); //should not print anything
}

//Fisher–Yates shuffle 
void shuffle(void* array, int count, size_t elementSize) {
    char* bytearray = (char*)array; //for pointer math
    void* swapMem = malloc(elementSize);
    for (int i = count - 1; i > 0; i--) {
        int toSwap = floor((rand() / (float)RAND_MAX) * (i + 1)); //random int, 0 <= toSwap <= i
        memcpy(swapMem, bytearray + i * elementSize, elementSize);
        memcpy(bytearray + i * elementSize, bytearray + toSwap * elementSize, elementSize);
        memcpy(bytearray + toSwap * elementSize, swapMem, elementSize);
    }
    free(swapMem);
}
