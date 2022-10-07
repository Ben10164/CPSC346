/**
 *version control and author
 *Ben Puryear
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

#define MAX_LINE 80    /* 80 chars per line, per command, should be enough. */
#define MAX_COMMANDS 9 /* size of history */

char history[MAX_COMMANDS][MAX_LINE]; // the array used to store history commands.
char display_history[MAX_COMMANDS][MAX_LINE];
/*the array used for "printf" to display history nicely. Remove special characters like "\n" or "\0"*/

int command_count = 0;

/**
 * Add the most recent command to the history.
 */

void addtohistory(char inputBuffer[])
{

    // update array"history": add the command to history, strcpy(str1,str2);
    strcpy(history[command_count % MAX_COMMANDS], inputBuffer);
    // update array"display_history": remove characters like '\n', '\0' in order to display nicely
    strcpy(display_history[command_count % MAX_COMMANDS], inputBuffer);
    // find and replace all \n with \0 (no clue how there would be more than 1 though, but better safe than sorry)
    for (int i = 0; i < sizeof display_history[command_count % MAX_COMMANDS]; i++)
    {
        if (display_history[command_count][strlen(display_history[command_count]) - i] == '\n')
        {
            display_history[command_count][strlen(display_history[command_count]) - i] = '\0';
        }
    }
    command_count++;
    return;
}

void displayhistory()
{
    int i;
    if (command_count == 0)
    {
        printf("No commands in history.\n");
    }
    for (i = 0; i < command_count; i++)
    {
        printf("%d: %s\n", i, display_history[i]);
    }
}

int setup(char inputBuffer[], char *args[], int *background)
{

    int length,         /* # of characters in the command line */
        i,              /* loop index for accessing inputBuffer array */
        command_number; /* index of requested command number */

    // define your local varialbes here;

    /* read what the user enters on the command line */
    do
    {
        printf("osh>");
        fflush(stdout);
        length = read(STDIN_FILENO, inputBuffer, MAX_LINE);
    } while (inputBuffer[0] == '\n'); /* swallow newline characters */

    if (length == 0)
        exit(0); /* ^d was entered, end of user command stream */

    if ((length < 0))
    {
        perror("error reading the command");
        exit(-1); /* terminate with error code of -1 */
    }

    // history handling
    if (inputBuffer[0] == '!')
    {
        if (inputBuffer[1] != '\n')
        {
            if (inputBuffer[1] == '!')
            {
                command_number = command_count - 1;
            }
            else
            {
                // if the user enters anything not a number, command_number will be 0 (aka the most recent one)
                command_number = atoi(&inputBuffer[1]);
            }
            if (command_number < command_count)
            {
                printf("You are repeating command %d: %s\n", command_number, display_history[command_number]);
                strcpy(inputBuffer, history[command_number]);
            }
            else
            {
                printf("Command not found in history. Pick one of these:\n");
                displayhistory();
            }
        }
        else
        {
            displayhistory();
        }
    }
    else
    {
        // if it doesnt start with a !, then it is a normal command, so we add it to history
        // note how we dont add the history command to history, so it doesnt clutter up the history and you could repeat it with the same result
        if (strncmp(inputBuffer, "history", 7) == 0)
        {
            displayhistory();
            return 1;
        }
        else if (strncmp(inputBuffer, "exit", 4) == 0)
        {
            return 0;
        }
        else
        {
            addtohistory(inputBuffer);
        }
    }

    // if the code has reached this point, then it is a normal command (not history or exit), so we can continue
    // and if it was a !# command, it has already been replaced with the actual command

    int argc = 0;            // th e number of arguments
    int inWord = 0;          // "bool" to indicate if we are in a word or not
    char firstLetter;        // used to store the first letter of an argument (format: char)
    char letter;             // used to store the first letter of an argument that is not the first letter (format: char)
    char *firstLetterString; // used to store the first letter of an argument (format: char*)
    char *concatedArg;       // used to store the argument before and after concatenation

    for (i = 0; i < length; i++)
    {
        /* examine every character in the inputBuffer */
        switch (inputBuffer[i])
        {
        case ' ':
            // move onto default
        case '\t': /* argument separators */

            letter = '\0';                                                   // store the letter
            concatedArg = (char *)malloc(sizeof(char) * strlen(args[argc])); // allocate memory for the argument (plus the new letter)
            strcpy(concatedArg, args[argc]);                                 // copy the argument into the new "array" called concatedArg
            concatedArg[strlen(concatedArg)] = letter;                       // add the new letter to the end of concatedArg
            args[argc] = concatedArg;                                        // store concatedArg in the args array, replacing the old incomplete argument
            inWord = 0;                                                      // we are not in a word anymore, so set inWord to 0
            argc++;                                                          // increment the number of arguments
            break;

        case '\n': /* should be the final char examined */
            letter = '\0';
            if (args[argc] != NULL)
            {

                concatedArg = (char *)malloc(sizeof(char) * strlen(args[argc])); // allocate memory for the argument (plus the new letter)
                strcpy(concatedArg, args[argc]);                                 // copy the argument into the new "array" called concatedArg
                concatedArg[strlen(concatedArg)] = letter;                       // add the new letter to the end of concatedArg
                args[argc] = concatedArg;                                        // store concatedArg in the args array, replacing the old incomplete argument
                inWord = 0;                                                      // we are not in a word anymore, so set inWord to 0
                argc++;                                                          // increment the number of arguments
            }
            else
            {
                args[argc] = NULL; // set the last argument to NULL
            }                      
            break;

        default:
            if (inputBuffer[i] == '&')
            {
                // the inputBuffer[i] is '&', set up background flag
                *background = 1;
            }
            else
            {
                // if inWord is 0, we are not "in a word", meaning we are at the start of a word
                if (inWord == 0)
                {
                    firstLetter = inputBuffer[i];                                            // store the first letter of the word
                    firstLetterString = (char *)malloc(sizeof(char) * strlen(&firstLetter)); // allocate memory for the first letter as an "array" of chars
                    firstLetterString[0] = firstLetter;                                      // store the first letter in the "array"
                    args[argc] = firstLetterString;                                          // store the "array" in the args array
                    inWord = 1;                                                              // we are now in a word, so we set inWord to 1
                }
                else
                {
                    letter = inputBuffer[i];                                         // store the letter
                    concatedArg = (char *)malloc(sizeof(char) * strlen(args[argc])); // allocate memory for the argument (plus the new letter)
                    strcpy(concatedArg, args[argc]);                                 // copy the argument into the new "array" called concatedArg
                    concatedArg[strlen(concatedArg)] = letter;                       // add the new letter to the end of concatedArg
                    args[argc] = concatedArg;                                        // store concatedArg in the args array, replacing the old incomplete argument
                }
            }
        }
    }

    // for testing purposes
    // for (int argument = 0; argument <= argc; argument++)
    // {
    //     printf("args[%d] = %s\n", argument, args[argument]);
    // }

    memset(inputBuffer, 0, sizeof &inputBuffer); // clear the inputBuffer array before use
    return 1;
}

int main(void)
{
    char inputBuffer[MAX_LINE];   /* buffer to hold the command entered */
    int background;               /* equals 1 if a command is followed by '&' */
    char *args[MAX_LINE / 2 + 1]; /* command line (of 80) has max of 40 arguments */
    pid_t child;                  /* process id of the child process */

    int shouldrun = 1;

    while (shouldrun)
    {
        background = 0;

        // reset the args array so commands dont bleed into each other
        for (int i = 0; i < MAX_LINE / 2 + 1; i++)
        {
            args[i] = NULL;
        }

        shouldrun = setup(inputBuffer, args, &background);

        if (shouldrun) // if there is no error and can return 1
        {
            if ((child = fork()) == 0)
            {
                execvp(args[0], args);
                exit(0); // kills the child
            }
            else if (!background)
            {
                // parent waits for foreground job to terminate
                waitpid(child, 0, 0);
            }
        }
    }

    return 0;
}