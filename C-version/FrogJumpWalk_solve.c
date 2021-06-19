# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include <ctype.h>

/*  This program show solutions of the game frog_walk_or_jump.  */

int main(int argc, char const *argv[])
{
  unsigned int nFrogs = 0;
  const char c[] = {'A', 'B'};
  unsigned int i = 0, j = 0;
  char pauser = '\0';
  unsigned int const nFrogsDef = 3;
  char buf[BUFSIZ + 1] = "";
  bool isInteractiveMode = true;

  for (;;)
  {
    if (argc > 2)
    {
      printf("At most 1 command argument should be provided, but got %d.\n", (argc - 1));
      puts("Please use the interactive mode.");
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
        printf("A positive number for amount of frogs is required, but got %d.\n", (int)nFrogs);
        puts("Please use the interactive mode.");
      }
      else
        printf("Unrecognizable number \"%s\", please use the interactive mode.\n", argv[1]);
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
      printf("Unrecognizable number \"%s\", please input again.\n", buf);
    }  
    break;
  }
  
  puts("");
  puts("a/A for left side and b/B for right side.");
  puts("Upper letters for jumping and lower letters for walking.");
  puts("");

  for (i = 1; i <= nFrogs; i ++)
    for (j = 1; j <= i; j ++)
      if (j == i)
        putchar(tolower(c[! (i % 2)]));
      else
        putchar(c[! (i % 2)]);
  for (j = 1; j <= nFrogs; j ++)
    putchar(c[nFrogs % 2]);
  for (i = nFrogs; i >= 1; i --)
    for (j = i; j >= 1; j --)
      if (j == i)
        putchar(tolower(c[! (i % 2)]));
      else
        putchar(c[! (i % 2)]);
  putchar('\n');

  for (i = 1; i <= nFrogs; i ++)
    for (j = 1; j <= i; j ++)
      if (j == i)
        putchar(tolower(c[i % 2]));
      else
        putchar(c[i % 2]);
  for (j = 1; j <= nFrogs; j ++)
    putchar(c[! (nFrogs % 2)]);
  for (i = nFrogs; i >= 1; i --)
    for (j = i; j >= 1; j --)
      if (j == i)
        putchar(tolower(c[i % 2]));
      else
        putchar(c[i % 2]);
  putchar('\n');

  if (isInteractiveMode)
  {
    puts("\nPress <Enter> to exit...");
    while ((pauser = getchar()) != '\n' && pauser != EOF)
      ;
  }

  return 0;
}

