#ifndef __COMMON_H__
#define __COMMON_H__

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include <curl/curl.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <libxml/uri.h>
#include <pthread.h>

/**
 * A function to stop the compiler from complaning that the
 * void *_args from the thread is not used
 */
#define UNUSED(x) (void)(x)

/**
 * The Initial size of the chapter lines
 */
#define INIT_SIZE 10

/*
 * The max number of threads that can run at a time
 */
#define MAX_RUNNING_THREADS 10

/*
 * Xpath of the link from the last chapter of a novel
 */
#define LAST_URL "/html/body/div[2]/div/div/div[1]/div[2]/ul/li[1]/a"

/*
 * Xpath of the chapter title
 */
#define TITLE "/html/body/div[2]/div/div/div[1]/span"

/*
 * Xpath of the div that contains the chapter content
 */
#define DIV_TXT "/html/body/div[2]/div/div/div[2]"

typedef struct {
  char *buf;
  size_t size;
} memory;

struct chapter {
  int number;
  char *title;
  char **lines;
  size_t lines_length;
  size_t lines_size;
  struct chapter *next;
};
typedef struct chapter chapter;

typedef struct {
  chapter head, *tail;
  char *base_url;
  int total_ch;
} novel;

/**
 * Make a global novel
 */
extern novel n;

/**
 * Function to print a errno to the console
 * @warning does not work on widows
 *
 * @param int 0 to not exit and 1 to exit
 * @param char the formatted message to show to the user (like printf)
*/
void err(const int, const char *, ...);

/**
 * Initialize a novel
 */
void init_novel(void);

/**
 * Initialize a list of lines for the chapter as an initial value of INIT_SIZE
 *
 * @param chapter the pointer to the chapter
 */
void init_chapter_lines_list(chapter *);

/**
 * Add a line to the lines list
 *
 * @param chapter the pointer to the chapter
 * @param char the string to add to the list
 *
 * @returns the size if added successfuly or
 *          -1 if failed
 */
int add_to_lines_list(chapter *, char *);

/**
 * Add chapter to novel if fails exits
 *
 * @param chapter the pointer to the chapter
 */
void add_to_novel(chapter *);

/**
 * Free a memory struct pointer
 *
 * @param memory the memory pointer to free
 */
void free_memory(memory *);

/**
 * Free the novel struct, this includes
 * all of the chapters
 */
void free_novel(void);

/**
 * Extract a number from a string
 *
 * @param char the string
 *
 * @return the number found or -1
 */
int get_number_from_string(char *);

#endif
