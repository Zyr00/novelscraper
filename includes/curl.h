#ifndef __CURL__H
#define __CURL__H

#include "common.h"

/*
 * Initialize curl
 * @warning this function closes the program if it fails
 *
 * @return CURL instance if it initializes with success
 */
CURL *init(void);

/*
 * Destroy curl instance
 *
 * @para curl the pointer for the curl instance
 */
void destroy(CURL *curl);

/**
 * Make a resquest to a url and retorn the response in memory
 *
 * @param char the url for the request
 */
memory *make_request(char *);

/**
 * Function needed by curl to grow memory to the right size
 */
size_t grow_buffer(void *, size_t, size_t, void *);

#endif
