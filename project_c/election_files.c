#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Macro definitions for maximum length constraints
#define Max_name_len 50
#define Max_id_len 15
#define Max_password_len 15

// Structure Definitions

// Structure for a voter, which includes ID, password, voted status, and a next pointer
typedef struct Voter
{
    char id[Max_id_len];
    char password[Max_password_len];
    int Voted; // 0->no, 1->yes
    struct Voter *next;
} Voter;

// Structure for a candidate, which includes ID, password, name, achievements, votes, and a next pointer
typedef struct Candidate
{
    char id[Max_id_len];
    char password[Max_password_len];
    char name[Max_name_len];
    char achievements[255];
    int Votes;
    struct Candidate *next;
} Candidate;

// Structure for the election system, which holds pointers to linked lists of candidates and voters
typedef struct
{
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
void saveCandidateToFile(Candidate *candidate);
void updateVotesInFile(Election_system *system);

// Function Definitions

// Initializes the election system with empty candidate and voter lists
void initializeElectionSystem(Election_system *system)
{
    system->Candidates = NULL;
    system->voters = NULL;
}

// Creates a candidate with the given details and returns a pointer to the new candidate
Candidate *createCandidate(char *name, char *achievements, char *id, char *password)
{
    Candidate *newCandidate = (Candidate *)malloc(sizeof(Candidate));
    strcpy(newCandidate->name, name);
    strcpy(newCandidate->achievements, achievements);
    strcpy(newCandidate->id, id);
    strcpy(newCandidate->password, password);
    newCandidate->Votes = 0;
    newCandidate->next = NULL;
    return newCandidate;
}

// Creates a voter with the given ID and password, initializes Voted to 0, and returns a pointer to the new voter
Voter *createVoter(char *id, char *password)
{
    Voter *newVoter = (Voter *)malloc(sizeof(Voter));
    strcpy(newVoter->id, id);
    strcpy(newVoter->password, password);
    newVoter->Voted = 0;
    newVoter->next = NULL;
    return newVoter;
}

// Function to save a candidate's information to a file
void saveCandidateToFile(Candidate *candidate)
{
    FILE *file = fopen("candidates.txt", "a"); // Append mode to add new candidates
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }
    fprintf(file, "ID: %s, Name: %s, Achievements: %s, Votes: %d\n",
            candidate->id, candidate->name, candidate->achievements, candidate->Votes);
    fclose(file);
}

// Function to update the vote count for all candidates in the file
void updateVotesInFile(Election_system *system)
{
    FILE *file = fopen("candidates.txt", "w"); // Write mode to overwrite with updated data
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }
    Candidate *temp = system->Candidates;
    while (temp != NULL)
    {
        fprintf(file, "ID: %s, Name: %s, Achievements: %s, Votes: %d\n",
                temp->id, temp->name, temp->achievements, temp->Votes);
        temp = temp->next;
    }
    fclose(file);
}

// Registers a new candidate, taking their name and achievements, and generates a unique ID and password
void registerCandidate(Election_system *system)
{
    char name[Max_name_len], achievements[255], id[Max_id_len], password[Max_password_len];
    printf("Enter the Candidate Name: ");
    scanf(" %[^\n]", name);
    printf("Enter the Achievements of the Candidate: ");
    scanf(" %[^\n]", achievements);

    // Generate a unique ID and password for the candidate
    snprintf(id, Max_id_len, "CANDIDATE2024%02d", rand() % 100);
    snprintf(password, Max_password_len, "Password%02d", rand() % 100);

    // Create the new candidate and add to the system's linked list
    Candidate *newCandidate = createCandidate(name, achievements, id, password);
    newCandidate->next = system->Candidates;
    system->Candidates = newCandidate;

    // Save the candidate to the file
    saveCandidateToFile(newCandidate);

    // Display confirmation with generated ID and password
    printf("Candidate registered with ID: %s and Password: %s\n", id, password);
}

// Displays all registered candidates with their details
void displayCandidates(Election_system *system)
{
    printf("\nThe List of Candidates:\n");
    Candidate *temp = system->Candidates;
    while (temp != NULL)
    {
        printf("ID: %s, Name: %s, Achievements: %s, Votes: %d\n",
               temp->id, temp->name, temp->achievements, temp->Votes);
        temp = temp->next;
    }
}

// Registers a new voter with a specified ID and password
void registerVoter(Election_system *system)
{
    char id[Max_id_len], password[Max_password_len];
    printf("Enter the Voter ID: ");
    scanf("%s", id);
    printf("Enter Voter's password: ");
    scanf("%s", password);

    // Create the new voter and add to the system's linked list
    Voter *newVoter = createVoter(id, password);
    newVoter->next = system->voters;
    system->voters = newVoter;

    // Display confirmation of registration
    printf("Voter registered with ID: %s\n", id);
}

// Authenticates a voter by checking their ID and password, and ensures they haven't voted yet
Voter *authenticateVoter(Election_system *system, char *id, char *password)
{
    Voter *temp = system->voters;
    while (temp != NULL)
    {
        if (strcmp(temp->id, id) == 0 && strcmp(temp->password, password) == 0)
        {
            if (temp->Voted == 1)
            {
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

// Allows a voter to vote for a candidate by ID, ensuring valid voter authentication
void vote(Election_system *system)
{
    char voterID[Max_id_len], voterPassword[Max_password_len], candidateID[Max_id_len];
    printf("Enter the Voter ID: ");
    scanf("%s", voterID);
    printf("Enter your Voter password: ");
    scanf("%s", voterPassword);

    // Authenticate voter
    Voter *voter = authenticateVoter(system, voterID, voterPassword);
    if (voter != NULL)
    {
        printf("Enter the Candidate ID to vote: ");
        scanf("%s", candidateID);

        // Find the candidate by ID and increment their vote count
        Candidate *temp = system->Candidates;
        while (temp != NULL)
        {
            if (strcmp(temp->id, candidateID) == 0)
            {
                temp->Votes++;
                voter->Voted = 1; // Mark voter as having voted
                printf("You have successfully voted for %s\n", temp->name);

                // Update the votes in the file
                updateVotesInFile(system);
                return;
            }
            temp = temp->next;
        }
        printf("Candidate ID not found\n");
    }
}

// Frees memory allocated to the linked list of candidates
void freeCandidates(Candidate *candidate)
{
    while (candidate != NULL)
    {
        Candidate *temp = candidate;
        candidate = candidate->next;
        free(temp);
    }
}

// Frees memory allocated to the linked list of voters
void freeVoters(Voter *voter)
{
    while (voter != NULL)
    {
        Voter *temp = voter;
        voter = voter->next;
        free(temp);
    }
}

// Frees memory allocated to the entire election system
void freeElectionSystem(Election_system *system)
{
    freeCandidates(system->Candidates);
    freeVoters(system->voters);
}

// Main function to run the election system menu
int main()
{
    Election_system system;
    initializeElectionSystem(&system);
    int choice;

    while (1)
    {
        printf("\n---- Election System ----\n");
        printf("1. Register Candidate\n");
        printf("2. Display Candidates\n");
        printf("3. Register Voter\n");
        printf("4. Vote\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
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
            printf("Exiting the system.\n");
            return 0;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
}




/*echo "# Election_voting_system" >> README.md
git init
git add README.md
git commit -m "first commit"
git branch -M main
git remote add origin https://github.com/MANI12321/Election_voting_system.git
git push -u origin main*/