# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>

/*  This program show solutions of the game frog_walk_or_jump.  */

/*  # define PRINT_WITH_COLOR  */

# define COLOR_RED   0x01
# define COLOR_GREEN 0x02
# define COLOR_BLUE  0x04

char const jump[] = {'d', 'k'};
char const walk[] = {'f', 'j'};

void Print_symbol(char const *operation, int index);

int main(int argc, char const *argv[])
{
  unsigned int nFrogs = 0;
  unsigned int i = 0, j = 0;
  char pauser = '\0';
  unsigned int const nFrogsDef = 3;
  char buf[BUFSIZ + 1] = "";
  bool isInteractiveMode = true;

  for (;;)
  {
    if (argc > 2)
    {
      fprintf(stderr, "At most 1 command argument should be provided, but got %d.\n", (argc - 1));
      fprintf(stderr, "Please use the interactive mode.\n");
    }
    else if (argc == 2)
    {
      if (! strcasecmp(argv[1], "default"))
      {
        nFrogs = nFrogsDef;
        isInteractiveMode = false;
        break;
      }
      if (sscanf(argv[1], "%u", & nFrogs) == 1)
      {
        if ((int)nFrogs > 0)
        {
          isInteractiveMode = false;
          break;
        }
        fprintf(stderr, "A positive number for amount of frogs is required, but got %d.\n", (int)nFrogs);
        fprintf(stderr, "Please use the interactive mode.\n");
      }
      else
        fprintf(stderr, "Unrecognizable number \"%s\", please use the interactive mode.\n", argv[1]);
    }
    printf("Input the number of frogs on each side.\n");
    printf("Press <Enter> directly to use the default value: %u.\n", nFrogsDef);
    for (;;)
    {
      fgets(buf, BUFSIZ, stdin);
      while (strlen(buf) > 0 && buf[strlen(buf) - 1] == '\n')
        buf[strlen(buf) - 1] = '\0';
      if (! strcmp(buf, ""))
      {
        nFrogs = nFrogsDef;
        break;
      }
      if (sscanf(buf, "%u", & nFrogs) == 1)
        break;
      fprintf(stderr, "Unrecognizable number \"%s\", please input again.\n", buf);
    }  
    break;
  }
  
  printf("\n");
  printf("f/d for left side and j/k for right side.\n");
  printf("d/k for jumping and f/j for walking.\n");
  printf("\n");
  system("");

  for (i = 1; i <= nFrogs; i ++)
    for (j = 1; j <= i; j ++)
      if (j == i)
        Print_symbol(walk, ! (i % 2));
      else
        Print_symbol(jump, ! (i % 2));
  for (j = 1; j <= nFrogs; j ++)
    Print_symbol(jump, nFrogs % 2);
  for (i = nFrogs; i >= 1; i --)
    for (j = i; j >= 1; j --)
      if (j == i)
        Print_symbol(walk, ! (i % 2));
      else
        Print_symbol(jump, ! (i % 2));
  printf("\n");

  for (i = 1; i <= nFrogs; i ++)
    for (j = 1; j <= i; j ++)
      if (j == i)
        Print_symbol(walk, i % 2);
      else
        Print_symbol(jump, i % 2);
  for (j = 1; j <= nFrogs; j ++)
    Print_symbol(jump, ! (nFrogs % 2));
  for (i = nFrogs; i >= 1; i --)
    for (j = i; j >= 1; j --)
      if (j == i)
        Print_symbol(walk, i % 2);
      else
        Print_symbol(jump, i % 2);
  printf("\n");

  if (isInteractiveMode)
  {
    printf("\nPress <Enter> to exit...\n");
    while ((pauser = getchar()) != '\n' && pauser != EOF)
      ;
  }

  return 0;
}

void Print_symbol(char const *operation, int index)
{
  # ifdef PRINT_WITH_COLOR
  if (operation == walk)
    printf("\e[3%dm", ! index ? COLOR_RED : COLOR_BLUE);
  else if (operation == jump)
    printf("\e[3%d;47m", ! index ? COLOR_RED : COLOR_BLUE);
  # endif
  printf("%c", operation[index]);
  # ifdef PRINT_WITH_COLOR
  printf("\e[0m");
  # endif

  return;
}

