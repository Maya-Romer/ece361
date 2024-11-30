#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "WeatherSearchTree.h"

struct node* rootNode = NULL;

void insertWeatherDataIntoTree(struct weather_data* data) {
    struct node* tempNode = malloc(sizeof(struct node));
    struct node* current;
    struct node* parent;

    if (tempNode == NULL) {
        printf("Out of memory.");
        exit(1);
    }
    if (data == NULL) {
        printf("Cannot add null data.");
        return;
    }

    tempNode->data = data;
    tempNode->leftChild = NULL;
    tempNode->rightChild = NULL;

    //if tree is empty
    if (rootNode == NULL) {
        rootNode = tempNode;
    }
    else {
        current = rootNode;
        parent = NULL;

        while (1) {
            parent = current;

            //go to left of the tree
            if (data->timestamp < parent->data->timestamp) {
                current = current->leftChild;

                //insert to the left
                if (current == NULL) {
                    parent->leftChild = tempNode;
                    return;
                }
            }  //go to right of the tree
            else {
                current = current->rightChild;

                //insert to the right
                if (current == NULL) {
                    parent->rightChild = tempNode;
                    return;
                }
            }
        }
    }
}

struct node* searchTree(time_t timestamp) {
    struct node* current = rootNode;

    while (current->data->timestamp != timestamp) {
        if (current->data->timestamp > timestamp)
            current = current->leftChild;
        else
            current = current->rightChild;

        //not found
        if (current == NULL) {
            return NULL;
        }
    }

    return current;
}

void printInOrderFrom(struct node* node) {
    if (node->leftChild != NULL)
        printInOrderFrom(node->leftChild);
    struct tm* time = localtime(&node->data->timestamp);
    printf("%i-%i-%i: Temperature: %f, Humidity: %f\n", 
        time->tm_year + 1900, time->tm_mon + 1, time->tm_mday, node->data->temperature, node->data->humidity);
    if (node->rightChild != NULL)
        printInOrderFrom(node->rightChild);
}

void printTreeInOrder() {
    if (rootNode == NULL)
        return;
    printInOrderFrom(rootNode);
}

void deleteTreeFrom(struct node* node) {
    if (node->leftChild != NULL)
        deleteTreeFrom(node->leftChild);
    if (node->rightChild != NULL)
        deleteTreeFrom(node->rightChild);
    free(node);
}

void deleteTree() {
    if (rootNode == NULL)
        return;
    deleteTreeFrom(rootNode);
    rootNode = NULL;
}