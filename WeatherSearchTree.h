#pragma once

struct weather_data {
    time_t timestamp;
    float temperature;
    float humidity;
};

struct node {
    struct weather_data* data;
    struct node* leftChild;
    struct node* rightChild;
};

void insertWeatherDataIntoTree(struct weather_data* data);
struct weather_data* searchTree(time_t timestamp);
void printTreeInOrder();
void deleteTree();
