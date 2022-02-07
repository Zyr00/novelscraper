#ifndef __SCRAPER__H
#define __SCRAPER__H

#include "common.h"

/**
 * Scrape a chapter from html
 *
 * @param memory the html page in memory
 * @param char url of the page
 *
 * @return chapter a pointer to the chapter
 */
chapter *read_chapter(memory *, char *);

/**
 * Get the total count of chapters to handle
 * @warning value goes to global novel
 *
 * @param memory the html page in memory
 * @param char the string url of the html page
 */
void get_total_chapters_count(memory *, char *);

#endif
