/**
  * Thanks to https://github.com/gabrieledarrigo/c-weather
  **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "jsmn.h"

#ifndef PARSED_JSON
#define PARSED_JSON
/**
 * A record that holds the parsed json.
 * @var int number_of_tokens The number of parsed tokens.
 * @var jsmntok_t * tokens The parsed tokens.
 **/
struct Parsed_Json {
    int number_of_tokens;
    jsmntok_t * tokens;
};
#endif //PARSED_JSON

#ifndef PARSE_JSON
#define PARSE_JSON
/**
 * Parse a json string with jsmn lib.
 * @see https://github.com/zserge/jsmn
 * for parsing implementation detail.
 * Return the parsed tokens.
 *
 * @param int number_of_tokens
 * @param char * data
 * @return jsmntok_t * tokens
 */
struct Parsed_Json parse_json(char * data, int number_of_tokens) {
    jsmn_parser parser;
    jsmn_init(&parser);
    struct Parsed_Json parsed;

    // Calculate the memory required for the tokens
    size_t size = sizeof(jsmntok_t) * number_of_tokens;
    jsmntok_t * tokens = malloc(size);

    int result = jsmn_parse(&parser, data, strlen(data), tokens, number_of_tokens);

    if (result < 0) {
        printf("Failed to parse JSON");
        perror("Err");
        exit(1);
    }

    if (tokens[0].type != JSMN_ARRAY && tokens[0].type != JSMN_OBJECT) {
        printf("The first element should be an array or an object");
        exit(1);
    }

    parsed.number_of_tokens = result;
    parsed.tokens = tokens;

    return parsed;
}
#endif //PARSE_JSON


#ifndef CHECK_JSON_STRING
#define CHECK_JSON_STRING
/**
 * Check if two string parsed from a jsmntok_t token are equal.
 * Return 0 if the string are equals, 0 otherwise
 *
 * @param char * to_search
 * @param char * string
 * @param jsmntok_t token
 * @return int
 */
int check_json_string(char * to_search, char * string, jsmntok_t token) {
    if (token.type == JSMN_STRING && strcmp(to_search, string) == 0) {
        return 0;
    }

    return -1;
}
#endif //CHECK_JSON_STRING

#ifndef BUFFER
#define BUFFER
/**
 * A simple Buffer structure.
 * @var char * memory An array of chars containing the buffer data
 * @var size_t size Array fo chars size
 */
struct Buffer {
    char * memory;
    size_t size;
};
#endif //BUFFER

/**
 * Save the data from received from the server into a Buffer struct.
 * @see https://curl.haxx.se/libcurl/c/CURLOPT_WRITEDATA.html for the function signature.
 */
size_t write_to_buffer(void *data, size_t size, size_t nmemb, void *pointer) {
    size_t real_size = size * nmemb;

    // Cast the received pointer to the Buffer structured data.
    struct Buffer * buffer = (struct Buffer *) pointer;

    // Reallocate the memory for the Buffer char pointer
    buffer->memory = realloc(buffer->memory, buffer->size + real_size + 1);
    buffer->size += real_size;

    if (buffer->memory == NULL) {
        printf("Failed to allocate memory for the http get data");
        return 1;
    }

    // Copy the fetched data to the Buffer char pointer
    memcpy(buffer->memory, data, real_size);
    buffer->memory[real_size] = '\0';

    return real_size;
};

/**
 * Retrieve the weather data from https://openweathermap.org/api
 * with an HTTP GET request.
 * Return an array of chars.
 *
 * @param char * city
 * @return char * buffer.memory
 */
char * getWeatherData() {
    curl_global_init(CURL_GLOBAL_ALL);

    // Build the url
    char base_url[256];
    snprintf(base_url, (size_t)256, "https://api.openweathermap.org/data/2.5/onecall?lat=%s&lon=%s&exclude=minutely,hourly&appid=%s&units=metric", WEATHER_LON, WEATHER_LAN, WEATHER_KEY);
    char length = strlen(base_url);
    char * url = malloc(sizeof(char) * length + 1);
    memset(url, 0, length);

    strcat(url, base_url);
    //strcat(url, city);

    // Allocate an empty buffer that will hold the http get data
    struct Buffer buffer;
    buffer.memory = NULL;
    buffer.size = 0;

    CURL *handle;
    int result;
    handle = curl_easy_init();

    // Set curl options
    curl_easy_setopt(handle, CURLOPT_URL, url);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_to_buffer);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *) &buffer);
    result = curl_easy_perform(handle);

    if (result != CURLE_OK) {
        printf("A problem occurred during the HTTP CALL to:\n\n%s\n\nWith the following error: %s\n\n", url, curl_easy_strerror(result));
        exit(1);
    }

    free(handle);
    return buffer.memory;
};
