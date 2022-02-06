#include "../includes/common.h"
#include "../includes/curl.h"
#include "../includes/scraper.h"
#include "../includes/file.h"

novel n;

// void main(void) { main_tex_file(); }

int
main(void) {
  LIBXML_TEST_VERSION;

  memory *mem;
  char *u = "https://freewebnovel.com/nano-machine-retranslated-version.html";

  init_novel();
  curl_global_init(CURL_GLOBAL_DEFAULT);

  mem = make_request(u);
  get_list_chapters(mem, u);

  chapter *ch = NULL;
  for (int i = 0; i < n.total_ch; i++) {
    char url[65];
    sprintf(url, "%s/chapter-%d.html", n.base_url, i + 1);

    printf("Requesting url: %s\n", url);

    mem = make_request(url);
    if (mem == NULL)
      err(1, "Error requesting page: \t%s\n", url);

    ch = read_chapter(mem, url);
    if (ch == NULL)
      err(1, "Error getting chapter");

    add_to_novel(ch);
  }

  chapter_to_tex();
  main_tex_file(n.total_ch);

  return 0;
}
// */
