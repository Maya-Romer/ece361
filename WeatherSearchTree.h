#pragma once

/** a struct to hold weather measurement data */
struct weather_data {
    /** The time this reading was taken. Hour, minute, and second are all 0. */
    time_t timestamp;
    /** The measured temperature, in degrees Celsius */
    float temperature;
    /** The measured humidity, in percent */
    float humidity;
};

/**
* Inserts a new weather data element into the tree
* @param data: the data to insert
*/
void insertWeatherDataIntoTree(struct weather_data* data);

/*
* Searches tree for data matching a specified timestamp.
* @param timestamp: the timestamp to search for. Hour, minute, and second should all be 0.
* @return: a pointer to the weather_data with a matching timestamp. NULL if no match found, unpredictable if multiple matches.
*/
struct weather_data* searchTree(time_t timestamp);

/** Prints all weather_data in order of timestamp. */
void printTreeInOrder();

/** Deletes a tree and frees all used memory. */
void deleteTree();
