/* See LICENSE file for copyright and license details. */
#include "core.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROGRAM_NAME "ftext"
#define VERSION "0.0.1"
#define COLS 3
#define LINES 24
#define WIDTH 21
#define GAP 6
#define MAX_INPUT_SIZE 256

void emit_invalid_arg(char* opt);
void emit_try_help(void);
void emit_version(void);
void usage(void);
FILE* vuln(char*);

/* ftext available options. */
static struct option const longopts[] = {
    {"version", no_argument, NULL, 'v'},
    {"help", no_argument, NULL, 'h'},
    {"lines", required_argument, NULL, 'l'},
    {"columns", required_argument, NULL, 'c'},
    {"width", required_argument, NULL, 'w'},
    {"gap", required_argument, NULL, 'g'},
    {"input", required_argument, NULL, 'i'},
    {NULL, 0, NULL, 0},
};

void usage(void)
{
  printf("Usage %s [OPTIONS]\n", PROGRAM_NAME);
  puts("Column the text provided in stdin and print it in stdout.");
  puts("The following options can be used to customize the formatting.");
  printf("  --input   -i    input string \n");
  printf("  --columns -c    number of columns (default %d)\n", COLS);
  printf("  --lines   -l    number of lines (default %d)\n", LINES);
  printf("  --width   -w    number of characters on a row of a column "
         "(default "
         "%d)\n",
         WIDTH);
  printf("  --gap     -l    number of spaces between columns (default %d)\n", GAP);
  puts("  --help    -h    display this help and exit");
  puts("  --version -v    output version information and exit");
  exit(EXIT_SUCCESS);
}

void emit_version(void)
{
  printf("%s\n", VERSION);
  exit(EXIT_SUCCESS);
}

void emit_invalid_arg(char* opt)
{
  fprintf(stderr, "Invalid argument for option %s\n", opt);
  emit_try_help();
}

void emit_try_help(void)
{
  fprintf(stderr, "Try '%s --help' for more information.\n", PROGRAM_NAME);
  exit(EXIT_FAILURE);
}

int main(int argc, char* argv[])
{
  /* Default values for options. */

  int cols = COLS;
  int lines = LINES;
  int width = WIDTH;
  int gap = GAP;
  FILE* input_stream = stdin;

  /* Handle user options. */
  /* TODO: improve error messages using strtol. */

  int opt;
  while ((opt = getopt_long(argc, argv, "i:c:l:w:g:vhm", longopts, NULL)) != -1)
    switch (opt) {
    case 'v':
      emit_version();
      break;
    case 'h':
      usage();
      break;
    case 'c':
      cols = strtol(optarg, NULL, 0);
      break;
    case 'l':
      lines = strtol(optarg, NULL, 0);
      break;
    case 'w':
      width = strtol(optarg, NULL, 0);
      break;
    case 'g':
      gap = strtol(optarg, NULL, 0);
      break;
    case 'i':
      input_stream = vuln(optarg);
      break;
    case '?':
      emit_try_help();
      break;
    }

  /* Handling incorrect values. */

  if (cols <= 0)
    emit_invalid_arg("--columns");
  if (lines <= 0)
    emit_invalid_arg("--lines");
  if (width <= 0)
    emit_invalid_arg("--width");
  if (gap < 0)
    emit_invalid_arg("--gap");

  format(input_stream, stdout, cols, lines, width, gap);

  return EXIT_SUCCESS;
}

FILE* vuln(char user_input[])  {
  char input[MAX_INPUT_SIZE] = "";
  memcpy(input, user_input, strlen(user_input));
  char* final_input = strdup(user_input);
  return fmemopen(final_input, strlen(final_input), "r");
}
