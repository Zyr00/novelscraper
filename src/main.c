#include "../includes/common.h"
#include "../includes/curl.h"
#include "../includes/scraper.h"
#include "../includes/file.h"
novel n;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int chapter_count = 1;
int max_running_threads_count = 0;

/**
 * Function called by thread to
 * fetch and store the chapter
 *
 * @param void pointer of arguments (NOT USED)
 */
void *handle_chapters_read(void *);

int
main(void) {
  LIBXML_TEST_VERSION;

  memory *mem;
  int i, j, thread_release_count;
  char *u = "https://freewebnovel.com/nano-machine-retranslated-version.html";
  // char *u = "https://freewebnovel.com/master-fus-beloved-petite-wife-is-calling-for-divorce-again.html";

  init_novel();
  curl_global_init(CURL_GLOBAL_DEFAULT);

  mem = make_request(u);
  get_total_chapters_count(mem, u);
  free_memory(mem);

  pthread_t threads[n.total_ch];
  thread_release_count = 0;

  for (i = 0; i < n.total_ch; i++) {
    pthread_create(&threads[i], NULL, handle_chapters_read, NULL);

    if (max_running_threads_count >= MAX_RUNNING_THREADS) {
      for (j = 0; j < max_running_threads_count; j++) {
        pthread_join(threads[thread_release_count], NULL);
        thread_release_count++;
      }
      max_running_threads_count = 0;
    }
  }

  for (i = thread_release_count; i < n.total_ch; i++)
    pthread_join(threads[i], NULL);

  chapter_to_tex();
  main_tex_file(n.total_ch);

  return 0;
}

void *handle_chapters_read(void *_args) {
  UNUSED(_args);

  chapter *ch = NULL;
  memory *mem;
  char url[250];
  int current;

  pthread_mutex_lock(&mutex);
  max_running_threads_count++;
  current = chapter_count++;
  pthread_mutex_unlock(&mutex);

  sprintf(url, "%s/chapter-%d.html", n.base_url, current);

  printf("Processing CH: %d from URL: %s...\n", current, url);

  mem = make_request(url);
  if (mem == NULL) err(1, "Error requesting page: \t%s\n", url);

  ch = read_chapter(mem, url);
  if (ch == NULL) err(1, "Error getting chapter");
  ch->number = current;

  free_memory(mem);

  pthread_mutex_lock(&mutex);
  add_to_novel(ch);
  pthread_mutex_unlock(&mutex);

  pthread_exit(NULL);
}
