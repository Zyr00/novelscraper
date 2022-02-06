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

void get_list_chapters(memory *mem, char *url);

#endif
