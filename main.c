#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Utilities
int getUserInput();
int clearscreen();

// Root Window
int rootWindow();
int rootCMDSelection();
int rootCMDExecution();

// Music Library Window
int MusicLibraryWindow();
int MusicLibraryCMDSelection();
int MusicLibraryCMDExecution();

int main()
{
    rootWindow();
}

int getUserInput()
{
    int input;
    printf("Choose your selection: ");
    scanf(" %d", &input);
    return input;
}

int clearscreen()
{
    return system("cls");
}

int rootWindow()
{
    printf("Digital Music Library\n\n");
    rootCMDSelection();
    int userInput = getUserInput();
    rootCMDExecution(userInput);
}

int rootCMDSelection() // Root Window
{
    printf("[1] Music Library\n");
    printf("[2] Scan music folder\n");
    printf("[3] Add music folder\n");
    printf("[4] About\n\n");
}

int rootCMDExecution(int input)
{
    switch (input)
    {
    case 1:
        clearscreen();
        MusicLibraryWindow();
        break;

    case 2:

        break;
    case 3:

        break;

    case 4:

        break;
    default:
        printf("\nUnknown selection! try again\n");
        input = getUserInput();
        rootCMDExecution(input); // recursion
        break;
    }
}

int MusicLibraryWindow() // Window 2
{
    MusicLibraryCMDSelection();
    int input = getUserInput();
    MusicLibraryCMDExecution(input);
}

int MusicLibraryCMDSelection()
{
    // basically kite nk list dulu lagu2nye.
    printf("[1] Select Song\n");
    printf("[2] Display queue\n");
    printf("[3] Remove from queue\n");
    printf("[4] Display queue\n\n");
    printf("[0] Back to Main Menu\n\n");
}

int MusicLibraryCMDExecution(int input)
{
    switch (input)
    {
    case 1:
        system("cls");
        break;

    case 2:

        break;
    case 3:

        break;

    case 4:

        break;
    case 0:
        clearscreen();
        rootWindow();
        break;

    default:
        printf("\nUnknown selection! try again\n");
        input = getUserInput();
        MusicLibraryCMDExecution(input); // recursion
        break;
    }
}