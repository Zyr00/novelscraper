#include "../includes/common.h"
#include "../includes/file.h"

void chapter_to_tex() {
  FILE *fp;
  char *name;
  size_t j;

  chapter *ch = n.head.next;
  while (ch != NULL) {
    name = (char *) malloc(sizeof(char *));
    sprintf(name, "ch%d.tex", ch->number);

    printf("Writing file %s for %s...\n", name, ch->title);

    fp = fopen(name, "w");
    if (fp == NULL) {
      free(name);
      err(1, "Could not open file %s", name);
    }

    fprintf(fp, "\\section*{%s}\n\n", ch->title);

    for (j = 0; j < ch->lines_size; j++)
      fprintf(fp, "%s\n\n", ch->lines[j]);

    free(name);
    fclose(fp);
    ch = ch->next;
  }
}

void main_tex_file(int total) {
  FILE *fp;
  char *name = "main.tex";
  int i;

  printf("Writing main tex file...\n");

  fp = fopen(name, "w");

  fprintf(fp, "\\documentclass[11pt]{article}\n\n");
  fprintf(fp, "%%xelatex\n");
  fprintf(fp, "\\usepackage{xeCJK}\n");
  fprintf(fp, "\\usepackage[T1]{fontenc}\n");
  fprintf(fp, "\\fontfamily{georgia}\\selectfont\n");
  fprintf(fp, "\\usepackage[a4paper,width=150mm,top=25mm,bottom=25mm]{geometry}\n\n\n");
  fprintf(fp, "\\begin{document}\n\n");

  for (i = 0; i < total; i++) {
    fprintf(fp, "\\input{ch%d.tex}\n", i + 1);
    fprintf(fp, "\\clearpage\n");
  }

  fprintf(fp, "\n\\end{document}\n\n");
  fclose(fp);
}
