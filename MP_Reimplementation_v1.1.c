#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SUBJECT 101
#define MAX_CONNECTIONS 10
#define MAX_CHAR_USER 21
#define MAX_CHAR_PASS 21
#define MAX_CHAR_MESSAGE 1000 // Maximum no. of Characters for each message / announcement
#define MAX_MESSAGES 1000 //Maximum no. of Messages that can exist in the program
#define MAX_USERS 30 //Maximum no. of Users that can be registered
#define MAX_CHAR_PASS 21 // Maximum no. of characters for password
#define MAX_CHAR_SEC 101 //Maximum number of Characters for Security Questions & Answers
#define MAX_LINES 1000
#define MAX_SENDER 21
#define MAX_RECEIVER_NAME 21
#define MAX_RECEIVERS 10
#define MAX_SUBJECT 101

#define USERSFILE "UsersFile.txt"
#define ADMINPASSFILE "AdminPassFile.txt"
#define MESSAGESFILE "MessagesFile.txt"

typedef char String[101];

typedef struct {
    int isAccountLocked; // 0 for unlocked, 1 for locked
    int numConnections;
    char name[MAX_CHAR_USER];
    char username[MAX_CHAR_USER];
    char password[MAX_CHAR_PASS];
    char securityQuestion[MAX_CHAR_SEC];
    char securityAnswer[MAX_CHAR_SEC];
    char description[101]; 
    char connections[MAX_CONNECTIONS][MAX_CHAR_USER];
} UserInfo;

typedef struct { 
	int numLines;
    int numReceivers;
    char strSender[MAX_SENDER];
    char strReceivers[MAX_RECEIVERS][MAX_RECEIVER_NAME];
    char strSubject[MAX_SUBJECT];
    char **strMessageEntry;
} messageTag;

/***********Utility Functions****************/
void clean(String strTemp);
int getValidChoice(int lower, int upper);

/****************************USER CREATION AND LOG IN FUNCTIONS*****************************/
void forgotPassword(UserInfo newUser[MAX_USERS], int numUsers, String resetRequests[MAX_USERS], int *numResetRequests);
void saveToUsersFile(UserInfo newUser[MAX_USERS], int numUsers);
void loadFromUsersFile(UserInfo newUser[MAX_USERS], int *numUsers);
int isUserValid(String username, String password, UserInfo newUser[MAX_USERS], int numUsers);
int getUserIndex(String username, String password, UserInfo newUser[MAX_USERS], int numUsers);
void Login(UserInfo newUser[MAX_USERS], int *numUsers);
void LoginPage(char adminPass[MAX_CHAR_PASS], UserInfo newUser[MAX_USERS], int *numUsers, String resetRequests[MAX_USERS], int *numResetRequests);
int isUsernameTaken(UserInfo newUser[MAX_USERS], int numUsers, char *username);
void createNewAccount(UserInfo newUser[MAX_USERS], int *numUsers);
/********************************************************************************************/

/************************************ADMIN FUNCTIONS*****************************************/
void saveToAdminPassFile(char adminPass[MAX_CHAR_PASS]);
void loadFromAdminPassFile(char adminPass[MAX_CHAR_PASS]);
void createNewAdminPass(char adminPass[MAX_CHAR_PASS]);
void changeAdminPass(char adminPass[MAX_CHAR_PASS]);
void AdminModuleLogin(char adminPass[MAX_CHAR_PASS], UserInfo newUser[MAX_USERS], int numUsers, String resetRequests[MAX_USERS], int *numResetRequests);
void AdminModulePage(char adminPass[MAX_CHAR_PASS], UserInfo newUser[MAX_USERS], int numUsers, String resetRequests[MAX_USERS], int *numResetRequests);
void forgotPassword(UserInfo newUser[MAX_USERS], int numUsers, String resetRequests[MAX_USERS], int *numResetRequests);
void refreshUserAccountPasswordPage(UserInfo newUser[MAX_USERS], int numUsers, String resetRequests[MAX_USERS], int *numResetRequests);
/********************************************************************************************/

/***********************************USER MODULE FUNCTIONS************************************/
/*-----------------------------------Messaging Functions------------------------------------*/
void splitReceivers(char *receiverBuffer, char receiversArray[MAX_RECEIVERS][MAX_RECEIVER_NAME], int *numReceivers);
void freeMessage(messageTag *msg);
void freeAllMessages(messageTag msgs[MAX_MESSAGES], int count);
int isValidReceiver(char receiver[], UserInfo LoadedUsers[MAX_USERS], int numUsers);
void loadAllMessagesFiles(messageTag Messages[MAX_MESSAGES], int *numMessages);
void printMessageDetails(messageTag message, int index);
void viewSentMessages(char user[], messageTag SavedMessages[MAX_MESSAGES], int totalMessages, messageTag SentMessages[MAX_MESSAGES], int *sentCount); // 3
void viewAnnouncements(messageTag SavedMessages[MAX_MESSAGES], int totalMessages, messageTag Announcements[MAX_MESSAGES], int *AnnounceCount); // 4
void viewReceivedMessages(char user[], messageTag SavedMessages[MAX_MESSAGES], int totalMessages, messageTag Received[MAX_MESSAGES], int *receiveCount, UserInfo LoadedUsers[MAX_USERS], int numUsers); // 2
void saveMessagesToFile(messageTag SavedMessages[MAX_MESSAGES], int savedCount);
void composeMessage(char user[], messageTag SavedMessages[MAX_MESSAGES], int *savedCount, UserInfo LoadedUsers[MAX_USERS], int numUsers); // 1
/*------------------------------------------------------------------------------------------*/
void addUserConnection(UserInfo newUser[MAX_USERS], int numUsers, int userIndex, int targetIndex); // 8.4
void viewUserPage(UserInfo newUser[MAX_USERS], int numUsers, int targetIndex, int userIndex, char user[], messageTag SavedMessages[MAX_MESSAGES], int *savedCount) ; // 8.3
void filterUsersByName(UserInfo newUser[MAX_USERS], int numUsers, int userIndex, char *username, messageTag SavedMessages[MAX_MESSAGES], int *savedCount); // 8.2
void viewAllUsers(UserInfo newUser[MAX_USERS], int numUsers, int userIndex, char *username, messageTag SavedMessages[MAX_MESSAGES], int *savedCount); // 8.1
void browseUsers(UserInfo newUser[MAX_USERS], int numUsers, int userIndex, char *username, messageTag SavedMessages[MAX_MESSAGES], int *savedCount); // 8
void viewUserConnections(UserInfo newUser[MAX_USERS], int numUsers, int userIndex); // 7.4
void removePersonalConnection(UserInfo newUser[MAX_USERS], int numUsers, int userIndex); // 7.3
void viewPersonalConnections(UserInfo newUser[MAX_USERS], int userIndex); // 7.2
void addPersonalConnection(UserInfo newUser[MAX_USERS], int numUsers, int userIndex); // 7.1
void modifyPersonalConnectionsPage(UserInfo newUser[MAX_USERS], int numUsers, char *username, int userIndex, messageTag SavedMessages[MAX_MESSAGES], int *savedCount) ; //  7
void modifyAccountSecurity(UserInfo newUser[MAX_USERS], int numUsers, char *username, int userIndex); // 6
void modifyPersonalContents(UserInfo newUser[MAX_USERS], int numUsers, char *username, int userIndex); //5
void userModulePage(UserInfo newUser[MAX_USERS], int numUsers, char *username, int userIndex);
/********************************************************************************************/

/*****************
UTILITY FUNCTIONS
******************/
void clean(String strTemp) {
	int len = strlen(strTemp);
	int lastIndex = len-1;
	
	if (lastIndex >= 0 && strTemp[lastIndex] == '\n')
		strTemp[lastIndex] = '\0';
}

int getValidChoice(int lower, int upper) {
    int choice;
    int validInput;

    do {
        printf("Enter choice: ");
        validInput = scanf("%d", &choice);
        fflush(stdin);

        if (validInput != 1 || choice < lower || choice > upper) {
            printf("Invalid input, please choose one of the choices (%d-%d).\n", lower, upper);
        }
    } while (validInput != 1 || choice < lower || choice > upper);

    return choice;
}

/*********************
USER MODULE FUNCTIONS
*********************/
/*--------------------------------------------------------------------------------------------------------------------------------*/
// Messaging Functions

void splitReceivers(char *receiverBuffer, char receiversArray[MAX_RECEIVERS][MAX_RECEIVER_NAME], int *numReceivers) {
    *numReceivers = 0;
    char *token = strtok(receiverBuffer, ",");

    while (token != NULL && *numReceivers < MAX_RECEIVERS) {
        clean(token); // Clean each receiver to trim spaces
        strncpy(receiversArray[*numReceivers], token, MAX_RECEIVER_NAME - 1);
        receiversArray[*numReceivers][MAX_RECEIVER_NAME - 1] = '\0';
        (*numReceivers)++;
        token = strtok(NULL, ",");
    }
}

void freeMessage(messageTag *msg) {
    int i = 0;
    while (i < msg->numLines) {
        free(msg->strMessageEntry[i]);
        i++;
    }
    free(msg->strMessageEntry);
}

void freeAllMessages(messageTag msgs[MAX_MESSAGES], int count) {
    int i = 0;
    while (i < count) {
        freeMessage(&msgs[i]);
        i++;
    }
}

int isValidReceiver(char receiver[], UserInfo LoadedUsers[MAX_USERS], int numUsers) {
    int i;

    // Check if receiver is "everyone" for announcements
    clean(receiver);
    if (strcasecmp(receiver, "everyone") == 0) {
        return 1;  // Valid for announcements
    }

    // Check if the receiver is a valid user
    for (i = 0; i < numUsers; i++) {
        clean(LoadedUsers[i].username);
        if (strcmp(LoadedUsers[i].username, receiver) == 0) {
            return 1;  // Receiver found
        }
    }
    
    return 0;  // Receiver not found
}

void loadAllMessagesFiles(messageTag Messages[MAX_MESSAGES], int *numMessages) {
    FILE *pFile = fopen(MESSAGESFILE, "rt");
    String strTemp;
    int i = 0, j;

    *numMessages = 0;

    if (pFile != NULL) {
        fgets(strTemp, sizeof(strTemp), pFile);
        *numMessages = atoi(strTemp);

        while (i < *numMessages) {
            fgets(strTemp, sizeof(strTemp), pFile);
            clean(strTemp);

            int numLines;
            char receiverBuffer[101];
            sscanf(strTemp, "%20[^|]|%100[^|]|%100[^|]|%d",
                   Messages[i].strSender,
                   receiverBuffer,
                   Messages[i].strSubject,
                   &numLines);

            splitReceivers(receiverBuffer, Messages[i].strReceivers, &Messages[i].numReceivers);
            Messages[i].numLines = numLines;

            Messages[i].strMessageEntry = malloc(sizeof(char *) * numLines);
            j = 0;
            while (j < numLines) {
                fgets(strTemp, sizeof(strTemp), pFile);
                clean(strTemp);
                Messages[i].strMessageEntry[j] = malloc(strlen(strTemp) + 1);
                strcpy(Messages[i].strMessageEntry[j], strTemp);
                j++;
            }
            i++;
        }
        fclose(pFile);
    }
}

void printMessageDetails(messageTag message, int index) {
    int j;
	
	system("cls");
    printf("Message #%d\n", index + 1);
    printf("From    : %s\n", message.strSender);

    printf("To      : ");
    j = 0;
    while (j < message.numReceivers) {
        printf("%s", message.strReceivers[j]);
        if (j < message.numReceivers - 1) {
            printf(", ");
        }
        j++;
    }
    printf("\n");

    printf("Subject : %s\n", message.strSubject);
    printf("Message :\n");
    j = 0;
    while (j < message.numLines) {
        printf("  %s\n", message.strMessageEntry[j]);
        j++;
    }
    printf("--------------------\n");
    system("pause");
}

void viewSentMessages(char user[], messageTag SavedMessages[MAX_MESSAGES], int totalMessages, messageTag SentMessages[MAX_MESSAGES], int *sentCount) {
    int i = 0, hasMessages = 0;
	
	system("cls");
    *sentCount = 0;
    if (totalMessages > 0) {
        while (i < totalMessages) {
            if (strcmp(SavedMessages[i].strSender, user) == 0) {
                SentMessages[*sentCount] = SavedMessages[i];
                (*sentCount)++;
                hasMessages = 1;
            }
            i++;
        }
    }

    if (hasMessages == 0) {
        printf("No Mail Found.\n");
    } else {
        i = 0;
        while (i < *sentCount) {
            printMessageDetails(SentMessages[i], i);
            i++;
        }
    }
    system("pause");
}

void viewAnnouncements(messageTag SavedMessages[MAX_MESSAGES], int totalMessages, messageTag Announcements[MAX_MESSAGES], int *AnnounceCount) {
    int i = 0, j, hasAnnouncements = 0;
	
	system("cls");
    *AnnounceCount = 0;
    if (totalMessages > 0) {
        while (i < totalMessages) {
            int isAnnouncement = 0;
            j = 0;
            while (j < SavedMessages[i].numReceivers) {
                if (strcmp(SavedMessages[i].strReceivers[j], "everyone") == 0) {
                    isAnnouncement = 1;
                }
                j++;
            }
            if (isAnnouncement == 1) {
                Announcements[*AnnounceCount] = SavedMessages[i];
                (*AnnounceCount)++;
                hasAnnouncements = 1;
            }
            i++;
        }
    }

    if (hasAnnouncements == 0) {
        printf("No Announcements Found.\n");
    } else {
        i = 0;
        while (i < *AnnounceCount) {
            printMessageDetails(Announcements[i], i);
            i++;
        }
    }
    system("pause");
}

void viewReceivedMessages(char user[], messageTag SavedMessages[MAX_MESSAGES], int totalMessages, messageTag Received[MAX_MESSAGES], int *receiveCount, UserInfo LoadedUsers[MAX_USERS], int numUsers) {
    int i, j, userFound = 0, hasMessages = 0;

    system("cls");
    *receiveCount = 0;

    // Check if the user exists in LoadedUsers
    for (i = 0; i < numUsers; i++) {
        clean(LoadedUsers[i].username);
        clean(user);

        if (strcmp(LoadedUsers[i].username, user) == 0) {
            userFound = 1;
        }
    }

    // Proceed only if the user is valid
    if (userFound == 1) {
        if (totalMessages > 0) {
            for (i = 0; i < totalMessages; i++) {
                int isSentToUser = 0;

                // Check if message has receivers
                if (SavedMessages[i].numReceivers != 0) {
                    // Check each receiver in the message
                    for (j = 0; j < SavedMessages[i].numReceivers; j++) {
                        clean(SavedMessages[i].strReceivers[j]);

                        // Check if the message is sent to "everyone"
                        if (strcasecmp(SavedMessages[i].strReceivers[j], "everyone") == 0) {
                            isSentToUser = 1;  // Sent to all users
                        }

                        // Compare receiver with the logged-in user
                        if (strcasecmp(SavedMessages[i].strReceivers[j], user) == 0) {
                            isSentToUser = 1;  // Sent to the specific user
                        }
                    }

                    // Save the message if sent to the user or everyone
                    if (isSentToUser == 1) {
                        Received[*receiveCount] = SavedMessages[i];
                        (*receiveCount)++;
                        hasMessages = 1;
                    }
                }
            }

            // Display messages if any
            if (hasMessages == 0) {
                printf("No Mail Found.\n");
            } else {
                for (i = 0; i < *receiveCount; i++) {
                    printMessageDetails(Received[i], i);
                }
            }
        } else {
            printf("No messages available.\n");
        }
    } else {
        printf("Error: User '%s' does not exist. Cannot view messages.\n", user);
    }

    system("pause");
}

void saveMessagesToFile(messageTag SavedMessages[MAX_MESSAGES], int savedCount) {
    FILE *pFile = fopen(MESSAGESFILE, "wt");
    int i = 0, j;

    if (pFile != NULL) {
        fprintf(pFile, "%d\n", savedCount);
        while (i < savedCount) {
            fprintf(pFile, "%s|", SavedMessages[i].strSender);
            j = 0;
            while (j < SavedMessages[i].numReceivers) {
                fprintf(pFile, "%s", SavedMessages[i].strReceivers[j]);
                if (j < SavedMessages[i].numReceivers - 1) {
                    fprintf(pFile, ",");
                }
                j++;
            }
            fprintf(pFile, "|%s|%d\n", SavedMessages[i].strSubject, SavedMessages[i].numLines);

            j = 0;
            while (j < SavedMessages[i].numLines) {
                fprintf(pFile, "%s\n", SavedMessages[i].strMessageEntry[j]);
                j++;
            }
            i++;
        }
        fclose(pFile);
    } else {
        printf("Error opening %s for writing.\n", MESSAGESFILE);
    }
}

void composeMessage(char user[], messageTag SavedMessages[MAX_MESSAGES], int *savedCount, UserInfo LoadedUsers[MAX_USERS], int numUsers) {
    if (*savedCount >= MAX_MESSAGES) {
        printf("Message limit reached. Cannot compose more messages.\n");
    } else {
        messageTag newMessage;
        String strTemp;
        int numLines = 0;
        int done = 0;
        int i, validReceivers = 1;

        system("cls");
        printf("Creating a Message!\n\n");
        printf("Add a Subject: ");
        fgets(newMessage.strSubject, sizeof(newMessage.strSubject), stdin);
        clean(newMessage.strSubject);

        strcpy(newMessage.strSender, user);
        printf("Enter the Recepient. Separate with commas if multiple users or enter \"everyone\" to make an announcement\n\n");
        printf("To: ");
        fgets(strTemp, sizeof(strTemp), stdin);
        clean(strTemp);
        splitReceivers(strTemp, newMessage.strReceivers, &newMessage.numReceivers);
		
        // Check if all recipients are valid using isValidReceiver()
        i = 0;
        while (i < newMessage.numReceivers && validReceivers == 1) {
            if (isValidReceiver(newMessage.strReceivers[i], LoadedUsers, numUsers) == 0) {
                printf("\nError: User '%s' does not exist. Message not sent.\n", newMessage.strReceivers[i]);
                validReceivers = 0; // Mark as invalid and prevent message sending
            }
            i++;
        }

        // Proceed only if all recipients are valid
        if (validReceivers == 1) {
            newMessage.strMessageEntry = malloc(sizeof(char *) * MAX_LINES);

            printf("Write your message. Type 'END' on a new line to finish:\n");
            while (!done && numLines < MAX_LINES) {
                fgets(strTemp, sizeof(strTemp), stdin);
                clean(strTemp);
                if (strcmp(strTemp, "END") == 0) {
                    done = 1;
                } else {
                    newMessage.strMessageEntry[numLines] = malloc(strlen(strTemp) + 1);
                    strcpy(newMessage.strMessageEntry[numLines], strTemp);
                    numLines++;
                }
            }
            newMessage.numLines = numLines;

            // Save the message
            SavedMessages[*savedCount] = newMessage;
            (*savedCount)++;

            saveMessagesToFile(SavedMessages, *savedCount);
            printf("Message composed successfully!\n");
        }
        system("pause");
    }
}

/*---------------------------------------------------------------------------------------------------------------------------------*/

void addUserConnection(UserInfo newUser[MAX_USERS], int numUsers, int userIndex, int targetIndex) { // 8.4
    // Check if the user is trying to add themselves
    if (userIndex == targetIndex) {
        printf("You cannot add yourself as a personal connection.\n");
    } 
    else {
        // Call the existing function to add the connection
        addPersonalConnection(newUser, numUsers, userIndex);
    }

    system("pause");
}

void viewUserPage(UserInfo newUser[MAX_USERS], int numUsers, int targetIndex, int userIndex, char *username, messageTag SavedMessages[MAX_MESSAGES], int *savedCount) { // 8.3
	int bGoBack = 0;
	do {
		system("cls");
	    printf("Viewing %s's Profile:\n\n", newUser[targetIndex].username);
	    printf("Name: %s\n", newUser[targetIndex].name);
	
	    if (strlen(newUser[targetIndex].description) == 0) {
	        printf("Description: DEFAULT USER\n\n");
	    } 
		else {
	        printf("Description: %s\n\n", newUser[targetIndex].description);
	    }
	
    // Display options for interaction
	    printf("[1] - Add as Personal Connection\n");
	    printf("[2] - Send Private Message\n");
	    printf("[3] - Go back\n\n");
	
	    int choice = getValidChoice(1, 3);
	
	    if (choice == 1) {
	        addUserConnection(newUser, numUsers, userIndex, targetIndex);
	    } 
	    else if (choice == 2) {
	        composeMessage(username, SavedMessages, savedCount, newUser, numUsers);
	    }
		else if (choice ==3)
			bGoBack = 1;
	} while (!bGoBack);
}

void filterUsersByName(UserInfo newUser[MAX_USERS], int numUsers, int userIndex, char *username, messageTag SavedMessages[MAX_MESSAGES], int *savedCount) { // 8.2
    char searchName[51];
    int i, found = 0;
	int nChoice, targetIndex;
    system("cls");
    printf("Enter the name to search: ");
    scanf(" %[^\n]", searchName);

    printf("\nFiltered Results:\n\n");

    for (i = 0; i < numUsers; i++) {
        if (strstr(newUser[i].name, searchName) != NULL) {
            printf("[%d] Username: %s\n", i + 1, newUser[i].username);
            printf("    Name: %s\n", newUser[i].name);

            if (strlen(newUser[i].description) == 0) {
                printf("    Description: DEFAULT USER\n\n");
            } else {
                printf("    Description: %s\n\n", newUser[i].description);
            }
            
            found = 1;
        }
    }

    if (!found) {
        printf("No users found matching '%s'.\n\n", searchName);
    }

    printf("[0] - Go back\n\n");
    nChoice = getValidChoice(0, numUsers);
    targetIndex = nChoice - 1;
    if (nChoice != 0) {
        viewUserPage(newUser, numUsers, targetIndex, userIndex, username, SavedMessages, savedCount);  
    }
}

void viewAllUsers(UserInfo newUser[MAX_USERS], int numUsers, int userIndex, char *username, messageTag SavedMessages[MAX_MESSAGES], int *savedCount) { // 8.1
    int i, j, nChoice;
    int targetIndex;
    system("cls");
    printf("Users Registered in The Application:\n\n");

    for (i = 0; i < numUsers; i++) {
        printf("[%d] Username: %s\n", i + 1, newUser[i].username);
        printf("    Name: %s\n", newUser[i].name);

        // Show description
        if (strlen(newUser[i].description) == 0 || strcmp(newUser[i].description, "DEFAULT USER") == 0) {
            printf("    Description: DEFAULT USER\n");
        } 
        else {
            printf("    Description: %s\n", newUser[i].description);
        }

        // Show connections
        if (newUser[i].numConnections > 0) {
            printf("    Connections: ");
            for (j = 0; j < newUser[i].numConnections; j++) {
                printf("%s", newUser[i].connections[j]);
                if (j < newUser[i].numConnections - 1) {
                    printf(", ");
                }
            }
            printf("\n\n");
        } 
        else {
            printf("    Connections: NONE\n\n");
        }
    }

    printf("[0] - Go back\n\n");
    nChoice = getValidChoice(0, numUsers);
    targetIndex = nChoice - 1;
    
    if (nChoice != 0) {
        viewUserPage(newUser, numUsers, targetIndex, userIndex, username, SavedMessages, savedCount);
    }
}

void browseUsers(UserInfo newUser[MAX_USERS], int numUsers, int userIndex, char *username, messageTag SavedMessages[MAX_MESSAGES], int *savedCount) { // 8
    int nChoice;
    
    do {
        system("cls");
        printf("Browse Users:\n");
        printf("[1] - View All Users\n");
        printf("[2] - Search by Name\n");
        printf("[3] - Go back\n\n");
        
        nChoice = getValidChoice(1, 3);

        if (nChoice == 1) {
            viewAllUsers(newUser, numUsers, userIndex, username, SavedMessages, savedCount);
        } 
        else if (nChoice == 2) {
            filterUsersByName(newUser, numUsers, userIndex, username, SavedMessages, savedCount);
        }
    } while (nChoice != 3);
}

void viewUserConnections(UserInfo newUser[MAX_USERS], int numUsers, int userIndex) { // 7.4
    int i, j, found = 0;

    system("cls");
    printf("Users Who Have You in Their Connections:\n\n");

    for (i = 0; i < numUsers; i++) {
        if (i != userIndex) {
            for (j = 0; j < newUser[i].numConnections; j++) {
                clean(newUser[i].connections[j]);
                clean(newUser[userIndex].username);

                // Check if current user is in another user's connection list
                if (strcmp(newUser[i].connections[j], newUser[userIndex].username) == 0) {
                    printf("Username: %s\n", newUser[i].username);
                    printf("Name: %s\n", newUser[i].name);
                    printf("Description: %s\n\n", strlen(newUser[i].description) == 0 ? "DEFAULT USER" : newUser[i].description);
                    found = 1;
                }
            }
        }
    }

    // If no matches found
    if (!found) {
        printf("No users have added you to their connections.\n");
    }

    system("pause");
}

void removePersonalConnection(UserInfo newUser[MAX_USERS], int numUsers, int userIndex) { // 7.3
    char connectionName[MAX_CHAR_USER];
    int i, j, connectionIndex = -1, found = 0;

    system("cls");
    printf("Remove Personal Connection\n\n");

    // Check if the user has any connections
    if (newUser[userIndex].numConnections > 0) {
        printf("Your Current Connections:\n");
        for (i = 0; i < newUser[userIndex].numConnections; i++) {
            printf("%d. %s\n", i + 1, newUser[userIndex].connections[i]);
        }

        // Get the connection to remove
        printf("\nEnter the username to remove: ");
        fgets(connectionName, sizeof(connectionName), stdin);
        clean(connectionName);

        // Search for the connection
        for (i = 0; i < newUser[userIndex].numConnections; i++) {
            if (strcmp(newUser[userIndex].connections[i], connectionName) == 0) {
                connectionIndex = i;
                found = 1;
                break;
            }
        }

        // If connection is found, remove it
        if (found) {
            for (j = connectionIndex; j < newUser[userIndex].numConnections - 1; j++) {
                strcpy(newUser[userIndex].connections[j], newUser[userIndex].connections[j + 1]);
            }

            // Clear last connection slot and reduce connection count
            strcpy(newUser[userIndex].connections[newUser[userIndex].numConnections - 1], "NONE");
            newUser[userIndex].numConnections--;

            // Clean up remaining slots to prevent NONE spam
            for (i = newUser[userIndex].numConnections; i < MAX_CONNECTIONS; i++) {
                strcpy(newUser[userIndex].connections[i], "NONE");
            }

            printf("Connection removed successfully!\n");
        } else {
            printf("User not found in your personal connections.\n");
        }
    } else {
        printf("You have no personal connections to remove.\n");
    }

    // Save changes to file after modification
    saveToUsersFile(newUser, numUsers);
    system("pause");
}

void viewPersonalConnections(UserInfo newUser[MAX_USERS], int userIndex) { // 7.2
    int i, hasConnections = 0;

    system("cls");
    printf("Your Personal Connections:\n\n");

    // Check and display connections
    if (newUser[userIndex].numConnections > 0) {
        hasConnections = 1;
        for (i = 0; i < newUser[userIndex].numConnections; i++) {
            printf("%d. %s\n", i + 1, newUser[userIndex].connections[i]);
        }
    }

    // No connections found
    if (!hasConnections) {
        printf("You have no personal connections.\n");
    }

    system("pause");
}

void addPersonalConnection(UserInfo newUser[MAX_USERS], int numUsers, int userIndex) { // 7.1
    char connectionName[MAX_CHAR_USER];
    int connectionIndex = -1, alreadyConnected = 0, validConnection = 0;
    int i, j;
    int selfAddAttempt = 0;

    printf("\nEnter the username to add: ");
    fgets(connectionName, sizeof(connectionName), stdin);
    clean(connectionName);

    // Check if the user is trying to add themselves
    selfAddAttempt = strcmp(connectionName, newUser[userIndex].username) == 0;

    if (!selfAddAttempt) {
        // Check if the username exists in the list of users
        for (i = 0; i < numUsers; i++) {
            if (strcmp(newUser[i].username, connectionName) == 0) {
                connectionIndex = i;
                validConnection = 1;
            }
        }
    }

    // Proceed only if connection is valid and not adding themselves
    if (validConnection && !selfAddAttempt) {
        // Check if the user is already connected
        for (j = 0; j < newUser[userIndex].numConnections; j++) {
            if (strcmp(newUser[userIndex].connections[j], connectionName) == 0) {
                alreadyConnected = 1;
            }
        }

        // If not already connected, add the connection
        if (!alreadyConnected) {
            if (newUser[userIndex].numConnections < MAX_CONNECTIONS) {
                // Add connection to the list
                strcpy(newUser[userIndex].connections[newUser[userIndex].numConnections], connectionName);
                newUser[userIndex].numConnections++;

                // Save changes to file after adding connection
                saveToUsersFile(newUser, numUsers);

                printf("Connection added successfully!\n");
            } else {
                printf("You have reached the maximum number of personal connections.\n");
            }
        } else {
            printf("User is already in your personal connections.\n");
        }
    } 
    else if (selfAddAttempt) {
        printf("You cannot add yourself as a personal connection.\n");
    } 
    else {
        printf("Invalid username or user not found.\n");
    }

    system("pause");
}

void modifyPersonalConnectionsPage(UserInfo newUser[MAX_USERS], int numUsers, char *username, int userIndex, messageTag SavedMessages[MAX_MESSAGES], int *savedCount) { // 7
    int done = 0, nChoice;

    // Check if the user was found
    if (userIndex != -1) {
		while (!done) {
	        system("cls");
	        printf("Modify Personal Connections for %s\n\n", newUser[userIndex].name);
	        printf("[1] Add a Personal Connection\n");
	        printf("[2] View Personal Connections\n");
	        printf("[3] Remove a Personal Connection\n");
	        printf("[4] View Users Who Have You in Their Connections\n");
	        printf("[5] Back\n\n");
			
			nChoice = getValidChoice(1,5);
	
	        switch (nChoice) {
	            case 1:
	                addPersonalConnection(newUser, numUsers, userIndex);
	                break;
	            case 2:
	                viewPersonalConnections(newUser, userIndex);
	                break;
	            case 3:
	                removePersonalConnection(newUser, numUsers, userIndex);
	                break;
	            case 4:
	                viewUserConnections(newUser, numUsers, userIndex);
	                break;
	            case 5:
	                done = 1;  // Exit and return to the main menu
	                break;
	                
		    }	
	    }
	}
    else {
		printf("User not found.\n");
	    system("pause");
	}
}

void modifyAccountSecurity(UserInfo newUser[MAX_USERS], int numUsers, char *username, int userIndex) { // 6
    UserInfo userTemp;
    char currentPassword[MAX_CHAR_PASS];
    int i, verified = 0, done = 0;

    // Check if the user was found
    if (userIndex != -1) {
        // Copy current user details to userTemp
        userTemp = newUser[userIndex];

        while (!done) {
            system("cls");
            printf("Modify Account Security (Enter 'back' to cancel)\n\n");

            // Verify current password before allowing changes
            printf("Enter your current password: ");
            fgets(currentPassword, sizeof(currentPassword), stdin);
            clean(currentPassword);

            if (strcmp(currentPassword, "back") == 0) {
                done = 1;
            } else if (strcmp(currentPassword, userTemp.password) == 0) {
                verified = 1;
            } else {
                printf("Incorrect password. Please try again.\n");
                system("pause");
            }

            if (verified) {
                system("cls");
                printf("Modify Account Security (Enter 'back' to cancel)\n\n");

                // Input new password
                printf("Enter New Password: ");
                fgets(userTemp.password, sizeof(userTemp.password), stdin);
                clean(userTemp.password);

                if (strcmp(userTemp.password, "back") == 0) {
                    done = 1;
                } else {
                    char confirmPassword[MAX_CHAR_PASS];
                    printf("Re-enter New Password: ");
                    fgets(confirmPassword, sizeof(confirmPassword), stdin);
                    clean(confirmPassword);

                    if (strcmp(confirmPassword, "back") == 0) {
                        done = 1;
                    } else if (strcmp(userTemp.password, confirmPassword) != 0) {
                        printf("Passwords do not match. Please try again.\n");
                        system("pause");
                        continue;
                    }

                    // Input new security question and answer
                    printf("\nEnter New Security Question: ");
                    fgets(userTemp.securityQuestion, sizeof(userTemp.securityQuestion), stdin);
                    clean(userTemp.securityQuestion);

                    if (strcmp(userTemp.securityQuestion, "back") == 0) {
                        done = 1;
                    } else {
                        printf("Enter New Security Answer: ");
                        fgets(userTemp.securityAnswer, sizeof(userTemp.securityAnswer), stdin);
                        clean(userTemp.securityAnswer);

                        if (strcmp(userTemp.securityAnswer, "back") == 0) {
                            done = 1;
                        } else {
                            // Apply changes to the original user
                            newUser[userIndex] = userTemp;

                            // Save changes to file
                            saveToUsersFile(newUser, numUsers);
                            printf("Security settings updated successfully!\n");
                            done = 1;
                            system("pause");
                        }
                    }
                }
            }
        }
    } else {
        printf("User not found.\n");
        system("pause");
    }
}

void modifyPersonalContents(UserInfo newUser[MAX_USERS], int numUsers, char *username, int userIndex) { // 5
    UserInfo userTemp;
    char tempName[21];
    char passwordCheck[MAX_CHAR_PASS];
    int i, verified = 0, done = 0, inputValid = 0;
    clean(username);

    // Check if the user was found
    if (userIndex != -1) {
        // Copy current user details to userTemp
        userTemp = newUser[userIndex];

        while (!done) {
            system("cls");
            printf("Modify Personal Contents (Enter 'back' to cancel)\n\n");

            // Show current details
            printf("Current Name: %s\n", userTemp.name);

            // Show current description
            if (strlen(userTemp.description) == 0 || strcmp(userTemp.description, "DEFAULT USER") == 0) {
                printf("Current Description: DEFAULT USER\n");
            } 
            else {
                printf("Current Description: %s\n", userTemp.description);
            }

            // Show current connections with | separator
            if (userTemp.numConnections > 0) {
                printf("Current Connections: ");
                for (i = 0; i < userTemp.numConnections; i++) {
                    printf("%s", userTemp.connections[i]);
                    if (i < userTemp.numConnections - 1) {
                        printf(" | ");
                    }
                }
                printf("\n");
            } 
            else {
                printf("Current Connections: NONE\n");
            }

            // Input new name
            printf("\nEnter New Name: ");
            fgets(tempName, sizeof(tempName), stdin);
            clean(tempName);

            // Check if user wants to go back
            if (strcmp(tempName, "back") == 0) {
                done = 1;
            } 
            else {
                // Input new description
                printf("Enter New Description (or leave empty to skip): ");
                fgets(userTemp.description, sizeof(userTemp.description), stdin);
                clean(userTemp.description);

                // Input new connection to modify (optional)
                printf("Enter New Connection (or leave empty to skip): ");
                fgets(userTemp.connections[userTemp.numConnections], sizeof(userTemp.connections[userTemp.numConnections]), stdin);
                clean(userTemp.connections[userTemp.numConnections]);

                inputValid = 1;  // Inputs are valid if we get here
            }

            // Only proceed if inputs are valid
            if (inputValid && !done) {
                // Verify password to confirm changes
                printf("\nEnter your password to confirm changes: ");
                fgets(passwordCheck, sizeof(passwordCheck), stdin);
                clean(passwordCheck);

                // Validate password
                if (strcmp(passwordCheck, userTemp.password) == 0) {
                    verified = 1;
                } 
                else {
                    printf("Incorrect password. Please try again.\n");
                    system("pause");
                }

                // Apply changes if verified
                if (verified) {
                    // Update name
                    strcpy(userTemp.name, tempName);

                    // Default description if left empty
                    if (strlen(userTemp.description) == 0) {
                        strcpy(userTemp.description, "DEFAULT USER");
                    }

                    // Add new connection if provided
                    if (strlen(userTemp.connections[userTemp.numConnections]) > 0) {
                        if (userTemp.numConnections < MAX_CONNECTIONS) {
                            userTemp.numConnections++;
                        } 
                        else {
                            printf("Maximum number of connections reached.\n");
                        }
                    }

                    // Apply changes to original user
                    newUser[userIndex] = userTemp;

                    // Save changes and exit
                    saveToUsersFile(newUser, numUsers);
                    printf("Changes saved successfully!\n");
                    done = 1;  // Exit after successful modification
                }
            }
        }
    } 
    else {
        printf("User not found.\n");
        system("pause");
    }
}

void userModulePage(UserInfo newUser[MAX_USERS], int numUsers, char *username, int userIndex) {
    int nChoice, bQuit = 0;
    int sentCount = 0;
    int announcementCount = 0;
    int receiveCount = 0;
    int numMessages = 0;
    char sender[MAX_CHAR_USER];
	messageTag messages[MAX_MESSAGES];
    messageTag sentMessages[MAX_MESSAGES];
    messageTag Announcements[MAX_MESSAGES];
    messageTag Received[MAX_MESSAGES];
    

	strcpy(sender, username); // Copy the username to sender
	loadAllMessagesFiles(messages ,&numMessages);
    do {
        system("cls");
        printf("Welcome to the User Module, %s!\n\n", username);
        printf("[1] - Compose Message\n");
        printf("[2] - Inbox\n");
        printf("[3] - Sent\n");
        printf("[4] - View Announcements\n");
        printf("[5] - Modify Personal Contents\n");
        printf("[6] - Modify Account Security\n");
        printf("[7] - Modify Personal Connections\n");
        printf("[8] - Browse Users\n");
        printf("[9] - Logout\n");
        printf("\n");

        nChoice = getValidChoice(1, 9);

        switch (nChoice) {
            case 1:
                composeMessage(sender, messages, &numMessages, newUser, numUsers);
                break;
            case 2:
                viewReceivedMessages(sender, messages, numMessages, Received, &receiveCount, newUser, numUsers);
                break;
            case 3:
                viewSentMessages(sender, messages, numMessages, sentMessages, &sentCount);
                break;
            case 4:
                viewAnnouncements(messages, numMessages, Announcements, &announcementCount);
                break;
            case 5:
                modifyPersonalContents(newUser, numUsers, username, userIndex);
                break;
            case 6:
                modifyAccountSecurity(newUser, numUsers, username, userIndex);
                break;
            case 7:
                modifyPersonalConnectionsPage(newUser, numUsers, username, userIndex, messages, &numMessages);
                break;
            case 8:
                browseUsers(newUser, numUsers, userIndex, username, messages, &numMessages);
                break;
            case 9:
                printf("\nLogging out...\n");
                bQuit = 1;  // Exit the module when logging out
                system("pause");
                break;
        }
    } while (!bQuit);
}

/********************************
USER CREATION AND LOGIN FUNCTIONS
*********************************/
int getUserIndexNoPassword(String username,UserInfo newUser[MAX_USERS], int numUsers) { //Loops through the UsersFile to determine whether user is valid or not, returns the index of the found username
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(newUser[i].username, username) == 0) {
            return i; //Returns the found user index
        }
    }
    return -1; // User index not found
}
void forgotPassword(UserInfo newUser[MAX_USERS], int numUsers, String resetRequests[MAX_USERS], int *numResetRequests) {
    char nameInput[MAX_CHAR_USER];
    char securityAnswer[MAX_CHAR_SEC];
    int userIndex;
    int bGoBack = 0;

    system("cls");
    printf("Forgot Password (Enter 'back' to return to the previous page)\n\n");

    while (!bGoBack) {
        printf("Enter your username: ");
        fgets(nameInput, sizeof(nameInput), stdin);
        clean(nameInput);
        bGoBack = strcmp(nameInput, "back") == 0;

        if (!bGoBack) {
            userIndex = getUserIndexNoPassword(nameInput, newUser, numUsers);
            if (userIndex != -1) {
                printf("Security Question: %s\n", newUser[userIndex].securityQuestion);
                printf("Enter your answer: ");
                fgets(securityAnswer, sizeof(securityAnswer), stdin);
                clean(securityAnswer);

                if (strcmp(securityAnswer, newUser[userIndex].securityAnswer) == 0) {
                    // Add request to reset password
                    strcpy(resetRequests[*numResetRequests], nameInput);
                    (*numResetRequests)++;
                    printf("Password reset request has been sent to the administrator.\n");
                    bGoBack = 1;
                } else {
                    printf("Incorrect answer. Please try again.\n");
                }
            } else {
                printf("Username not found. Please try again.\n");
            }
        }
    }
    system("pause");
}

int isUsernameTaken(UserInfo newUser[MAX_USERS], int numUsers, char *username) {
    int i;
    for (i = 0; i < numUsers; i++) {
        if (strcmp(newUser[i].username, username) == 0) {
            return 1; // Username already taken
        }
    }
    return 0; // Username is available
}

void createNewAccount(UserInfo newUser[MAX_USERS], int *numUsers) {
    UserInfo userTemp; // Place inputs here for safekeeping
    int bGoBack = 0, numLines;
    int bUsernameTaken;
    String confirmPassword; // Used for re-entering password

    system("cls");
    printf("Creating New Account (Enter 'back' to return to the previous page)\n\n");

    while (!bGoBack) {
        printf("Name: ");
        fgets(userTemp.name, sizeof(userTemp.name), stdin);
        clean(userTemp.name);
        bGoBack = strcmp(userTemp.name, "back") == 0;

        if (!bGoBack) {
            do {
                printf("Username: ");
                fgets(userTemp.username, sizeof(userTemp.username), stdin);
                clean(userTemp.username);
                bGoBack = strcmp(userTemp.username, "back") == 0;

                if (!bGoBack) {
                    bUsernameTaken = isUsernameTaken(newUser, *numUsers, userTemp.username);
                    if (bUsernameTaken) {
                        printf("Username already taken. Try again.\n");
                    }
                }
            } while (!bGoBack && bUsernameTaken);
        }

        if (!bGoBack) {
            do {
                printf("Password: ");
                fgets(userTemp.password, sizeof(userTemp.password), stdin);
                clean(userTemp.password);
                bGoBack = strcmp(userTemp.password, "back") == 0;

                if (!bGoBack) {
                    printf("Re-enter Password: ");
                    fgets(confirmPassword, sizeof(confirmPassword), stdin);
                    clean(confirmPassword);
                    bGoBack = strcmp(confirmPassword, "back") == 0;

                    if (strcmp(userTemp.password, confirmPassword) != 0) {
                        printf("Passwords do not match. Try again.\n");
                    }
                }
            } while (!bGoBack && strcmp(userTemp.password, confirmPassword) != 0);
        }

        if (!bGoBack) {
            printf("Security Question: ");
            fgets(userTemp.securityQuestion, sizeof(userTemp.securityQuestion), stdin);
            clean(userTemp.securityQuestion);
            bGoBack = strcmp(userTemp.securityQuestion, "back") == 0;
        }

        if (!bGoBack) {
            printf("Security Answer: ");
            fgets(userTemp.securityAnswer, sizeof(userTemp.securityAnswer), stdin);
            clean(userTemp.securityAnswer);
            bGoBack = strcmp(userTemp.securityAnswer, "back") == 0;
        }

        if (!bGoBack) {
            // Assigning userTemp to new user structure
            strcpy(newUser[*numUsers].description, "DEFAULT USER"); 
            strcpy(newUser[*numUsers].name, userTemp.name);
            strcpy(newUser[*numUsers].username, userTemp.username);
            strcpy(newUser[*numUsers].password, userTemp.password);
            strcpy(newUser[*numUsers].securityQuestion, userTemp.securityQuestion);
            strcpy(newUser[*numUsers].securityAnswer, userTemp.securityAnswer);
            (*numUsers)++; // Update the user count
            saveToUsersFile(newUser, *numUsers);
            printf("Account created successfully!\n");
            bGoBack = 1;
            system("pause");
        }
    }
}

void saveToUsersFile(UserInfo newUser[MAX_USERS], int numUsers) {
    FILE *pFile = fopen(USERSFILE, "wt"); // Open in write mode
    int i, j;

    if (pFile != NULL) {
        // Save the number of users first
        fprintf(pFile, "%d\n", numUsers);

        // Loop through all users and write their information in ONE line
        for (i = 0; i < numUsers; i++) {
            // Write basic user information
            if (strlen(newUser[i].description) == 0) {
                fprintf(pFile, "%d-%s-%s-%s-%s-%s-DEFAULT USER-", 
                    newUser[i].isAccountLocked,
                    newUser[i].name,
                    newUser[i].username,
                    newUser[i].password,
                    newUser[i].securityQuestion,
                    newUser[i].securityAnswer);
            } 
            else {
                fprintf(pFile, "%d-%s-%s-%s-%s-%s-%s-", 
                    newUser[i].isAccountLocked,
                    newUser[i].name,
                    newUser[i].username,
                    newUser[i].password,
                    newUser[i].securityQuestion,
                    newUser[i].securityAnswer,
                    newUser[i].description);
            }

            // Check if the user has any valid connections
            int hasValidConnections = 0;
            for (j = 0; j < newUser[i].numConnections; j++) {
                if (strcmp(newUser[i].connections[j], "NONE") != 0 && strcmp(newUser[i].connections[j], "") != 0) {
                    if (hasValidConnections) {
                        fprintf(pFile, "|");  // Add | between connections
                    }
                    fprintf(pFile, "%s", newUser[i].connections[j]);
                    hasValidConnections = 1;
                }
            }

            // If no valid connections, write "NONE"
            if (!hasValidConnections) {
                fprintf(pFile, "NONE");
            }

            fprintf(pFile, "\n");  // End of user's data line
        }

        fclose(pFile);
    } 
    else {
        printf("Error generating %s\n", USERSFILE);
    }
}

void parseConnections(char connections[MAX_CONNECTIONS][MAX_CHAR_USER], char tempConnections[], int *numConnections) {
    char *token = strtok(tempConnections, "|");  // Split connections using '|'
    *numConnections = 0;

    while (token != NULL && *numConnections < MAX_CONNECTIONS) {
        if (strcmp(token, "NONE") != 0 && strcmp(token, "") != 0) {
            strcpy(connections[*numConnections], token);
            (*numConnections)++;
        }
        token = strtok(NULL, "|");  // Move to the next connection
    }
}

void loadFromUsersFile(UserInfo newUser[MAX_USERS], int *numUsers) {
    FILE *pFile = fopen(USERSFILE, "rt"); // Open in read mode
    String strTemp;
    int i = 0;
    char tempConnections[10]; // Temp storage for connections

    if (pFile != NULL) {
        // Read the number of users
        fgets(strTemp, sizeof(strTemp), pFile);
        *numUsers = atoi(strTemp);

        if (*numUsers > MAX_USERS) {
            printf("Warning: Too many users in the file. Only the first %d users will be loaded.\n", MAX_USERS);
            *numUsers = MAX_USERS;
        }

        // Loop through all users and parse their information
        while (fgets(strTemp, sizeof(strTemp), pFile) != NULL && i < *numUsers) {
            if (sscanf(strTemp, "%d-%[^-]-%[^-]-%[^-]-%[^-]-%[^-]-%[^-]-%[^\n]",
                       &newUser[i].isAccountLocked,
                       newUser[i].name,
                       newUser[i].username,
                       newUser[i].password,
                       newUser[i].securityQuestion,
                       newUser[i].securityAnswer,
                       newUser[i].description,
                       tempConnections) == 8) {
                
                // Parse connections properly
                parseConnections(newUser[i].connections, tempConnections, &newUser[i].numConnections);
                
                i++;
            } else {
                printf("Error reading user data. Skipping invalid entry.\n");
            }
        }
        fclose(pFile);
    } else {
        printf("Error reading %s. Initializing with 0 users.\n", USERSFILE);
        *numUsers = 0;
    }
}

int isUserValid(String username, String password, UserInfo newUser[MAX_USERS], int numUsers) { //Loops through the UsersFile to determine whether user is valid or not 
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(newUser[i].username, username) == 0 && strcmp(newUser[i].password, password) == 0) {
            return 1; // Found valid user
        }
    }
    return 0; // Invalid user
}

int getUserIndex(String username, String password, UserInfo newUser[MAX_USERS], int numUsers) { //Loops through the UsersFile to determine whether user is valid or not, returns the index of the found username
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(newUser[i].username, username) == 0 && strcmp(newUser[i].password, password) == 0) {
            return i; //Returns the found user index
        }
    }
    return -1; // User index not found
}

void Login(UserInfo newUser[MAX_USERS], int *numUsers) { //TO FIX: LOCKING USER AFTER MANY ATTEMPTS
    String nameInput;
    String passInput;
    int userIndex = -1;  // Initialize to -1 to indicate no valid user yet
    int attempts = 3;
    int bGoBack = 0;

    while (attempts > 0 && !bGoBack) {
        system("cls");
        printf("Enter your username or enter \"back\" to return to the previous page\n\n");
        
        printf("Enter Username: ");
        fgets(nameInput, sizeof(nameInput), stdin);
        clean(nameInput);
        bGoBack = strcmp(nameInput, "back") == 0;
        
        if (!bGoBack) {
            printf("Enter Password: ");
            fgets(passInput, sizeof(passInput), stdin);
            clean(passInput);

            userIndex = getUserIndex(nameInput, passInput, newUser, *numUsers);
            
            // Check if the account is locked
            if (userIndex != -1 && newUser[userIndex].isAccountLocked) {
                printf("Your account is locked. Please contact the administrator to unlock your account.\n");
                system("pause");
                bGoBack = 1;
            } 
            else if (userIndex != -1 && isUserValid(nameInput, passInput, newUser, *numUsers)) {
                printf("Login successful!\n");
                system("pause");
                userModulePage(newUser, *numUsers, nameInput, userIndex);
                return;  // Successful login, exit the function
            } 
            else {
                attempts--;
                printf("Incorrect username or password. Attempts left: %d\n\n", attempts);
                system("pause");
            }
        }
    }

    if (bGoBack) {
        printf("Returning to the previous page...\n");
    } 
    else if (attempts == 0 && userIndex != -1) {
        printf("Too many failed attempts. Your account is now locked.\n");
        
        // Lock the account and save changes
        newUser[userIndex].isAccountLocked = 1;
        saveToUsersFile(newUser, *numUsers);  
    }

    system("pause");
}


void LoginPage(char adminPass[MAX_CHAR_PASS], UserInfo newUser[MAX_USERS], int *numUsers, String resetRequests[MAX_USERS], int *numResetRequests) {
    int nChoice, bQuit = 0;

    do {
        system("cls");
        printf("Please choose an option:\n\n");
        printf("[1] - Login\n");
        printf("[2] - Create a New Account\n");
        printf("[3] - Administrator Module\n");
        printf("[4] - Forgot Password\n");
        printf("[5] - Quit\n\n");

        nChoice = getValidChoice(1, 5);

        switch (nChoice) {
            case 1:
                Login(newUser, numUsers);
                break;
            case 2:
                createNewAccount(newUser, numUsers);
                break;
            case 3:
                AdminModuleLogin(adminPass, newUser, *numUsers, resetRequests, numResetRequests);
                break;
            case 4:
                forgotPassword(newUser, *numUsers, resetRequests, numResetRequests);
                break;
            case 5:
                bQuit = 1;
                break;
        }
    } while (!bQuit);
}

/**************
ADMIN FUNCTIONS
***************/

void saveToAdminPassFile(char adminPass[MAX_CHAR_PASS]) { //Used in changePassword file, saves a new password to AdminPassFIle if called
	FILE *pFile = fopen(ADMINPASSFILE, "wt"); // Open in write mode
    int i;

    if (pFile != NULL) {
        // Save the number of users first
        fprintf(pFile, "%s\n", adminPass);
        fclose(pFile);
    } else {
        printf("Error generating %s\n", ADMINPASSFILE);
    }
}

void createNewAdminPass(char adminPass[MAX_CHAR_PASS]) { //Create new admin password function
    String strInput;
    int bGoBack = 0;
    String confirmPassword;

    system("cls");
    printf("Creating New Admin Password (Enter 'back' to return to the previous page)\n\n");

    while (!bGoBack) {
        do {
            printf("Password: ");
            fgets(strInput, sizeof(strInput), stdin);
            clean(strInput);
            bGoBack = strcmp(strInput, "back") == 0;

            if (!bGoBack) {
                printf("Re-enter Password: ");
                fgets(confirmPassword, sizeof(confirmPassword), stdin);
                clean(confirmPassword);
                bGoBack = strcmp(confirmPassword, "back") == 0;

                if (strcmp(strInput, confirmPassword) != 0) {
                    printf("Passwords do not match. Try again.\n");
                }
            }
        } while (!bGoBack && strcmp(strInput, confirmPassword) != 0);

        if (!bGoBack) {
            // Assign new password to adminPass
            strcpy(adminPass, strInput);
            saveToAdminPassFile(adminPass);
            printf("Admin Password created successfully!\n");
            bGoBack = 1;
            system("pause");
        }
    }
}

void changeAdminPass(char adminPass[MAX_CHAR_PASS]) {
    String currentPass, newPass, confirmPass;
    int bGoBack = 0;

    system("cls");
    printf("Change Admin Password (Enter 'back' to return to the previous page)\n\n");

    while (!bGoBack) {
        // Verify current password
        printf("Enter current password: ");
        fgets(currentPass, sizeof(currentPass), stdin);
        clean(currentPass);

        if (strcmp(currentPass, "back") == 0) {
            bGoBack = 1;
        } else if (strcmp(currentPass, adminPass) == 0) {
            // Get new password
            do {
                printf("Enter new password: ");
                fgets(newPass, sizeof(newPass), stdin);
                clean(newPass);

                printf("Re-enter new password: ");
                fgets(confirmPass, sizeof(confirmPass), stdin);
                clean(confirmPass);

                if (strcmp(newPass, confirmPass) != 0) {
                    printf("Passwords do not match. Try again.\n");
                }
            } while (strcmp(newPass, confirmPass) != 0);

            // Save new password
            strcpy(adminPass, newPass);
            saveToAdminPassFile(adminPass);
            printf("Password changed successfully!\n");
            system("pause");
            bGoBack = 1;
        } else {
            printf("Incorrect current password. Try again.\n");
            system("pause");
        }
    }
}

void loadFromAdminPassFile(char adminPass[MAX_CHAR_PASS]) { //loads information from AdminPassFile (creates new file if not initialized yet)
    FILE *pFile;
    int isEmpty = 1;

    // Open file in read mode
    pFile = fopen(ADMINPASSFILE, "rt");

    if (pFile != NULL) {
    	fgets(adminPass, MAX_CHAR_PASS, pFile);
        clean(adminPass); // Clean newline if any
        fclose(pFile);
    }
    else {
    	createNewAdminPass(adminPass);
	}
}

void AdminModuleLogin(char adminPass[MAX_CHAR_PASS], UserInfo newUser[MAX_USERS], int numUsers, String resetRequests[MAX_USERS], int *numResetRequests) {
    String enteredPass;
    int attempts = 3;
    int bGoBack = 0;
    
	loadFromAdminPassFile(adminPass);

    while (attempts > 0 && !bGoBack) {
        system("cls");
        printf("Enter admin password or type \"back\" to return:\n");
        printf("\nEnter Password: ");
        fgets(enteredPass, sizeof(enteredPass), stdin);
        clean(enteredPass);

        if (strcmp(enteredPass, "back") == 0) {
            bGoBack = 1;
        } else if (strcmp(enteredPass, adminPass) == 0) {
            printf("Admin login successful!\n");
            AdminModulePage(adminPass, newUser, numUsers, resetRequests, numResetRequests);
			system("pause");
        } else {
            attempts--;
            printf("Incorrect password. Attempts left: %d\n", attempts);
            system("pause");
        }
    }
}

void refreshUserAccountPasswordPage(UserInfo newUser[MAX_USERS], int numUsers, String resetRequests[MAX_USERS], int *numResetRequests) {
    int i, userIndex;
    char defaultPassword[MAX_CHAR_PASS] = "default";
    int nChoice, bQuit = 0;

    while (!bQuit) {
        system("cls");
        printf("Refresh User Account Password\n\n");

        if (*numResetRequests == 0) {
            printf("No password reset requests.\n");
            bQuit = 1;
        } else {
            printf("Users who have requested a password reset:\n\n");
            for (i = 0; i < *numResetRequests; i++) {
                userIndex = getUserIndexNoPassword(resetRequests[i], newUser, numUsers);
                if (userIndex != -1) {
                    printf("[%d] Username: %s\n", i + 1, newUser[userIndex].username);
                    printf("    Name: %s\n", newUser[userIndex].name);
                    printf("    Description: %s\n\n", strlen(newUser[userIndex].description) == 0 ? "DEFAULT USER" : newUser[userIndex].description);
                } else {
                    printf("[%d] Username: %s (User not found)\n\n", i + 1, resetRequests[i]);
                }
            }

            printf("[0] - Go back\n\n");
            nChoice = getValidChoice(0, *numResetRequests);

            if (nChoice != 0) {
                userIndex = getUserIndexNoPassword(resetRequests[nChoice - 1], newUser, numUsers);
                if (userIndex != -1) {
                    // Reset the user's password to the default password
                    strcpy(newUser[userIndex].password, defaultPassword);
                    newUser[userIndex].isAccountLocked = 1; // Lock the account until the user changes the password
                    printf("Password for user '%s' has been reset to '%s'.\n", resetRequests[nChoice - 1], defaultPassword);

                    // Remove the request from the list
                    for (i = nChoice - 1; i < *numResetRequests - 1; i++) {
                        strcpy(resetRequests[i], resetRequests[i + 1]);
                    }
                    (*numResetRequests)--;

                    saveToUsersFile(newUser, numUsers);
                } else {
                    printf("User '%s' not found.\n", resetRequests[nChoice - 1]);
                }
                system("pause");
            } else {
                bQuit = 1;
            }
        }
    }
}

void cipherPassword(char *password, char *cipheredPassword) {
    int shift = 3; // Caesar cipher shift
    for (int i = 0; password[i] != '\0'; i++) {
        cipheredPassword[i] = password[i] + shift;
    }
    cipheredPassword[strlen(password)] = '\0';
}

void AdminViewAllUsers(UserInfo newUser[MAX_USERS], int numUsers) {
    int i, j, nChoice;
    int bQuit = 0;
    int targetIndex;
    char cipheredPassword[MAX_CHAR_PASS];
    system("cls");
    printf("Users Registered in The Application:\n\n");

    while (!bQuit) {
        for (i = 0; i < numUsers; i++) {
            cipherPassword(newUser[i].password, cipheredPassword);

            printf("[%d] Username: %s\n", i + 1, newUser[i].username);
            printf("    Name: %s\n", newUser[i].name);
            printf("    Password: %s\n", cipheredPassword);

            if (strlen(newUser[i].description) == 0) {
                printf("    Description: DEFAULT USER\n");
            } else {
                printf("    Description: %s\n", newUser[i].description);
            }

            if (newUser[i].isAccountLocked) {
                printf("    Account Status: Locked\n");
            } else {
                printf("    Account Status: Unlocked\n");
            }

            // Show connections
            if (newUser[i].numConnections > 0) {
                printf("    Connections: ");
                for (j = 0; j < newUser[i].numConnections; j++) {
                    printf("%s", newUser[i].connections[j]);
                    if (j < newUser[i].numConnections - 1) {
                        printf(", ");
                    }
                }
                printf("\n\n");
            } else {
                printf("    Connections: NONE\n\n");
            }
        }

        printf("Press 0 to go back\n");
        nChoice = getValidChoice(0, 0);
        if (nChoice == 0) {
            bQuit = 1;
        }
    }
}
void AdminHandleUsersModulePage(char adminPass[MAX_CHAR_PASS], UserInfo newUser[MAX_USERS], int numUsers, String resetRequests[MAX_USERS], int *numResetRequests) {
    int nChoice, bQuit = 0;
    while (!bQuit) {
        system("cls");
        printf("ADMIN MODULE - Users Module\n");
        printf("\n");
        printf("Please choose an option:\n\n");
        printf("[1] - View Users\n");
        printf("[2] - Modify Users\n");
        printf("[3] - Refresh User Account Password\n");
        printf("[4] - Delete Users\n");
        printf("[5] - Quit\n\n");

        nChoice = getValidChoice(1, 5);

        switch (nChoice) {
            case 1:
               	AdminViewAllUsers(newUser, numUsers);
                break;
            case 2:
                // modifyUsers();
                break;
            case 3:
                refreshUserAccountPasswordPage(newUser, numUsers, resetRequests, numResetRequests);
                break;
            case 4:
                // deleteUsers();
                break;
            case 5:
                bQuit = 1;
                break;
        }
    }
}

void AdminModulePage(char adminPass[MAX_CHAR_PASS], UserInfo newUser[MAX_USERS], int numUsers, String resetRequests[MAX_USERS], int *numResetRequests) {
    int nChoice;
    int bQuit = 0;

    while (!bQuit) {
        system("cls");
        printf("ADMIN MODULE\n");
        printf("\n");
        printf("Please choose an option:\n\n");
        printf("[1] - Users Module\n");
        printf("[2] - Messages Module\n");
        printf("[3] - Change Admin Password\n");
        printf("[4] - Quit\n\n");

        nChoice = getValidChoice(1, 4);

        switch (nChoice) {
            case 1:
                AdminHandleUsersModulePage(adminPass, newUser, numUsers, resetRequests, numResetRequests);
                break;
            case 2:
                // adminHandleMessagesModule();
                break;
            case 3:
                changeAdminPass(adminPass);
                break;
            case 4:
                bQuit = 1;
                break;
        }
    }
}

int main() {
    int nChoice, bQuit = 0; // Flags
    int numUsers; // No. of users
    char adminPass[MAX_CHAR_PASS];
    UserInfo newUser[MAX_USERS];
    String resetRequests[MAX_USERS];
    int numResetRequests = 0;

    numUsers = 0;
    loadFromUsersFile(newUser, &numUsers);

    do {
        system("cls");
        printf("Welcome to the Gummiphone!\n\n");
        printf("[1] - Login\n");
        printf("[2] - Administrator Module\n");
        printf("[3] - Quit\n\n");

        nChoice = getValidChoice(1, 3);

        if (nChoice == 1) {
            LoginPage(adminPass, newUser, &numUsers, resetRequests, &numResetRequests);
        } 
		else if (nChoice == 2) {
            AdminModuleLogin(adminPass, newUser, numUsers, resetRequests, &numResetRequests);
        } 
		else if (nChoice == 3) {
            bQuit = 1;
        }

    } while (!bQuit);

    return 0;
}




