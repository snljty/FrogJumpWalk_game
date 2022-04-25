# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>

# ifdef linux
  # include <termio.h>
  int getch();
  # define stricmp_unity(str1, str2) strcasecmp(str1, str2)
# else
  # ifdef WIN32
    # include <conio.h>
    # define stricmp_unity(str1, str2) stricmp(str1, str2)
  # endif
# endif

# define DeleteBoard(p)    free(p); p = NULL;

void SwapChar(char *p, char *q);

void PrintHint(void);

unsigned int AcquireSize(int argc, char const *argv[]);

char *InitializeBoard(unsigned int nFrogs);

bool IsFinished(char *board, unsigned int nFrogs);

void ResetBoard(char *board, unsigned int nFrogs, unsigned int *posBlankPointer);

void PauseGame(void);

void PrintBoard(char *board);

void PrintEndGame(void);

void PrintFinishGame(void);

void RunGame(char *board, unsigned int nFrogs);


int main(int argc, char const *argv[])
{
  char *board = NULL;
  unsigned int nFrogs = 0;

  /*  print game hint  */
  PrintHint();
  /*  acquire the size of the board  */
  nFrogs = AcquireSize(argc, argv);
  /*  initialize the board  */
  board = InitializeBoard(nFrogs);
  /*  run the game  */
  PrintBoard(board);
  RunGame(board, nFrogs);
  /*  delete the board  */
  DeleteBoard(board);

  return 0;
}


void SwapChar(char *p, char *q)
{
  char t;
  
  t = * p;
  * p = * q;
  * q = t;

  return;
}

void PrintHint(void)
{
  printf("This program is a game of frogs jumping or walking.\n");
  printf("\n");
  printf("The frogs '>' are heading right, and can only go right.\n");
  printf("The frogs '<' are heading  left, and can only go  left.\n");
  printf("'_' is a blank position.\n");
  printf("If a frog is on the side of the blank position, \n");
  printf("Then it can walk to the blank, \n");
  printf("as long as it is on the correct direction.\n");
  printf("Or if a frog is separated by another frog from the blank, \n");
  printf("it can jump over to the blank if in the correct direction.\n");
  printf("\n");
  printf("Your aim is to swap the positions of all frogs '>' and '<'.\n");
  printf("Press 'f' to walk a frog right, 'd' to jump a frog right, \n");
  printf("'j' to walf a frog right, 'k' to jump a frog left, \n");
  printf("and 'r' to reset the game.\n");
  printf("Press <Enter> to exit.\n");
  printf("\n");
}

unsigned int AcquireSize(int argc, char const *argv[])
{
  unsigned int nFrogs = 0;
  char c = '\0';
  unsigned int const nFrogsDef = 3;
  char buf[BUFSIZ + 1] = "";

  for (;;)
  {
    if (argc > 2)
    {
      fprintf(stderr, "At most 1 command argument should be provided, but got %d.\n", (argc - 1));
      fprintf(stderr, "Please use the interactive mode.\n");
    }
    else if (argc == 2)
    {
      if (! stricmp_unity(argv[1], "default"))
      {
        nFrogs = nFrogsDef;
        break;
      }
      if (sscanf(argv[1], "%u", & nFrogs) == 1)
      {
        if ((int)nFrogs > 0)
          break;
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
      {
        if (nFrogs)
          break;
        fprintf(stderr, "There should be at least one frog, please input again.\n");
      }
      else
        fprintf(stderr, "Unrecognizable number \"%s\", please input again.\n", buf);
    }  
    break;
  }
  printf("\n");
  printf("\n");

  return nFrogs;
}

char *InitializeBoard(unsigned int nFrogs)
{
  char *board = NULL;
  unsigned int i = 0;

  board = (char *)malloc((2 * nFrogs + 2) * sizeof(char));
  board[2 * nFrogs + 1] = '\0';
  for (i = 0; i < nFrogs; i ++)
    board[i] = '>';
  board[nFrogs] = '_';
  for (i = nFrogs + 1; i <= 2 * nFrogs; i ++)
    board[i] = '<';

  return board;
}

void ResetBoard(char *board, unsigned int nFrogs, unsigned int *posBlankPointer)
{
  unsigned int i = 0;

  for (i = 0; i < nFrogs; i ++)
    board[i] = '>';
  board[nFrogs] = '_';
  for (i = nFrogs + 1; i <= 2 * nFrogs; i ++)
    board[i] = '<';
  * posBlankPointer = nFrogs;

  return;
}

bool IsFinished(char *board, unsigned int nFrogs)
{
  unsigned int i = 0;

  for (i = 0; i < nFrogs; i ++)
    if (board[i] != '<')
      return false;
  if (board[nFrogs] != '_')
    return false;
  return true;
}

void PauseGame(void)
{
  char c;

  while ((c = getchar()) != '\n' && c != EOF)
    ;

  return;
}

void PrintBoard(char *board)
{
  unsigned int i = 0;
  unsigned int posBlank = 0;
  unsigned int j = 0;

  putchar('\r');
  for (;;)
  {
    if (! board[i])
      break;
    if (board[i] == '_')
      posBlank = i;
    printf("%c", board[i]);
    ++ i;
  }
  for (j = i; j > posBlank; -- j)
    printf("%c", '\b');

  return;
}

void PrintEndGame(void)
{
  printf("\n");
  printf("Game ended. Press <Enter> to exit.\n");
  PauseGame();

  return;
}

void PrintFinishGame(void)
{
  printf("\n");
  printf("Congratulations! You finished the game successfully!\n");
  printf("Press <Enter> to exit.\n");
  PauseGame();

  return;
}

void RunGame(char *board, unsigned int nFrogs)
{
  char c = '\0';
  unsigned int posBlank = nFrogs;
  bool ichanged = false;

  while (true)
  {
    c = getch();
    if ((c == '\r') || (c == EOF))
      break;

    ichanged = false;
    switch (c)
    {
    case 'f': /*  walk right  */
      if ((posBlank == 0) || (board[posBlank - 1] != '>'))
      {
        printf("\a");
        break;
      }
      ichanged = true;
      SwapChar(board + posBlank - 1, board + posBlank);
      posBlank --;
      break;
    case 'j': /*  walk left  */
      if ((posBlank == 2 * nFrogs) || (board[posBlank + 1] != '<'))
      {
        printf("\a");
        break;
      }
      ichanged = true;
      SwapChar(board + posBlank + 1, board + posBlank);
      posBlank ++;
      break;
    case 'd': /*  jump right  */
      if ((posBlank <= 1) || (board[posBlank - 2] != '>'))
      {
        printf("\a");
        break;
      }
      ichanged = true;
      SwapChar(board + posBlank - 2, board + posBlank);
      posBlank -= 2;
      break;
    case 'k': /*  walk left  */
      if ((posBlank >= 2 * nFrogs - 1) || (board[posBlank + 2] != '<'))
      {
        printf("\a");
        break;
      }
      ichanged = true;
      SwapChar(board + posBlank + 2, board + posBlank);
      posBlank += 2;
      break;
    case 'r': /*  reset  */
      ichanged = true;
      ResetBoard(board, nFrogs, & posBlank);
      break;
    default:
      printf("\a");
      break;
    }
    if (ichanged)
    {
      PrintBoard(board);
      if (IsFinished(board, nFrogs))
      {
        /*  game finished  */
        PrintFinishGame();
        return;
      }
    }
  }

  PrintEndGame();
  return;
}

# ifdef linux
  int getch()
  {
    struct termios tm, tm_old;
    int fd = 0, ch;

    if (tcgetattr(fd, & tm) < 0)
      return -1;
    tm_old = tm;
    cfmakeraw(& tm);
    if (tcsetattr(fd, TCSANOW, & tm) < 0)
      return -1;
    ch = getchar();
    if (tcsetattr(fd, TCSANOW, &tm_old) < 0)
      return -1;
  
    return ch;
  }
# endif

