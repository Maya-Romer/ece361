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

void populateBST(void* iomBasePtr);
void shuffle(void* array, int count, size_t elementSize);
void getLine(char* buffer, int count);

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

    int returnCode;
    void* iomBasePtr = iom361_initialize(0, 0, &returnCode);
    if (returnCode != 0) {
        printf("IOM361 initialization failed.\n");
        exit(1);
    }

    populateBST(iomBasePtr);
    while (true) {
        printf("Enter day in November 2024:");
        char buffer[5];
        int date;
        getLine(buffer, 5);
        if (buffer[0] == '\0')
            break;
        sscanf_s(buffer, "%i", &date);
        if (date > 30 || date < 1) {
            printf("Invalid input.\n");
            continue;
        }
        struct tm* time = calloc(1, sizeof(struct tm));
        time->tm_year = 124; //2024
        time->tm_mon = 10; //november
        time->tm_mday = date;
        struct weather_data* weather = searchTree(mktime(time));
        printf("Weather data for 11/%i/2024: Temperature: %f C. Humidity: %f%%.\n",
            date, weather->temperature, weather->humidity);
    }

   
    printTreeInOrder();

    return 0;
}

void populateBST(void* iomBasePtr) {
    struct weather_data* novemberWeather[30];

    for (int i = 0; i < 30; i++) {
        struct weather_data* weather = malloc(sizeof(struct weather_data));
        if (weather == NULL) {
            printf("Out of memory.");
            exit(1);
        }

        _iom361_setSensor1_rndm(0, 35, 55, 85);

        int returnCode;
        uint32_t iomRegister = iom361_readReg(iomBasePtr, TEMP_REG, &returnCode);
        if (returnCode != 0)
            printf("Reading temperature failed.");
        float temperature = (iomRegister / (float)(1 << 20)) * 200 - 50; // per iom361 docs
        weather->temperature = temperature;

        iomRegister = iom361_readReg(iomBasePtr, HUMID_REG, &returnCode);
        if (returnCode != 0)
            printf("Reading humidity failed.");
        float humidity = iomRegister / (float)(1 << 20) * 100; // per iom361 docs
        weather->humidity = humidity;

        struct tm* time = calloc(1, sizeof(struct tm));
        time->tm_year = 124; //2024
        time->tm_mon = 10; //november
        time->tm_mday = i + 1;
        weather->timestamp = mktime(time);
        novemberWeather[i] = weather;
        free(time);
    }
    shuffle(novemberWeather, 30, sizeof(struct weather_data*));
    for (int i = 0; i < 30; i++) {
        insertWeatherDataIntoTree(novemberWeather[i]);
    }
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

//generic Fisher–Yates shuffle 
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

//copies stdin to a buffer until new line. buffer will hold a null-terminated string.
void getLine(char* buffer, int count) {
    buffer[count - 1] = '\0';
    for (int i = 0; i < count - 1; i++) {
        char c = getchar();
        if (c == '\r' || c== '\n' || c == '\0') {
            buffer[i] = '\0';
            return;
        }
        buffer[i] = c;

    }
}
