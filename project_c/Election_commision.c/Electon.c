#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Macro definitions
#define Max_name_len 50
#define Max_id_len 10
#define Max_password_len 10

// Structure Definitions
typedef struct Voter {
    char id[Max_id_len];
    char password[Max_password_len];
    int Voted; // 0->no, 1->yes
    struct Voter *next;
} Voter;

typedef struct Candidate {
    char id[Max_id_len];
    char password[Max_password_len];
    char name[Max_name_len];
    char achivements[255];
    int Votes;
    struct Candidate *next;
} Candidate;

typedef struct {
    Candidate *Candidates;
    Voter *voters;
} Election_system;

// Function Declarations
void initializeElectionSystem(Election_system *system);
Candidate *createCandidate(char *name, char *achievements, char *id, char *password);
Voter *createVoter(char *id, char *password);
void registerCandidate(Election_system *system);
void displayCandidates(Election_system *system);
void registerVoter(Election_system *system);
Voter *authenticateVoter(Election_system *system, char *id, char *password);
void vote(Election_system *system);
void freeCandidates(Candidate *candidate);
void freeVoters(Voter *voter);
void freeElectionSystem(Election_system *system);

// Function Definitions
void initializeElectionSystem(Election_system *system) {
    system->Candidates = NULL;
    system->voters = NULL;
}

Candidate *createCandidate(char *name, char *achievements, char *id, char *password) {
    Candidate *newCandidate = (Candidate *)malloc(sizeof(Candidate));
    strcpy(newCandidate->name, name);
    strcpy(newCandidate->achivements, achievements);
    strcpy(newCandidate->id, id);
    strcpy(newCandidate->password, password);
    newCandidate->Votes = 0;
    newCandidate->next = NULL;
    return newCandidate;
}

Voter *createVoter(char *id, char *password) {
    Voter *newVoter = (Voter *)malloc(sizeof(Voter));
    strcpy(newVoter->id, id);
    strcpy(newVoter->password, password);
    newVoter->Voted = 0;
    newVoter->next = NULL;
    return newVoter;
}

void registerCandidate(Election_system *system) {
    char name[Max_name_len], achivements[255], id[Max_id_len], password[Max_password_len];
    printf("Enter the Candidate Name: ");
    scanf(" %[^\n]", name);
    printf("Enter the Achievements of the Candidate: ");
    scanf(" %[^\n]", achivements);

    snprintf(id, Max_id_len, "CANDIDATE2024%02d", rand() % 100);
    snprintf(password, Max_password_len, "Password%02d", rand() % 100);

    Candidate *newCandidate = createCandidate(name, achivements, id, password);
    newCandidate->next = system->Candidates;
    system->Candidates = newCandidate;
    printf("Candidate registered with ID: %s and Password: %s\n", id, password);
}

void displayCandidates(Election_system *system) {
    printf("\nThe List of Candidates:\n");
    Candidate *temp = system->Candidates;
    while (temp != NULL) {
        printf("ID: %s, Name: %s, Achievements: %s, Votes: %d\n",
               temp->id, temp->name, temp->achivements, temp->Votes);
        temp = temp->next;
    }
}

void registerVoter(Election_system *system) {
    char id[Max_id_len], password[Max_password_len];
    printf("Enter the Voter ID: ");
    scanf("%s", id);
    printf("Enter Voter's password: ");
    scanf("%s", password);

    Voter *newVoter = createVoter(id, password);
    newVoter->next = system->voters;
    system->voters = newVoter;
    printf("Voter registered with ID: %s\n", id);
}

Voter *authenticateVoter(Election_system *system, char *id, char *password) {
    Voter *temp = system->voters;
    while (temp != NULL) {
        if (strcmp(temp->id, id) == 0 && strcmp(temp->password, password) == 0) {
            if (temp->Voted == 1) {
                printf("Voter has already voted\n");
                return NULL;
            }
            return temp;
        }
        temp = temp->next;
    }
    printf("Voter is not registered\n");
    return NULL;
}

void vote(Election_system *system) {
    char voterID[Max_id_len], voterPassword[Max_password_len], candidateID[Max_id_len];
    printf("Enter the Voter ID: ");
    scanf("%s", voterID);
    printf("Enter your Voter password: ");
    scanf("%s", voterPassword);

    Voter *voter = authenticateVoter(system, voterID, voterPassword);
    if (voter != NULL) {
        printf("Enter the Candidate ID to vote: ");
        scanf("%s", candidateID);

        Candidate *temp = system->Candidates;
        while (temp != NULL) {
            if (strcmp(temp->id, candidateID) == 0) {
                temp->Votes++;
                voter->Voted = 1;
                printf("You have successfully voted for %s\n", temp->name);
                return;
            }
            temp = temp->next;
        }
        printf("Candidate ID not found\n");
    }
}

void freeCandidates(Candidate *candidate) {
    while (candidate != NULL) {
        Candidate *temp = candidate;
        candidate = candidate->next;
        free(temp);
    }
}

void freeVoters(Voter *voter) {
    while (voter != NULL) {
        Voter *temp = voter;
        voter = voter->next;
        free(temp);
    }
}

void freeElectionSystem(Election_system *system) {
    freeCandidates(system->Candidates);
    freeVoters(system->voters);
}

int main() {
    Election_system system;
    initializeElectionSystem(&system);
    int choice;

    while (1) {
        printf("\n---- Election System ----\n");
        printf("1. Register Candidate\n");
        printf("2. Display Candidates\n");
        printf("3. Register Voter\n");
        printf("4. Vote\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerCandidate(&system);
                break;
            case 2:
                displayCandidates(&system);
                break;
            case 3:
                registerVoter(&system);
                break;
            case 4:
                vote(&system);
                break;
            case 5:
                freeElectionSystem(&system);
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}
