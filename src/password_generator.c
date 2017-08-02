#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <ctype.h>
#include <time.h>
#include "pcg.h"
#define ALLOC(size, type) ((type*) malloc((size) * sizeof(type)))
#define USERNAME_LENGTH 31
#define TYPE_LENGTH 11
#define NAME_LENGTH 51
#define SUCC_INP 0
#define UNSUCC_INP 1
#define TOO_LONG 2

static const char upper[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char lower[] = "abcdefghijklmnopqrstuvwxyz";
static const char numbers[] = "0123456789";
static const char symbols[] = "\"`~!@#$^&*()_+-={}|[]:;'<>?,./%";  
static const char* table[] = { upper, lower, numbers, symbols };
static const size_t lengths[] = { strlen(upper), strlen(lower), 
                              strlen(numbers), strlen(symbols) };

typedef struct user_info
{
  char username[USERNAME_LENGTH];
  char type_name[NAME_LENGTH];
  char type[TYPE_LENGTH];
} info_t;

static int getLine(char*, char*, size_t);
char* generate_password(size_t);
void write_in_file(FILE*, info_t*, char*);
void help(void);

int main(int argc, char* argv[])
{
  FILE* fp;
  int nondeterministic = 0;
  int opterr = 0;
  int flag = 1;
  int c;
  info_t* info = ALLOC(1, info_t);
  if (argc <= 2)
  {
    fprintf(stderr, "\nYou should provide more options\n\n");
    help();
    exit(EXIT_FAILURE);
  }
  while (flag && (c = getopt(argc, argv, "+r::w::g::h::")) != -1) 
  {
    switch (c)
    {
      case 'h':
        help();
        return EXIT_SUCCESS;
      case 'r': 
        nondeterministic = 1; 
      break;
      case 'w':
        strncpy(info->type, "Website", sizeof(info->type));
        getLine("Please enter your username: ", 
          info->username, sizeof(info->username));
        getLine("Please enter the name of the website (not the whole link): ",
          info->type_name, sizeof(info->type_name));
        flag = 0;
      break;
      case 'g':
        strncpy(info->type, "Game", sizeof(info->type));
        getLine("Please enter your username: ",
          info->username, sizeof(info->username));
        getLine("Please enter the name of the game: ", 
          info->type_name, sizeof(info->type_name));
        flag = 0;
      break;
      case '?': 
        if (isprint(optopt))
        {
          fprintf(stderr, "Unkown option -%c.\n", optopt);
        }
        else
        {
          fprintf(stderr, "Unkown option character '\\x%x'.\n", optopt);
        }
        help();
        exit(EXIT_FAILURE);
      default:
        abort();
    }
  }
  if (nondeterministic) 
  {
    pcg32_srandom(time(NULL) ^ (intptr_t) &printf, (intptr_t) &opterr);
  } 
  else 
  {
    pcg32_srandom(42U, 54U);  
  }
  fp = fopen("passwords.txt", "ab");
  if (fp == NULL)
  {
    perror("Error: ");
    exit(EXIT_FAILURE);
  }
  int bound;
  printf("Please enter the length of the password that you would like to generate: ");
  scanf("%d", &bound);
  printf("Generating password...\n");
  char* password = generate_password(bound);
  printf("Done!\n");
  printf("Writing in file...\n");
  write_in_file(fp, info, password);
  printf("Done!\n");
  printf("You can find your password in the file \"passwords.txt\""
    " including your username and the rest of your information\n");
  fclose(fp);
  free(password);
  free(info);
  return EXIT_SUCCESS;
}

char* generate_password(size_t bound)
{
  char* password = ALLOC(bound + 1, char);
  for (size_t i = 0; i != bound; ++i)
  {
    uint32_t index_t = pcg32_boundedrand(4);
    uint32_t index_l = pcg32_boundedrand(lengths[index_t]); 
    password[i] = table[index_t][index_l];
  }
  return password;
}

static int getLine(char* prompt, char* buffer, size_t size)
{
  int ch, extra;
  if (prompt != NULL)
  {
    printf("%s", prompt);
    fflush(stdout);
  }
  if (fgets(buffer, size, stdin) == NULL) return UNSUCC_INP;
  if (buffer[strlen(buffer) - 1] != '\n')
  {
    extra = 0;
    while ((ch = getchar()) != '\n' && (ch != EOF)) extra = 1;
    return (extra == 1) ? TOO_LONG : SUCC_INP;
  }
  buffer[strlen(buffer) - 1] = '\0';
  return SUCC_INP;
}

void write_in_file(FILE* fp, info_t* info, char* password)
{
  fprintf(fp, "Username: %s\nPassword: %s\nType: %s\nName: %s\n",
          info->username, password, info->type, info->type_name);
  fputc('\n', fp);
}

void help(void)
{
  fprintf(stderr, "See program usage below:\n\n");
  fprintf(stderr, "You can use this program by passing 1 of the 3"
          " parameters available (-r, -w, -g).\n");
  fprintf(stderr, "e.g: ./password_generator -r -w\n");
  fprintf(stderr, "The parameter -r forces the seed to be random (you should always use -r for"
          " better results)\n");
  fprintf(stderr, "The parameter -w stands for website\n"
          "The parameter -g stands for game\n"
          "There is no functional difference between the two, they are just for"
          " the user's distinction for the rest of the information provided\n\n");
}