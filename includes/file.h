#ifndef __FILE__H__
#define __FILE__H__

#include "common.h"

/*
 * Write a chapter to tex format
 * This creates a file with the chapter number like (ch1.tex)
 */
void chapter_to_tex(void);

/**
 * Create the main tex file that will include all of the
 * other chapter files
 *
 * @param int a integer with the total number of chapters
 */
void main_tex_file(int);

#endif
