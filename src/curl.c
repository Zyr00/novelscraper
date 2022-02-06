#include "../includes/common.h"
#include "../includes/curl.h"

CURL *
init(void) {
  CURL *curl = curl_easy_init();
  if (!curl) {
    err(0, "Error initializing curl\n");
  }
  return curl;
}

void
destroy(CURL *curl) {
  curl_easy_cleanup(curl);
}

memory *
make_request(char *url) {
  CURLcode res;
  memory *mem;

  CURL *curl = init();
  mem = (memory *) malloc(sizeof(memory));
  if (mem == NULL) {
    return NULL;
  }
  mem->size = 0;
  mem->buf = malloc(1);

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, grow_buffer);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, mem);
  curl_easy_setopt(curl, CURLOPT_PRIVATE, mem);

  res = curl_easy_perform(curl);
  if(res != CURLE_OK) {
    destroy(curl);
    err(1, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
  }

  curl_easy_getinfo(curl, CURLINFO_PRIVATE, &mem);
  destroy(curl);

  return mem;
}

size_t
grow_buffer(void *contents, size_t sz, size_t nmemb, void *ctx) {
  size_t realsize = sz * nmemb;
  memory *mem = (memory*) ctx;
  char *ptr = realloc(mem->buf, mem->size + realsize);
  if(!ptr) {
    err(0, "not enough memory (realloc returned NULL)\n");
    return 0;
  }
  mem->buf = ptr;
  memcpy(&(mem->buf[mem->size]), contents, realsize);
  mem->size += realsize;
  return realsize;
}
