#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_SIZE 100
#define FILE_PATH "data.csv"
#define MAX_SONG_POOL 159

// Utilities
int getUserInput();
int clearscreen();
void listSong();

// Root Window
int rootWindow();
int rootCMDSelection();
int rootCMDExecution();

// Music Library Window
int MusicLibraryWindow();
int MusicLibraryCMDSelection();
int MusicLibraryCMDExecution();

void readFromCSV();
// Data Structure for an individual song

typedef struct Song
{
    char title[MAX_STRING_SIZE];
    char artist[MAX_STRING_SIZE];
    char album[MAX_STRING_SIZE];
    short int year;
    double duration;
    struct Song *next;
} Song;

Song *head, *temp;

// int pool_insert_index = 0;

void createSong(const char *title, const char *artist, const char *album, const short int year, const double duration)
{
    if (head == NULL)
    {
        head = malloc(sizeof(Song));
        strcpy(head->title, title);
        strcpy(head->artist, artist);
        strcpy(head->album, album);
        head->year = year;
        head->duration = duration;
        temp = head;
    }
    else
    {
        Song *newnode = malloc(sizeof(Song));
        strcpy(newnode->title, title);
        strcpy(newnode->artist, artist);
        strcpy(newnode->album, album);
        newnode->year = year;
        newnode->duration = duration;
        temp->next = newnode;
        temp = newnode;
    }
    // printf("Added: %s\n", title);
}

int main()
{
    readFromCSV(); // Initialize data
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
    printf("[2] About\n\n");
}

int rootCMDExecution(int input)
{
    switch (input)
    {
    case 1:
        clearscreen();
        MusicLibraryWindow();
        break;

    case 2:;
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
    listSong();
    MusicLibraryCMDSelection();
    int input = getUserInput();
    MusicLibraryCMDExecution(input);
}

int MusicLibraryCMDSelection()
{
    // basically kite nk list dulu lagu2nye.
    printf("[0] Next Page\n");
    printf("[1] Previous Page\n");
    printf("-----------------------\n");
    printf("[2] Select Song\n");
    printf("[3] Display queue\n");
    printf("[4] Remove from queue\n");
    printf("[0] Back to Main Menu\n\n");
}

int MusicLibraryCMDExecution(int input)
{
    switch (input)
    {
    case 1:
        system("cls");
        MusicLibraryCMDSelection();
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

void listSong()
{
    Song *tmp; // temporary struct obj to iterate all nodes
    if (head == NULL)
        return;
    tmp = head;
    while (tmp != NULL)
    {
        printf("%s %s %s %f\n", tmp->title, tmp->artist, tmp->album, tmp->duration);
        tmp = tmp->next; // Move to next node
    }
}
/** CSV Parser for mapping a data set of up to songs to the song pool. Songs must be present in the data.csv root directory.
 */
void readFromCSV()
{
    FILE *file = fopen(FILE_PATH, "r");
    char line[1000];
    if (file == NULL)
    {
        perror("******\nUnable to load songs from data source");
        printf("Please check if data.csv exists at the root directory of the application.\n");
    }
    else
    {
        char songtitle[MAX_STRING_SIZE];
        char artist[MAX_STRING_SIZE];
        char releaseYear[MAX_STRING_SIZE];
        int durationms;
        double duration = 0.0;
        char album[MAX_STRING_SIZE];
        int year;
        while (fgets(line, sizeof(line), file))
        {
            char *token;
            token = strtok(line, ","); // tokenizes current line with delimiter ',' and returns the first substring
            if (token == NULL)
                continue;
            strcpy(songtitle, token);
            token = strtok(NULL, ","); // get the next token
            strcpy(artist, token);
            token = strtok(NULL, ","); // get the next token
            strcpy(album, token);
            token = strtok(NULL, ","); // get the next token
            strcpy(releaseYear, token);
            sscanf(releaseYear, "%d", &year);
            token = strtok(NULL, ","); // get the next token
            char temp[MAX_STRING_SIZE];
            strcpy(temp, token);
            sscanf(temp, "%d", &durationms);
            duration = durationms / 60000.0;
            token = strtok(NULL, ","); // get the next token
            createSong(songtitle, artist, album, year, duration);
            // printf("%s %s %s %d %d\n",songtitle, artist, album, year, durationms);
        }
    }
    fclose(file);
}