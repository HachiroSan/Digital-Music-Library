#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <Conio.h>

#define MAX_STRING_SIZE 100
#define FILE_PATH "data.csv" // data.csv need to exist in current directory
#define MAX_SONG_POOL 159    // Number is fixed. There is 159 songs in data.csv

// Main Modules
// We split into 2 windows, root and music library

// Root Window
int rootWindow();
int rootCMDSelection();
int rootCMDExecution();
void About();
// Music Library Window
int MusicLibraryWindow();
int MusicLibraryCMDSelection();
int MusicLibraryCMDExecution();
void pagingManager();
void pagingDisplay();
void selectSong();
int PlayQueue();
int displayQueue();
void QueueManager();
void FindSongByTitle();
void FindSongByArtist();
void FindSongByAlbum();
int displayMatchedSongs();
void destroyList();
void TitleSort();
void ArtistSort();
void YearSort();
void EditMetadata(int);
// Utilities
int clearscreen();
int getUserInput();
int alphabet_to_num(char);
void ResetPoolID();
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

Song *head, *temp; // for song

/* song pool array to store all nodes address by index (faster alternative to access linked list nodes),
    it basically stores the address of each nodes from Song linked list. It will be easier to iterate and edit
    nodes inside linked list */
songptr song_pool[MAX_SONG_POOL]; 

// Algorithm-related
void Enqueue(int);
void Dequeue();
void destroyQueue();
void LinearSearchString(char *, int); 
void FrontBackSplit(Song *, Song **, Song **);
void MergeSort(Song **, int); 
Song *SortedMerge(Song *, Song *, int);

// Data structure for queue
typedef struct Queue *node_ptr;
typedef struct Queue
{
    Song *song;         // Nested structure implemented
    node_ptr next_song; // store next address
} Queue;

node_ptr front, rear; // for queue

// Data structure for List (holder to store filtered library)
typedef struct List *next;
typedef struct List
{
    songptr song;
    int id;         // song id
    next next_song; // store next address
} List;

next q_head, q_tmp; // for list

int pool_insert_index = 0; // log index to convert linked list into array based
int paging_count = 0;      // total number of songs displayed in each page

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
    return temp;
}

/* sorts the linked list by changing next pointers (not data) (Merge Sort Implementation)*/
Song *SortedMerge(Song *a, Song *b, int type)
{
    Song *result = NULL;
    int p, q;
    /* Base cases */
    if (a == NULL)
        return (b);
    else if (b == NULL)
        return (a);

    switch (type)
    {
        case 1: // Sort by title
            p = alphabet_to_num(a->title[0]);
            q = alphabet_to_num(b->title[0]);
            break;
        case 2: // Sort by artist
            p = alphabet_to_num(a->artist[0]);
            q = alphabet_to_num(b->artist[0]);
            break;
        case 3: // Sort by year
            p = a->year;
            q = b->year;
            break;
    }

    /* Pick either a or b, and recur */
    if (p <= q)
    {
        result = a;
        result->next = SortedMerge(a->next, b, type);
    }
    else
    {
        result = b;
        result->next = SortedMerge(a, b->next, type);
    }
    return (result);
}


int main()
{
    readFromCSV(); // Initialize data
    rootWindow();
}

int rootWindow()
{
    printf("\nPRISMANE\n-NextGen Digital Music Library-\n\n");
    rootCMDSelection(); // display list of selections
    int userInput = getUserInput(); // ask user input
    rootCMDExecution(userInput);    // pass to execution
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
    case 1: // Music Library
        clearscreen();
        MusicLibraryWindow(0);
        break;

    case 2: // About
        clearscreen();
        About();
        printf("Press Any Key to Continue...");
        _getch();   // getchar
        clearscreen();
        rootWindow();
        break;
    default:
        printf("\nUnknown selection! try again\n");
        input = getUserInput();
        rootCMDExecution(input); // recursion
        break;
    }
}

/* About in root window*/
void About()
{
    printf("Group 2\nData Structure & Algorithms Subject\nUniversiti Malaysia Pahang\n4/2/2022\n\n");
    printf("Team Members:\n");
    printf("1. Muhammad Farhad Bin Khaharruddin    SD20028\n");
    printf("2. Hafiz Hakimi Bin Mohd Rasani        SD20060\n");
    printf("3. Muhammad Akram Bin Mohamad A'sim    SD20013\n");
    printf("4. Aizat Haikal Bin Apandi             SD20006\n");
    printf("5. Muhammad 'arif Bin Mohd Anuwa       SD20039\n\n");
}

/* About in root window*/
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
    printf("[5] Play Songs in Queue\n");
    printf("[6] Queue Manager\n");
    printf("-----------------------\n");
    printf("[7] Sort library by title\n");
    printf("[8] Sort library by artist\n");
    printf("[9] Sort library by year\n");
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
    case 5:
        PlayQueue();
        MusicLibraryWindow(0);
        break;
    case 6:
        QueueManager();
        MusicLibraryWindow(0);
        break;
    case 7:
        TitleSort();
        MusicLibraryWindow(0); // Next page
        break;
    case 8:
        ArtistSort();
        MusicLibraryWindow(0); // Next page
        break;
    case 9:
        YearSort();
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

void EditMetadata(int id)
{
    int input;
    printf("\n-Metadata Editor-\n\n");
    printf("[1] Edit Title\n");
    printf("[2] Edit Artist\n");
    printf("[3] Edit Album\n");
    printf("[4] Edit Year\n");
    printf("[0] Back to menu\n");
    printf("\nWhat do you want to do?\n");
    scanf("%d", &input);
    char new_input[MAX_STRING_SIZE];
    int new_year;
    switch (input)
    {
    case 1:

        printf("\nInsert new title: ");
        scanf(" %[^\n]s", new_input);
        strcpy(song_pool[id]->title, new_input);
        break;
    case 2:
        printf("\nInsert new artist: ");
        scanf(" %[^\n]s", new_input);
        strcpy(song_pool[id]->artist, new_input);
        break;
    case 3:
        printf("\nInsert new album: ");
        scanf(" %[^\n]s", new_input);
        strcpy(song_pool[id]->album, new_input);
        break;
    case 4:
        printf("\nInsert new year: ");
        scanf(" %d", &new_year);
        song_pool[id]->year = new_year;
        break;
    case 0:
        MusicLibraryWindow(0); // Return to menu and reset the page
        break;
    }
    printf("\nSuccessfully updated metadata!");
    Sleep(1000);
}

void QueueManager()
{
    clearscreen();
    if (displayQueue()) // if there is node in linked list
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

/* apply new id to all songs after linked list changes (from sorting) */
void ResetPoolID()
{
    Song *tmp;
    tmp = head;
    for (int i = 0; i <= MAX_SONG_POOL && tmp != NULL; i++)
    {
        song_pool[i] = tmp;
        tmp = tmp->next;
    }
}

/* initiate music playback based on list of songs in queue (mock only) */
int PlayQueue()
{
    clearscreen();
    node_ptr current, next;
    if (front == NULL)
    {
        printf("\n\nThere is no song in queue. Please add first. Returning back to menu");
        Sleep(2000);
        return 0;
    }
    printf("*Mock Music Playback*\n\n");
    current = front;
    while (current != NULL) // iterate every nodes in linked list
    {
        next = current->next_song;
        if (next == NULL)   // only one element exists
        {
            printf("Playing : %s bt %s\n", current->song->title, current->song->artist);
            printf("Up Next: None\n\n");    
        }
        else
        {
            printf("Playing : %s by %s\n", current->song->title, current->song->artist);
            printf("Up Next: %s by %s\n\n", next->song->title, next->song->artist);
        }
        current = next;
        Dequeue();  // remove the song after playback
        Sleep(2000);
    }
    printf("Played all songs in queue. Returning back to menu");
    Sleep(1000);
}


/* sort title (ascending)*/
void TitleSort()
{
    MergeSort(&head, 1);    // Title = 1, Pass linked list to Merge Sort 
    ResetPoolID();  // Reset all song ids to apply the changes
}

/* sort artist (ascending)*/
void ArtistSort()
{
    MergeSort(&head, 2); // Artist = 2
    ResetPoolID(); 
}

/* sort by year (ascending)*/
void YearSort()
{
    MergeSort(&head, 3); // Year = 3
    ResetPoolID();
}

/* sorts the linked list by changing next pointers (not data) */
void MergeSort(Song **headRef, int type)
{
    Song *hd = *headRef;
    Song *a;
    Song *b;

    /* Base case -- length 0 or 1 */
    if ((hd == NULL) || (hd->next == NULL))
    {
        return;
    }

    /* Split head into 'a' and 'b' sublists */
    FrontBackSplit(hd, &a, &b);

    /* Recursively sort the sublists */
    MergeSort(&a, type);
    MergeSort(&b, type);

    /* answer = merge the two sorted lists together */
    *headRef = SortedMerge(a, b, type);
}

/* split linked list into two seperate halves (Merge Sort) */
void FrontBackSplit(Song *source, Song **frontRef, Song **backRef)
{
    Song *fast;
    Song *slow;
    slow = source;
    fast = source->next;

    /* Advance 'fast' two nodes, and advance 'slow' one node */
    while (fast != NULL)
    {
        fast = fast->next;
        if (fast != NULL)
        {
            slow = slow->next;
            fast = fast->next;
        }
    }

    /* 'slow' is before the midpoint in the list, so split it in two
    at that point. */
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}

/* find song by title */
void FindSongByTitle()
{
    char input[MAX_STRING_SIZE];
    printf("\nInsert title name : ");
    scanf(" %[^\n]s", input);     // accept space
    LinearSearchString(input, 1); // 1 - title, 2 - album, 3 - year
    clearscreen();
    if (displayMatchedSongs())  // if there is song that matches
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

/* find song by artist */
void FindSongByArtist()
{
    char input[MAX_STRING_SIZE];
    printf("\nInsert artist name : ");
    scanf(" %[^\n]s", input); // accept space
    LinearSearchString(input, 2);
    clearscreen();
    if (displayMatchedSongs()) // if there is song that matches
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

/* find song by album */
void FindSongByAlbum()
{
    char input[MAX_STRING_SIZE];
    printf("\nInsert album name : ");
    scanf(" %[^\n]s", input); // accept space
    LinearSearchString(input, 3);
    clearscreen();
    if (displayMatchedSongs()) // if there is song that matches
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

/* print matched song if found then return 1, else return 0 */
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
    destroyList(); // we empty the linked list after display
    return 1;
}

/* empty the list */
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

/* applying linear sequential search but based on string with linked list implementation */
void LinearSearchString(char *target, int attribute)
{
    int ret;
    for (int i = 0; i < MAX_SONG_POOL && song_pool[i] != NULL; i++)
    {
        switch (attribute)  // check the attribute first
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
            if (q_head == NULL) // initiate linked list if empty
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

/* handle selecting song logics */
void selectSong()
{
    int id, input;
    printf("\nInsert song number(#): ");
    scanf(" %d", &id);
    id = id - 1;     // Change to actual index number
    if (id < 0 || id > MAX_SONG_POOL - 1) // if song chosen is not in the song pool
    {
        printf("\nInvalid number! Returning back to menu");
        Sleep(2000);
        MusicLibraryWindow(0);
    }
    printf("\nSelected Track: %s by %s\n\n", song_pool[id]->title, song_pool[id]->artist);
    printf("[1] Add to queue\n");
    printf("[2] Edit Metadata\n");
    printf("[0] Back to menu\n");
    printf("\nWhat do you want to do?\n");
    scanf("%d", &input);
    switch (input)
    {
    case 1:
        Enqueue(id);           // Queue implementation
        MusicLibraryWindow(0); // Return to menu and reset the page
        break;
    case 2:
        EditMetadata(id);
        MusicLibraryWindow(0); // Return to menu and reset the page
        break;
    case 0:
        MusicLibraryWindow(0); // Return to menu and reset the page
        break;
    }
}

/* Add new song (by id) to queue */
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
    printf("\nSuccessfully added to queue!");
    Sleep(1000);
}

/* dequeue nodes (implemented in Play Song in Queue) */
void Dequeue()
{
    node_ptr current, next;
    current = front;
    if (current != NULL)
    {
        front = current->next_song;
        free(current);
    }
}

/* Delete entire song inside queue*/
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

/* display songs inside queue */
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

/* display songs inside queue */
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
        if (paging_count > MAX_SONG_POOL)
        {
            paging_count = MAX_SONG_POOL;
        }
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

/* display songs based step from paging manager*/
void pagingDisplay(int step)
{
    printf("-------------------------------------------------------------------------------------------------------------------\n");
    printf("|                                                 # Music Library #                                               |\n");
    printf("-------------------------------------------------------------------------------------------------------------------\n");
    printf("|  #   |  Title                          |  Artist          |  Album                          |  Year |  Duration |\n");
    printf("-------------------------------------------------------------------------------------------------------------------\n");
    for (int i = step - 10; i < step && song_pool[i] != NULL; i++)
    {
        printf("| %2d   |  %-30.30s |  %-15.15s |  %-30.30s |  %2.d |  %2.2lf min |\n", (i + 1), song_pool[i]->title, song_pool[i]->artist, song_pool[i]->album, song_pool[i]->year, song_pool[i]->duration);
    }
    printf("-------------------------------------------------------------------------------------------------------------------\n");
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
        while (fgets(line, sizeof(line), file)) // Iterate each line
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
            duration = durationms / 60000.0; // Convert miliseconds to minutes
            token = strtok(NULL, ",");       // get the next token
            song_pool[pool_insert_index] = createSong(songtitle, artist, album, year, duration);
            // printf("%s %s %s %d %d\n",songtitle, artist, album, year, durationms);
        }
    }
    fclose(file);
}

/* universal function for asking selection, returning user input*/
int getUserInput()
{
    int input;
    printf("Choose your selection: ");
    scanf(" %d", &input);
    return input;
}

/* clearscreen based on Windows.h */
int clearscreen()
{
    return system("cls");
}

/* converting char to number (part of merge sort implementation)*/
int alphabet_to_num(char c)
{
    int n = -1;
    static const char *const alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char *p = strchr(alphabet, toupper((unsigned char)c));

    if (p)
    {
        n = p - alphabet;
    }
    return n;
}