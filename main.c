#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define MAX_STRING_SIZE 100
#define FILE_PATH "data.csv"
#define MAX_SONG_POOL 159

// Utilities
int getUserInput();
int clearscreen();
void pagingDisplay();
void pagingManager();
void selectSong();
void Enqueue(int);
int displayQueue();
void destroyQueue();
void QueueManager();
void FindSongByTitle();
void FindSongByArtist();
void FindSongByAlbum();
void LinearSearchString(char *, int);
int displayMatchedSongs();
void destroyList();

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
typedef struct Song *songptr;
typedef struct Song
{
    char title[MAX_STRING_SIZE];
    char artist[MAX_STRING_SIZE];
    char album[MAX_STRING_SIZE];
    short int year;
    double duration;
    songptr next; // store next address
} Song;

Song *head, *temp;

// Create song pool array to store all song by index
songptr song_pool[MAX_SONG_POOL];

// Data structure for queue
typedef struct Queue *node_ptr;
typedef struct Queue
{
    songptr song;       // Nested structure implemented
    node_ptr next_song; // store next address
} Queue;

node_ptr front, rear;

// Data structure for List (holder to store filtered library)
typedef struct List *next;
typedef struct List
{
    songptr song;   // Nested structure implemented
    int id;         // song id
    next next_song; // store next address
} List;

next q_head, q_tmp;

int pool_insert_index = 0;
int paging_count = 0;

Song *createSong(const char *title, const char *artist, const char *album, const short int year, const double duration)
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
    pool_insert_index++;
    // printf("Added: %s\n", title);
    return temp;
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
        MusicLibraryWindow(0);
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

int MusicLibraryWindow(int page_state) // Window 2
{
    pagingManager(page_state); // 0 is reset, 1 is next, -1 is prev
    MusicLibraryCMDSelection();
    int input = getUserInput();
    MusicLibraryCMDExecution(input);
}

int MusicLibraryCMDSelection()
{
    printf("[-1] Next Page\n");
    printf("[-2] Previous Page\n\n");
    printf("-----------------------\n");
    printf("[1] Select Song\n");
    printf("-----------------------\n");
    printf("[2] Search song by title\n");
    printf("[3] Search song by artist\n");
    printf("[4] Search song by album\n");
    printf("-----------------------\n");
    printf("[5] Play Queue\n");
    printf("[6] Queue Manager\n");
    printf("-----------------------\n");
    printf("[7] Sort library by Artist\n");
    printf("[8] Sort library by Album\n");
    printf("[9] Sort library by Release Year\n");
    printf("-----------------------\n");
    printf("\n[0] Back to Main Menu\n\n");
}

int MusicLibraryCMDExecution(int input)
{
    switch (input)
    {
    case -1:
        MusicLibraryWindow(1); // Next page
        break;
    case -2:
        MusicLibraryWindow(-1); // Prev Page
        break;
    case 1:
        selectSong();
        break;
    case 2:
        FindSongByTitle();
        MusicLibraryWindow(0);
        break;
    case 3:
        FindSongByArtist();
        MusicLibraryWindow(0);
        break;
    case 4:
        FindSongByAlbum();
        MusicLibraryWindow(0);
        break;
    case 6:
        QueueManager();
        MusicLibraryWindow(0);
        break;
    case 7:
        destroyQueue();
        MusicLibraryWindow(0); // Next page
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

void QueueManager()
{
    clearscreen();
    if (displayQueue())
    {
        printf("[1] Delete queue\n");
        printf("[0] Back to menu\n\n");
        int input = getUserInput();
        switch (input)
        {
        case 1:
            destroyQueue();
            break;
        default:
            return; // exit
            break;
        }
    }
}

void FindSongByTitle()
{
    char input[MAX_STRING_SIZE];
    printf("\nInsert title name : ");
    scanf(" %[^\n]s", input); // accept space
    LinearSearchString(input, 1);
    clearscreen();
    if (displayMatchedSongs())
    {
        printf("[1] Select Song\n");
        printf("[0] Back to menu\n\n");
        int input = getUserInput();
        switch (input)
        {
        case 1:
            selectSong();
            break;
        default:
            return; // exit
            break;
        }
    }
}

void FindSongByArtist()
{
    char input[MAX_STRING_SIZE];
    printf("\nInsert artist name : ");
    scanf(" %[^\n]s", input); // accept space
    LinearSearchString(input, 2);
    clearscreen();
    if (displayMatchedSongs())
    {
        printf("[1] Select Song\n");
        printf("[0] Back to menu\n\n");
        int input = getUserInput();
        switch (input)
        {
        case 1:
            selectSong();
            break;
        default:
            return; // exit
            break;
        }
    }
}

void FindSongByAlbum()
{
    char input[MAX_STRING_SIZE];
    printf("\nInsert album name : ");
    scanf(" %[^\n]s", input); // accept space
    LinearSearchString(input, 3);
    clearscreen();
    if (displayMatchedSongs())
    {
        printf("[1] Select Song\n");
        printf("[0] Back to menu\n\n");
        int input = getUserInput();
        switch (input)
        {
        case 1:
            selectSong();
            break;
        default:
            return; // exit
            break;
        }
    }
}

int displayMatchedSongs()
{
    next tmp;
    if (q_head == NULL)
    {
        printf("\n\nNo songs found. Returning back to menu");
        Sleep(2000);
        return 0;
    }
    tmp = q_head;
    int i = 0;
    printf("Match Songs\n");
    printf("-------------------------------------------------------------\n");
    printf("   #   |  Title                          |  Artist          |  \n");
    printf("-------------------------------------------------------------\n");
    while (tmp != NULL)
    {
        printf("  %2d   |  %-30.30s |  %-15.15s |\n", (tmp->id + 1), tmp->song->title, tmp->song->artist);
        tmp = tmp->next_song;
        i++;
    }
    printf("-------------------------------------------------------------\n\n");
    destroyList(); // we destroy the list after display
    return 1;
}

void destroyList()
{
    next current, next;
    if (q_head == NULL)
    {
        return;
    }
    current = q_head;
    while (current != NULL)
    {
        next = current->next_song;
        free(current);
        current = next;
    }
    q_head = NULL;
}

// Applying linear sequential search but based on string with linked list implementation
void LinearSearchString(char *target, int attribute)
{
    int ret;
    for (int i = 0; i < MAX_SONG_POOL && song_pool[i] != NULL; i++)
    {
        switch (attribute)
        {
        case 1:
            ret = strcmp(song_pool[i]->title, target);
            break;
        case 2:
            ret = strcmp(song_pool[i]->artist, target);
            break;
        case 3:
            ret = strcmp(song_pool[i]->album, target);
            break;
        }
        if (ret == 0) // compared string match
        {
            if (q_head == NULL)
            {
                q_head = malloc(sizeof(List));
                q_head->song = song_pool[i];
                q_head->id = i;
                q_head->next_song = NULL;
                q_tmp = q_head;
            }
            else
            {
                List *new_node = malloc(sizeof(List));
                new_node->song = song_pool[i];
                new_node->id = i;
                new_node->next_song = NULL;
                q_tmp->next_song = new_node;
                q_tmp = new_node;
            }
        }
    }
}

void selectSong()
{
    int id, input;
    printf("\nInsert song number(#): ");
    scanf(" %d", &id);
    id = id - 1;                      // Change to actual index number
    if (id < 0 || id > MAX_SONG_POOL) // if song chosen is not in the song pool
    {
        printf("Invalid number! Returning back to menu");
        Sleep(2000);
        MusicLibraryWindow(0);
    }
    printf("\nSelected Track: %s by %s\n\n", song_pool[id]->title, song_pool[id]->artist);
    printf("[1] Add to queue\n");
    printf("[0] Back to menu\n");
    printf("\nWhat do you want to do?\n");
    scanf("%d", &input);
    switch (input)
    {
    case 1:
        Enqueue(id);
        MusicLibraryWindow(0); // Return to menu and reset the page
        break;
    case 0:
        MusicLibraryWindow(0); // Return to menu and reset the page
        break;
    }
}

void Enqueue(int id)
{
    node_ptr new_ptr = malloc(sizeof(Queue));
    new_ptr->song = song_pool[id];
    new_ptr->next_song = NULL;
    if (front == NULL)
    {
        front = new_ptr;
        rear = new_ptr;
    }
    else
    {
        rear->next_song = new_ptr;
        rear = new_ptr;
    }
    printf("\n\nSuccessfully added to queue!");
    Sleep(1000);
}

void destroyQueue()
{
    node_ptr current, next;
    if (front == NULL)
    {
        printf("\n\nQueue is empty. Returning back to menu");
        Sleep(2000);
        return;
    }
    current = front;
    while (current != NULL)
    {
        next = current->next_song;
        free(current);
        current = next;
    }
    front = NULL;
    printf("\n\nQueue is destroyed. Returning back to menu");
    Sleep(2000);
}

int displayQueue()
{
    node_ptr tmp;
    if (front == NULL)
    {
        printf("\n\nQueue is empty. Returning back to menu");
        Sleep(2000);
        return 0;
    }
    tmp = front;
    int i = 0;
    printf("In queue\n");
    printf("-------------------------------------------------------------\n");
    printf("   #   |  Title                          |  Artist          |  \n");
    printf("-------------------------------------------------------------\n");
    while (tmp != NULL)
    {
        printf("  %2d   |  %-30.30s |  %-15.15s |\n", (i + 1), tmp->song->title, tmp->song->artist);
        tmp = tmp->next_song;
        i++;
    }
    printf("-------------------------------------------------------------\n\n");
    return 1;
}

void pagingManager(int state)
{
    int step = 10;
    switch (state)
    {
    case 0:               // reset page
        paging_count = 0; // Reset page
        pagingManager(1); // Recursively call to initialize new paging
        break;
    case 1: // next page
        paging_count += step;
        clearscreen();
        pagingDisplay(paging_count);
        break;
    case -1: // prev page
        paging_count -= step;
        if (paging_count <= step)
        {
            paging_count = step;
        }
        clearscreen();
        pagingDisplay(paging_count);
        break;
    }
}

void pagingDisplay(int step)
{
    printf("-----------------------------------------------------------------------------------------------------------\n");
    printf("                                                   Music Tracks\n");
    printf("-----------------------------------------------------------------------------------------------------------\n");
    printf("   #   |  Title                          |  Artist          |  Album                          |  Duration \n");
    printf("-----------------------------------------------------------------------------------------------------------\n");
    for (int i = step - 10; i < step && song_pool[i] != NULL; i++)
    {
        printf("  %2d   |  %-30.30s |  %-15.15s |  %-30.30s |  %2.2lf min\n", (i + 1), song_pool[i]->title, song_pool[i]->artist, song_pool[i]->album, song_pool[i]->duration);
    }
    printf("-----------------------------------------------------------------------------------------------------------\n");
}

/** CSV Parser for mapping a data set of up to songs to the song pool. Songs must be present in the data.csv root directory. */
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
            if (token == NULL || !strcmp(token, "track_name"))
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
            song_pool[pool_insert_index] = createSong(songtitle, artist, album, year, duration);
            // printf("%s %s %s %d %d\n",songtitle, artist, album, year, durationms);
        }
    }
    fclose(file);
}