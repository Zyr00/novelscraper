#include "../includes/common.h"
#include "../includes/curl.h"
#include "../includes/scraper.h"
#include "../includes/file.h"
#include <pthread.h>

novel n;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
  memory *mem;
  char *url;
  int number;
} thread_args;

// void main(void) { main_tex_file(); }
//
void *handle_chapters_read(void *);

int
main(void) {
  LIBXML_TEST_VERSION;

  memory *mem;
  int i;
  // char *u = "https://freewebnovel.com/nano-machine-retranslated-version.html";
  char *u = "https://freewebnovel.com/master-fus-beloved-petite-wife-is-calling-for-divorce-again.html";

  init_novel();
  curl_global_init(CURL_GLOBAL_DEFAULT);

  mem = make_request(u);
  get_list_chapters(mem, u);
  free_memory(mem);

  pthread_t threads[n.total_ch];

  for (i = 0; i < n.total_ch; i++) {
    char url[250];
    memset(&url, '\0', sizeof(url));
    snprintf(url, 250, "%s/chapter-%d.html", n.base_url, i + 1);
    printf("Requesting url: %s\n", url);

    mem = make_request(url);
    if (mem == NULL)
      err(1, "Error requesting page: \t%s\n", url);

    thread_args args = { mem, url, i + 1 };
    pthread_create(&threads[i], NULL, handle_chapters_read, (void *) &args);
  }

  for (i = 0; i < n.total_ch; i++)
    pthread_join(threads[i], NULL);

  chapter_to_tex();
  main_tex_file(n.total_ch);

  chapter *ch = NULL;
  while (n.head.next) {
    ch = n.head.next;
    n.head.next = ch->next;
    for (size_t i = 0; i < ch->lines_size; i++)
      free(ch->lines[i]);
    free(ch->lines);
    free(ch->title);
    free(ch);
  }

  free(n.base_url);

  return 0;
}
// */

void *handle_chapters_read(void *_args) {
  chapter *ch = NULL;

  thread_args args = *(thread_args *) _args;
  printf("Processing chapter %d...\n", args.number);

  ch = read_chapter(args.mem, args.url);
  if (ch == NULL) {
    err(1, "Error getting chapter");
  }
  ch->number = args.number;

  pthread_mutex_lock(&mutex);
  add_to_novel(ch);
  pthread_mutex_unlock(&mutex);

  free_memory(args.mem);

  pthread_exit(NULL);
}
