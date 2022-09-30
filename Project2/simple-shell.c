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

    /**
     * Check if they are using history
     */

    if (inputBuffer[0] == '!')
    {
        if (inputBuffer[1] != '\n')
        {
            command_number = atoi(&inputBuffer[1]);
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
        addtohistory(inputBuffer);
    }

    /**
     * Parse the contents of inputBuffer
     */

    int argc = 0;            // the number of arguments
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

        case '\n':                                                           /* should be the final char examined */
            letter = '\0';                                                   // store the letter
            concatedArg = (char *)malloc(sizeof(char) * strlen(args[argc])); // allocate memory for the argument (plus the new letter)
            strcpy(concatedArg, args[argc]);                                 // copy the argument into the new "array" called concatedArg
            concatedArg[strlen(concatedArg)] = letter;                       // add the new letter to the end of concatedArg
            args[argc] = concatedArg;                                        // store concatedArg in the args array, replacing the old incomplete argument
            inWord = 0;                                                      // we are not in a word anymore, so set inWord to 0
            argc++;                                                          // increment the number of arguments
            break;

        default: /* some other character */
            if (inputBuffer[i] == '&')
            {
                // the inputBuffer[i] is '&', set up background flag
                *background = 1;
            }
            else
            {
                if (inWord == 0) // we are not "in a word", meaning we are at the start of a word
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

        } /* end of switch */
    }     /* end of for */

    /*
    for (int as = 0; as < argc; as++)
    {
        printf("args[%d] = %s", as, args[as]);
        printf("\n");
    }
    */

    memset(inputBuffer, 0, sizeof &inputBuffer); // clear the inputBuffer array before use
    return 1;

} /* end of setup routine */

int main(void)
{
    char inputBuffer[MAX_LINE];   /* buffer to hold the command entered */
    int background;               /* equals 1 if a command is followed by '&' */
    char *args[MAX_LINE / 2 + 1]; /* command line (of 80) has max of 40 arguments */
    pid_t child;                  /* process id of the child process */

    // define your local variables here, at the beginning of your program.

    int shouldrun = 1;

    while (shouldrun)
    { /* Program terminates normally inside setup */
        background = 0;

        // reset the args array
        for (int i = 0; i < MAX_LINE / 2 + 1; i++)
        {
            args[i] = NULL;
        }
        shouldrun = setup(inputBuffer, args, &background); /* get next command */

        if (strncmp(args[0], "exit", 4) == 0)
        {
            return 0;
        }

        if (strncmp(args[0], "history", 7) == 0)
        {
            displayhistory();
            continue;
        }

        if (shouldrun) // if there is no error and can return 1
        {
            if ((child = fork()) == 0)
            {
                execvp(args[0], args);
                exit(0); // kills the child
            }
            else if (!background)
            { // parent
                waitpid(child, 0, 0);
            }
        }
    }

    return 0;
}
