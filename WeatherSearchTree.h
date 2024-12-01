#pragma once

//@description: a struct to hold weather measurement data
struct weather_data {
    //@member timestamp: the time this reading was taken. Hour, minute, and second are all 0.
    time_t timestamp;
    //@member temperature: the measured temperature, in degrees Celsius
    float temperature;
    //@member humidity: the measured humidity, in percent
    float humidity;
};

//@description: inserts a new weather data element into the tree
//@param data: the data to insert
void insertWeatherDataIntoTree(struct weather_data* data);
//@description: searches tree for data matching a specified timestamp
//@param timestamp: the timestamp to search for. Hour, minute, and second should all be 0.
//@return: a pointer to the weather_data with a matching timestamp. NULL if no match found, unpredictable if multiple matches.
struct weather_data* searchTree(time_t timestamp);
//@description: prints all weather_data in order of timestamp.
void printTreeInOrder();
//@description: deletes a tree and frees all used memory.
void deleteTree();
