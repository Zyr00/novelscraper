#include "../includes/scraper.h"

/*
 * Get document pointer
 *
 * @param memory where the html is
 * @param char the url of the html
 *
 * @return the document
 */
static htmlDocPtr get_doc(memory *, char *);

/*
 * Get xpath
 *
 * @param htmlDocPtr the pointer of the html document
 * @param char the xpath string
 *
 * @return xmlXPathObjectPtr
 *         or NULL
 */
static xmlXPathObjectPtr get_xpath(htmlDocPtr, char *);

static htmlDocPtr get_doc(memory *mem, char *url) {
  int opts = HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING | HTML_PARSE_NONET;
  return htmlReadMemory(mem->buf, mem->size, url, NULL, opts);
}

static xmlXPathObjectPtr get_xpath(htmlDocPtr doc, char *xpath) {
  xmlXPathContextPtr context = xmlXPathNewContext(doc);
  xmlXPathObjectPtr result = xmlXPathEvalExpression((xmlChar *) xpath, context);
  xmlXPathFreeContext(context);
  return result;
}

void
get_total_chapters_count(memory *mem, char *url) {
  htmlDocPtr doc = get_doc(mem, url);

  xmlXPathObjectPtr result = get_xpath(doc, LAST_URL);
  if(!result)
    err(1, "result is null");

  xmlNodeSetPtr nodeset = result->nodesetval;
  if(xmlXPathNodeSetIsEmpty(nodeset)) {
    xmlXPathFreeObject(result);
    err(1, "xpath is empty");
  }

  xmlNodePtr node = (xmlNodePtr) nodeset->nodeTab[nodeset->nodeNr - 1];
  char *last_chapter = (char *) xmlGetProp(node, (const xmlChar *) "href");
  int number = get_number_from_string(last_chapter);

  int final_size = strlen(url) - strlen(".html");

  n.base_url = calloc(final_size + 1, sizeof(char *));
  strncpy(n.base_url, url, final_size);

  n.total_ch = number;

  free(last_chapter);
  xmlXPathFreeObject(result);
  xmlFreeDoc(doc);
}

chapter *
read_chapter(memory *mem, char *url) {

  htmlDocPtr doc = get_doc(mem, url);
  xmlXPathObjectPtr result = get_xpath(doc, DIV_TXT);
  xmlXPathObjectPtr title_result = get_xpath(doc, TITLE);

  if(!result || !title_result)
    return NULL;

  xmlNodeSetPtr nodeset = result->nodesetval;
  xmlNodeSetPtr nodeset_title = title_result->nodesetval;
  if(xmlXPathNodeSetIsEmpty(nodeset) || xmlXPathNodeSetIsEmpty(nodeset_title)) {
    xmlXPathFreeObject(result);
    xmlXPathFreeObject(title_result);
    return NULL;
  }

  xmlNodePtr div = (xmlNodePtr) nodeset->nodeTab[nodeset->nodeNr - 1];
  xmlNodePtr title = (xmlNodePtr) nodeset_title->nodeTab[nodeset_title->nodeNr - 1];
  xmlNodePtr cur = NULL;

  chapter *ch = (chapter *) malloc(sizeof(chapter));
  if (ch == NULL)
    return NULL;
  memset(ch, 0, sizeof(chapter));

  ch->title = (char *) xmlNodeListGetString(doc, title->xmlChildrenNode, 1);

  init_chapter_lines_list(ch);
  if (ch->lines == NULL)
    return NULL;

  for (cur = div->children; cur; cur = cur->next) {
    if (cur->type == XML_ELEMENT_NODE) {
      if (!xmlStrcmp(cur->name, (const xmlChar *) "p")) {
        xmlChar *line = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
        add_to_lines_list(ch, (char *) line);
      }
    }
  }

  xmlXPathFreeObject(result);
  xmlXPathFreeObject(title_result);
  xmlFreeDoc(doc);
  return ch;
}
