#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Batsman {
    char name[100];
    int runs;
    int noballsFaced;
    struct Batsman *next;
    struct Batsman *prev;
};

struct Bowler {
    char name[100];
    int noOfWickets;
    int runsConceded;
    int oversBowled;
    struct Bowler *next;
    struct Bowler *prev;
};

int findTotalRuns(struct Batsman *head);
struct Batsman *scanPlayerData(int count);
struct Bowler *scanBowlerData(int count);
void printPlayerData(struct Batsman *head, const char *playerType);
void printBowlerData(struct Bowler *head);
void freePlayerList(struct Batsman *head);
void freeBowlerList(struct Bowler *head);
void compareRuns(struct Batsman *head, const char *teamName);
void compareWickets(struct Bowler *head, const char *teamName);
void sortBowlerList(struct Bowler **head, int choice);
void sortBatsmanList(struct Batsman **head, int choice);
float calculateStrikeRate(struct Batsman *head);
float calculateEconomyRate(struct Bowler *head);
void menu();
void inputData(int choice, struct Batsman **batsman, struct Bowler **bowler);
void merge(struct Bowler **head, int choice);
void mergeSort(struct Bowler **head, int choice);
void mergeBatsman(struct Batsman **head, int choice);
void mergeSortBatsman(struct Batsman **head, int choice);
void split(struct Bowler *head, struct Bowler **front, struct Bowler **back);
void splitBatsman(struct Batsman *head, struct Batsman **front, struct Batsman **back);
struct Bowler *mergeLists(struct Bowler *front, struct Bowler *back, int choice);


void merge(struct Bowler **head, int choice) {
    struct Bowler *front = *head;
    struct Bowler *back = NULL;
    
    // Split the list into two halves
    split(front, &front, &back);
    
    // Recursively sort the two halves
    mergeSort(&front, choice);
    mergeSort(&back, choice);
    
    // Merge the sorted halves
    *head = mergeLists(front, back, choice);
}

struct Bowler *mergeLists(struct Bowler *front, struct Bowler *back, int choice) {
    struct Bowler *result = NULL;
    
    // Base cases
    if (front == NULL)
        return back;
    else if (back == NULL)
        return front;
    
    // Merge recursively based on the sorting criteria
    if (choice == 1) {
        if (front->noOfWickets >= back->noOfWickets) {
            result = front;
            result->next = mergeLists(front->next, back, choice);
        } else {
            result = back;
            result->next = mergeLists(front, back->next, choice);
        }
    } else if (choice == 2) {
        if (front->runsConceded >= back->runsConceded) {
            result = front;
            result->next = mergeLists(front->next, back, choice);
        } else {
            result = back;
            result->next = mergeLists(front, back->next, choice);
        }
    } else if (choice == 3) {
        if ((float)front->runsConceded / front->oversBowled >= (float)back->runsConceded / back->oversBowled) {
            result = front;
            result->next = mergeLists(front->next, back, choice);
        } else {
            result = back;
            result->next = mergeLists(front, back->next, choice);
        }
    }
    
    // Update the previous pointers
    if (result->next != NULL)
        result->next->prev = result;
    
    return result;
}

void mergeSort(struct Bowler **head, int choice) {
    if (*head == NULL || (*head)->next == NULL)
        return;
    struct Bowler *front = *head;
    struct Bowler *back = NULL;
    split(*head, &front, &back);
    mergeSort(&front, choice);
    mergeSort(&back, choice);
    *head = mergeLists(front, back, choice);
}

void split(struct Bowler *head, struct Bowler **front, struct Bowler **back) {
    struct Bowler *slow = head;
    struct Bowler *fast = head->next;
    
    // Move fast pointer two steps and slow pointer one step at a time
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    
    // At this point, slow is at the middle of the list
    *front = head;
    *back = slow->next;
    
    // Break the list into two parts
    slow->next = NULL;
    
    // Update the previous pointer of the back sublist
    if (*back != NULL) {
        (*back)->prev = NULL;
    }
}

// Function to merge two sorted lists of batters
struct Batsman *mergeListsBatsman(struct Batsman *front, struct Batsman *back, int choice) {
    struct Batsman *result = NULL;
    
    // Base cases
    if (front == NULL)
        return back;
    else if (back == NULL)
        return front;
    
    // Merge recursively based on the sorting criteria
    if (choice == 1) {
        if (front->runs >= back->runs) {
            result = front;
            result->next = mergeListsBatsman(front->next, back, choice);
        } else {
            result = back;
            result->next = mergeListsBatsman(front, back->next, choice);
        }
    } else if (choice == 2) {
        if (front->noballsFaced >= back->noballsFaced) {
            result = front;
            result->next = mergeListsBatsman(front->next, back, choice);
        } else {
            result = back;
            result->next = mergeListsBatsman(front, back->next, choice);
        }
    } else if (choice == 3) {
        if ((float)front->runs / front->noballsFaced >= (float)back->runs / back->noballsFaced) {
            result = front;
            result->next = mergeListsBatsman(front->next, back, choice);
        } else {
            result = back;
            result->next = mergeListsBatsman(front, back->next, choice);
        }
    }
    
    // Update the previous pointers
    if (result->next != NULL)
        result->next->prev = result;
    
    return result;
}

// Function to perform merge sort on the list of batters
void mergeSortBatsman(struct Batsman **head, int choice) {
    if (*head == NULL || (*head)->next == NULL)
        return;
    struct Batsman *front = *head;
    struct Batsman *back = NULL;
    splitBatsman(*head, &front, &back);
    mergeSortBatsman(&front, choice);
    mergeSortBatsman(&back, choice);
    *head = mergeListsBatsman(front, back, choice);
}

// Function to split the list of batters into two halves
void splitBatsman(struct Batsman *head, struct Batsman **front, struct Batsman **back) {
    struct Batsman *slow = head;
    struct Batsman *fast = head->next;
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    *front = head;
    *back = slow->next;
    if (slow != NULL) {
        slow->next = NULL;
    }
    if (*back != NULL) {
        (*back)->prev = NULL;
    }
}

// Function to sort the list of batters
void sortBatsmanList(struct Batsman **head, int choice) {
    mergeSortBatsman(head, choice);
}


void sortBowlerList(struct Bowler **head, int choice) {
    mergeSort(head, choice);
}



float calculateStrikeRate(struct Batsman *head) {
    int totalBalls = 0, totalRuns = 0;
    struct Batsman *temp = head;

    while (temp != NULL) {
        totalBalls += temp->noballsFaced;
        totalRuns += temp->runs;
        temp = temp->next;
    }

    return (totalRuns * 100.0 / totalBalls);
}

float calculateEconomyRate(struct Bowler *head) {
    float totalOvers = 0, totalRunsConceded = 0;
    struct Bowler *temp = head;

    while (temp != NULL) {
        totalOvers += temp->oversBowled;
        totalRunsConceded += temp->runsConceded;
        temp = temp->next;
    }

    return (totalRunsConceded / totalOvers);
}

struct Batsman *scanPlayerData(int count) {
    struct Batsman *head = NULL;
    struct Batsman *temp = NULL;

    for (int i = 0; i < count; i++) {
        printf("        \033[1;31mFollow the Input Validations Strictly else program will be terminated\033[1;0m\n          ");
        printf("Enter the details of Batsman %d\n",i+1);
        struct Batsman *newPlayer = (struct Batsman *)malloc(sizeof(struct Batsman));
        if (newPlayer == NULL) {
            printf("Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        printf("  \033[1;31mFOLLOW THIS ORDER \n");
        printf("\033[1;36mNAME : (sting of length less than 99)\n");
        printf("Runs Scored: (in int)\n");
        printf("Number of Balls faced :(in int\033[1;0m)\n");
        if (scanf("%99s %d %d", newPlayer->name, &(newPlayer->runs), &(newPlayer->noballsFaced)) != 3) {
            printf("Invalid input. Please enter valid data.\n");
            exit(EXIT_FAILURE);
        }

        newPlayer->next = NULL;
        newPlayer->prev = temp;

        if (head == NULL) {
            head = newPlayer;
        } else {
            temp->next = newPlayer;
        }
        temp = newPlayer;
    }

    return head;
}

struct Bowler *scanBowlerData(int count) {
    struct Bowler *head = NULL;
    struct Bowler *temp = NULL;

    for (int i = 0; i < count; i++) {
        printf("        \033[1;31mFollow the Input Validations Strictly else program will be terminated\033[1;0m\n          ");

        printf("Enter the details of Bowler %d\n",i+1);
        printf("  \033[1;31mFOLLOW THIS ORDER \n");
        printf("\033[1;36mName: (in string max leng 99)\n");
        printf("Number Of Wickets taken : (in int)\n");
        printf("Runs Conceded : (in int)\n");
        printf("NO. of overs Bowled\033[1;0m\n");

        struct Bowler *newBowler = (struct Bowler *)malloc(sizeof(struct Bowler));
        if (newBowler == NULL) {
                    printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    if (scanf("%99s %d %d %d", newBowler->name, &(newBowler->noOfWickets), &(newBowler->runsConceded), &(newBowler->oversBowled)) != 4) {
        printf("Invalid input. Please enter valid data.\n");
        exit(EXIT_FAILURE);
    }

    newBowler->next = NULL;
    newBowler->prev = temp;

    if (head == NULL) {
        head = newBowler;
    } else {
        temp->next = newBowler;
    }
    temp = newBowler;
}

return head;
}

void printPlayerData(struct Batsman *head, const char *playerType) {
    printf("\033[1;33m-----------------%s Data------------------\n\n\033[1;0m", playerType);
    while (head != NULL) {
        printf("Name: %s\t Runs: %d\t Balls Faced: %d\t Strike Rate: %.2f\n", 
               head->name, head->runs, head->noballsFaced, 
               (float)head->runs / head->noballsFaced * 100.0);
        head = head->next;
    }
    printf("\n\n");
}

void printBowlerData(struct Bowler *head) {
    printf("\033[1;33m----------Bowler Data:----------\033[1;0m\n\n");
    while (head != NULL) {
        printf("Name: %s\t Wickets: %d\t Runs Conceded: %d\t Overs Bowled: %d\t Economy Rate: %.2f\n", 
               head->name, head->noOfWickets, head->runsConceded, head->oversBowled,
               (float)head->runsConceded / head->oversBowled);
        head = head->next;
    }
    printf("\n\n");
}

void freePlayerList(struct Batsman *head) {
    struct Batsman *temp;

    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void freeBowlerList(struct Bowler *head) {
    struct Bowler *temp;

    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void compareRuns(struct Batsman *head, const char *teamName) {
    struct Batsman *temp = head;
    struct Batsman *bestBatsman = temp;

    while (temp != NULL) {
        if (temp->runs > bestBatsman->runs) {
            bestBatsman = temp;
        }
        temp = temp->next;
    }

    printf("Best Batsman for %s: %s\n", teamName, bestBatsman->name);
}

void compareWickets(struct Bowler *head, const char *teamName) {
    struct Bowler *temp = head;
    struct Bowler *bestBowler = temp;

    while (temp != NULL) {
        if (temp->noOfWickets > bestBowler->noOfWickets) {
            bestBowler = temp;
        }
        temp = temp->next;
    }

    printf("Best Bowler for %s: %s\n", teamName, bestBowler->name);
}

int findTotalRuns(struct Batsman *head) {
    int totalRuns = 0;
    struct Batsman *temp = head;

    while (temp != NULL) {
        totalRuns += temp->runs;
        temp = temp->next;
    }

    return totalRuns;
}

void menu() {
    printf("\n\033[1;35mMenu:\n");
    printf("1. Enter data for Batsmen\n");
    printf("2. Enter data for Bowlers\n");
    printf("3. Print match summary\n");
    printf("4. Sort Batsmen\n");
    printf("5. Sort Bowlers\n");
    printf("6. Exit\n\033[1;0m");
    printf("Enter your choice: ");
}

void inputData(int choice, struct Batsman **batsman, struct Bowler **bowler) {
    int count;
    printf("Enter the number of players: ");
    scanf("%d", &count);

    switch (choice) {
        case 1:
            printf("Enter the details of %d Batsmen:\n", count);
            *batsman = scanPlayerData(count);
            break;
        case 2:
            printf("Enter the details of %d Bowlers:\n", count);
            *bowler = scanBowlerData(count);
            break;
        default:
            printf("Invalid choice!\n");
    }
}

int main() {
    char team1[100], team2[100];
    int playersCount, extras1, extras2;
    printf("Enter Team Names\n");
    printf("  \033[1;31mFOLLOW THIS ORDER \n");
    printf("\033[1;36mTeam name 1 : (in string)\n");
    printf("Team name 2 : (in string)\n\033[1;0m");
    scanf("%99[^\n]%*c", team1);
    scanf("%99[^\n]%*c", team2);
   
    printf("Enter Extras conceded by %s\n",team1);
    scanf("%d",&extras1);
    printf("Enter Extras conceded by %s\n",team2);
    scanf("%d",&extras2);

    int choice;
    struct Batsman *team1Players = NULL, *team2Players = NULL;
    struct Bowler *team1Bowlers = NULL, *team2Bowlers = NULL;

    do {
        menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
            case 2:
                inputData(choice, &team1Players, &team1Bowlers);
                inputData(choice, &team2Players, &team2Bowlers);
                break;
            case 3:
                printf("\n\033[1;35m------------Match Summary:-------------\033[1;0m\n");
                printf("Team Name: %s\n", team1);
                printPlayerData(team1Players, "Batsmen");
                printBowlerData(team1Bowlers);
                printf("Total runs scored: %d\n", findTotalRuns(team1Players) + extras1);
                printf("Strike Rate: %.2f\n", calculateStrikeRate(team1Players));

                printf("\nTeam Name: %s\n", team2);
                printPlayerData(team2Players, "Batsmen");
                printBowlerData(team2Bowlers);
                printf("Total runs scored: %d\n", findTotalRuns(team2Players) + extras2);
                printf("Strike Rate: %.2f\n", calculateStrikeRate(team2Players));

                compareRuns(team1Players, team1);
                compareRuns(team2Players, team2);
                compareWickets(team1Bowlers, team1);
                compareWickets(team2Bowlers, team2);
                break;
            case 4:
                printf("\nSorting Options for Batsmen:\n");
                printf("\033[1;35m1. Sort by Runs Scored\n");
                printf("2. Sort by Balls Faced\n");
                printf("3. Sort by Strike Rate\033[1;0m\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);
                sortBatsmanList(&team1Players, choice);
                sortBatsmanList(&team2Players, choice);
                break;
            case 5:
                printf("\n\033[1;35mSorting Options for Bowlers:\n");
                printf("1. Sort by Wickets Taken\n");
                printf("2. Sort by Runs Conceded\n");
                printf("3. Sort by Economy Rate\033[1;0m\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);
                sortBowlerList(&team1Bowlers, choice);
                sortBowlerList(&team2Bowlers, choice);
                break;
            case 6:
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 6);

    freePlayerList(team1Players);
    freeBowlerList(team1Bowlers);
    freePlayerList(team2Players);
    freeBowlerList(team2Bowlers);

    printf("----------\033[1;36mThank you for using the cricket match summary program\033[1;0m----------");
    return 0;
}