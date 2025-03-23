#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINES 101
#define MAX_CHAR_MESSAGE 100 // Maximum no. of Characters for each message / announcement
#define MAX_MESSAGES 100 //Maximum no. of Messages that can exist in the program
#define MAX_USERS 30 //Maximum no. of Users that can be registered
#define MAX_CHAR_PASS 21 // Maximum no. of characters for password
#define MAX_CHAR_SEC 101 //Maximum number of Characters for Security Questions & Answers

#define USERSFILE "UsersFile.txt"
#define MESSAGESFILE "MessagesFile.txt"
#define ADMINPASSFILE "AdminPassFile.txt"

typedef char String[101];

typedef struct {
    char name[21];
    char username[21];
    char password[MAX_CHAR_PASS];
    char securityQuestion[MAX_CHAR_SEC];
    char securityAnswer[MAX_CHAR_SEC];
} UserInfo;

typedef struct { 
	int numLines;
    char strSender[21];
    char  strReceiver[21];
    char strSubject[101];
    String strMessageEntry[MAX_LINES];
} messageTag;

void saveToMessagesFile(messageTag *newMessage, int numNewMessages);
void loadFromMessagesFile(messageTag newMessage[MAX_MESSAGES], int *numMessages);
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

/********************
USER MODULE FUNCTIONS
*********************/

void saveToMessagesFile(messageTag *newMessage, int numNewMessages) {
    FILE *pFile;
    messageTag oldMessages[MAX_MESSAGES];
    int existingMessages = 0, totalMessages = 0;
    int i, j;

    loadFromMessagesFile(oldMessages, &existingMessages);

    totalMessages = existingMessages + numNewMessages;

    printf("Total Count: %d, Existing count: %d, Old count: %d", totalMessages, existingMessages, numNewMessages);

    // Open in write mode to rewrite everything
    pFile = fopen(MESSAGESFILE, "wt");
    if (pFile == NULL) {
        printf("Error writing to %s\n", MESSAGESFILE);
        return;
    }

    // Write total message count
    fprintf(pFile, "%d\n", totalMessages);

    // Write old messages
    for (i = 0; i < existingMessages; i++) {
        fprintf(pFile, "%s-%s-%s-%d\n",
                oldMessages[i].strSender,
                oldMessages[i].strReceiver,
                oldMessages[i].strSubject,
                oldMessages[i].numLines);
        for (j = 0; j < oldMessages[i].numLines; j++) {
            fprintf(pFile, "%s\n", oldMessages[i].strMessageEntry[j]);
        }
    }

    // Write new messages
    for (i = 0; i < numNewMessages; i++) {
        fprintf(pFile, "%s-%s-%s-%d\n",
                newMessage[i].strSender,
                newMessage[i].strReceiver,
                newMessage[i].strSubject,
                newMessage[i].numLines);
        for (j = 0; j < newMessage[i].numLines; j++) {
        fprintf(pFile, "%s\n", newMessage[i].strMessageEntry[j]);
    }
    }
    fclose(pFile);
}

void composePersonalMessage(char sender[21], messageTag newMessage[MAX_MESSAGES],int *numMessages) { 
	String strTemp; // Place inputs here for safekeeping
	messageTag messageTemp;
	int bEnd = 0, numLines = 0;
	
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
	printf ("Creating a personal message!\n\n");
	
	//Input receiver
	printf("To: ");
	fgets(messageTemp.strReceiver, sizeof(messageTemp.strReceiver), stdin);
	clean(messageTemp.strReceiver);
	//Input subject
	printf("Subject: ");
	fgets(messageTemp.strSubject, sizeof(messageTemp.strSubject), stdin);
	clean(messageTemp.strSubject);
	
	printf("Write your message below. Type \"END\" to finish writing.\n\n");
    memset(newMessage[*numMessages].strMessageEntry, 0, sizeof(newMessage[*numMessages].strMessageEntry));
    // Get multiple lines for the message body
    while (!bEnd) {
        fgets(strTemp, sizeof(strTemp), stdin);
        clean(strTemp);
        if (!strcmp(strTemp, "END")) {
            bEnd = 1;
        } 
		else {
            strcpy(newMessage[*numMessages].strMessageEntry[numLines], strTemp);
            numLines++;
        }
	}

	if (*numMessages < MAX_MESSAGES) {
        // Prepare a single message to pass into the save function
        messageTag temp[1];
        strcpy(temp[0].strSender, sender);
        strcpy(temp[0].strReceiver, messageTemp.strReceiver);
        strcpy(temp[0].strSubject, messageTemp.strSubject);
        temp[0].numLines = numLines;

        for (int i = 0; i < numLines; i++) {
            strcpy(temp[0].strMessageEntry[i], newMessage[*numMessages].strMessageEntry[i]);
        }

        // Save only the one new message
        saveToMessagesFile(temp, 1);

        // Add to main array
        newMessage[*numMessages] = temp[0];
        (*numMessages)++;
    } else {
        printf("Message storage is full. Cannot add more messages.\n");
    }
    
    printf("\n====================\n");
    printf("📨 All Messages in Memory:\n");
    printf("====================\n");

    for (int i = 0; i < *numMessages; i++) {
        printf("📧 Message #%d\n", i + 1);
        printf("From    : %s\n", newMessage[i].strSender);
        printf("To      : %s\n", newMessage[i].strReceiver);
        printf("Subject : %s\n", newMessage[i].strSubject);
        printf("Lines   : %d\n", newMessage[i].numLines);
        printf("Message :\n");
        for (int j = 0; j < newMessage[i].numLines; j++) {
            printf("  %s\n", newMessage[i].strMessageEntry[j]);
        }
        printf("--------------------\n");
    }
}


void loadFromMessagesFile(messageTag newMessage[MAX_MESSAGES], int *numMessages) {
    FILE *pFile = fopen(MESSAGESFILE, "rt");
    String strTemp;
    int i, j;

    if (pFile != NULL) {
        // Read the number of messages
        fgets(strTemp, sizeof(strTemp), pFile);
        *numMessages = atoi(strTemp); // Convert to integer

        for (i = 0; i < *numMessages; i++) {
            // Read the line containing sender, receiver, subject, and numLines
            fgets(strTemp, sizeof(strTemp), pFile);
            clean(strTemp);

            // Parse the string using sscanf (without strtok)
            int numLines;
            sscanf(strTemp, "%20[^-]-%20[^-]-%100[^\n]-%d",
                   newMessage[i].strSender,
                   newMessage[i].strReceiver,
                   newMessage[i].strSubject,
                   &numLines);

            newMessage[i].numLines = numLines;

            // Read the message lines
            for (j = 0; j < newMessage[i].numLines; j++) {
                fgets(strTemp, sizeof(strTemp), pFile);
                clean(strTemp);
                strcpy(newMessage[i].strMessageEntry[j], strTemp);
            }
        }
        fclose(pFile);
    } else {
        printf("Error reading %s\n", MESSAGESFILE);
    }
}

int main() {
    messageTag messages[MAX_MESSAGES];
    int numMessages = 0;
    int bEnd = 0;
    int choice;
    char sender[21] = "JP";

    // Load existing messages from file
    loadFromMessagesFile(messages, &numMessages);

    do {
        // Display menu
        printf("\n===== MESSAGE SYSTEM =====\n");
        printf("1. Compose a Personal Message\n");
        printf("2. Exit\n");
        
        // Get valid choice from user
        choice = getValidChoice(1, 2);

		switch (choice) {
		    case 1:
		        composePersonalMessage(sender, messages, &numMessages);
				break;
		
		    case 2:
			    // saveToMessagesFile(messages, numMessages);
			    printf("\nExiting... Goodbye!\n");
			    bEnd = 1;
			    break;
		}

    } while (!bEnd);
    return 0;
}



	
