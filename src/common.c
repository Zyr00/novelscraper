/**
 * @file common.c
 * @author João Cunha
 * @brief Common functions
 *
 * Common stuff.
 */
#include "../includes/common.h"

void
err(const int die, const char* fmt, ...) {
  int errno_save;
  va_list ap;

  errno_save = errno;

  va_start(ap, fmt);
  vfprintf(stdout, fmt, ap);
  fprintf(stdout, "\n");
  fflush(stdout);

  if (errno_save != 0) {
    fprintf(stdout, "(errno = %d): %s\n", errno_save, strerror(errno_save));
    fprintf(stdout, "\n");
    fflush(stdout);
  }

  va_end(ap);

  if (die)
    exit(1);
}

void
init_novel(void) {
  memset(&n,  0, sizeof(novel));
  n.tail = &n.head;
}

void
init_chapter_lines_list(chapter *ch) {
  ch->lines = (char **) malloc(INIT_SIZE * sizeof(char *));
  if (ch->lines == NULL)
    err(1, "Could not alocate array of lines\n");

  ch->lines_length = INIT_SIZE;
  ch->lines_size = 0;
}

int
add_to_lines_list(chapter * ch, char *string) {
  if (ch->lines_length == ch->lines_size) {
    ch->lines_length *= 2;
    ch->lines = (char **) realloc(ch->lines, ch->lines_length * sizeof(char *));
    if (ch->lines == NULL)
      err(1, "Could not realloc memory for lines\n");
  }
  ch->lines[ch->lines_size++] = string;
  return ch->lines_size;
}

void
add_to_novel(chapter *ch) {
  n.tail->next = ch;
  n.tail = ch;
}

void
print_chapter(void) {
  chapter *ch = NULL;
  for (ch = n.head.next; ch != NULL; ch = ch->next) {
    printf("TITLE: %s\n", ch->title);
    /*
    for (size_t i = 0; i < ch->lines_size; i++)
      printf("%s\n", ch->lines[i]);
    */
  }
}

int
get_number_from_string(char *str) {
  char *p = str;
  int val = 0;
  while (*p) {
    if (isdigit(*p)) {
      val = strtol(p, &p, 10);
      break;
    } else {
      p++;
    }
  }
  return val;
}
