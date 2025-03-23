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
    int numReceivers;
    char strSender[21];
    char strReceivers[10][21]; // Support up to 10 receivers
    char strSubject[101];
    String strMessageEntry[MAX_LINES];
} messageTag;

void printMessageDetails(messageTag message, int index);
void viewSentMessages(char user[], messageTag SavedMessages[MAX_MESSAGES], int totalMessages, messageTag SentMessages[MAX_MESSAGES], int *sentCount);
void viewAnnouncements(messageTag SavedMessages[MAX_MESSAGES], int totalMessages, messageTag Announcements[MAX_MESSAGES], int *AnnounceCount);
void splitReceivers(char receiverBuffer[], char receiversArray[10][21], int *numReceivers);
void loadAllMessagesFiles(messageTag Messages[MAX_MESSAGES], int *numMessages);
void viewReceivedMessages(char user[], messageTag SavedMessages[MAX_MESSAGES], int totalMessages, messageTag Received[MAX_MESSAGES], int *receiveCount);
void saveMessagesToFile(messageTag SavedMessages[MAX_MESSAGES], int savedCount);
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

void splitReceivers(char receiverBuffer[], char receiversArray[10][21], int *numReceivers) {
    *numReceivers = 0;
    char *token = strtok(receiverBuffer, ",");
    while(token != NULL && *numReceivers < 10){
        // printf("Token saving to Receivers: %s\n", token);
        strncpy(receiversArray[*numReceivers], token, 20);
        receiversArray[*numReceivers][20] = '\0'; // ensures that it is null terminated
        (*numReceivers)++;
        token = strtok(NULL, ",");
    }
}

void loadAllMessagesFile(messageTag Messages[MAX_MESSAGES], int *numMessages) {
    
    FILE *pFile = fopen(MESSAGESFILE, "rt");
    String strTemp;
    int i, j;
    if(pFile == NULL) {
        *numMessages = 0;
        return;
    }

    fgets(strTemp, sizeof(strTemp), pFile);
    *numMessages = atoi(strTemp);

    for(i = 0; i < *numMessages; i++) {
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

        for (j = 0; j < numLines; j++) {
            fgets(strTemp, sizeof(strTemp), pFile);
            clean(strTemp);
            strcpy(Messages[i].strMessageEntry[j], strTemp);
        }
    }
    
}

void printMessageDetails(messageTag message, int index) {
    int j;

    printf("Message #%d\n", index + 1);
    printf("From    : %s\n", message.strSender);

    printf("To      : ");
    for (j = 0; j < message.numReceivers; j++) {
        printf("%s", message.strReceivers[j]);
        if (j < message.numReceivers - 1)
            printf(", ");
    }
    printf("\n");

    printf("Subject : %s\n", message.strSubject);
    printf("Message :\n");
    for (j = 0; j < message.numLines; j++) {
        printf("  %s\n", message.strMessageEntry[j]);
    }
    printf("--------------------\n");
}

void viewSentMessages(char user[], messageTag SavedMessages[MAX_MESSAGES], int totalMessages, messageTag SentMessages[MAX_MESSAGES], int *sentCount) {
    int i, j;

    if(totalMessages == 0) {
        printf("No Mail Found.");
        return;
    } 

    for (i = 0; i < totalMessages; i++) {
        if(strcmp(SavedMessages[i].strSender, user) == 0) {
            SentMessages[*sentCount] = SavedMessages[i];
            (*sentCount)++;
        }
    }
    for (i = 0; i < *sentCount; i++) {
        printMessageDetails(SentMessages[i], i);
    }
    
}

void viewAnnouncements(messageTag SavedMessages[MAX_MESSAGES], int totalMessages, messageTag Announcements[MAX_MESSAGES], int *AnnounceCount) {
    int i, j;
    
    if(totalMessages == 0) {
        printf("No Mail Found.");
        return;
    }

    for (i = 0; i < totalMessages; i++) {
        int isAnnouncement = 0;
        for (j = 0; j < SavedMessages[i].numReceivers; j++) {
            if(strcmp(SavedMessages[i].strReceivers[j], "everyone") == 0) {
                isAnnouncement = 1;
            }
        }
        if (isAnnouncement) {
            Announcements[*AnnounceCount] = SavedMessages[i];
            (*AnnounceCount)++;
        }
    }

    for (i = 0; i < *AnnounceCount; i++) {
        printMessageDetails(Announcements[i], i);
    }
    
}

void viewReceivedMessages(char user[], messageTag SavedMessages[MAX_MESSAGES], int totalMessages, messageTag Received[MAX_MESSAGES], int *receiveCount) {
    int i, j;

    if(totalMessages == 0) {
        printf("No Mail Found.");
        return;
    } 

    for (i = 0; i < totalMessages; i++) {
        int isSentToUser = 0;
        for (j = 0; j < SavedMessages[i].numReceivers; j++) {
            if(strcmp(SavedMessages[i].strReceivers[j], user) == 0) {
                isSentToUser = 1;
            }
        }
        if (isSentToUser) {
            Received[*receiveCount] = SavedMessages[i];
            (*receiveCount)++;
        }
    }

    for (i = 0; i < *receiveCount; i++) {
        printMessageDetails(Received[i], i);
    }
    
}

void saveMessagesToFile(messageTag SavedMessages[MAX_MESSAGES], int savedCount) {
    FILE *pFile = fopen(MESSAGESFILE, "wt");
    int i, j;

    if (pFile == NULL) {
        printf("Error opening %s for writing.\n", MESSAGESFILE);
        return;
    }

    // Write number of messages
    fprintf(pFile, "%d\n", savedCount);

    // Write each message
    for (i = 0; i < savedCount; i++) {
        // Write sender | receivers | subject | numLines
        fprintf(pFile, "%s|", SavedMessages[i].strSender);
        for (j = 0; j < SavedMessages[i].numReceivers; j++) {
            fprintf(pFile, "%s", SavedMessages[i].strReceivers[j]);
            if (j < SavedMessages[i].numReceivers - 1)
                fprintf(pFile, ",");
        }
        fprintf(pFile, "|%s|%d\n", SavedMessages[i].strSubject, SavedMessages[i].numLines);

        // Write message body lines
        for (j = 0; j < SavedMessages[i].numLines; j++) {
            fprintf(pFile, "%s\n", SavedMessages[i].strMessageEntry[j]);
        }
    }

    fclose(pFile);
    printf("💾 Messages saved to %s successfully!\n", MESSAGESFILE);
}

void composeMessage(char user[], messageTag SavedMessages[MAX_MESSAGES], int *savedCount) {
    if(*savedCount > 100) {
        printf("Message limit reached. Cannot compose more messages.\n");
        return;
    }

    messageTag newMessage;
    String strTemp;
    int numLines = 0, done = 0;

    printf("Add a Subject: ");
    fgets(newMessage.strSubject, sizeof(newMessage.strSubject), stdin);
    clean(newMessage.strSubject);

    strcpy(newMessage.strSender, user);
    printf("To (Seperate with Commas or Single User): ");
    fgets(strTemp, sizeof(strTemp), stdin);
    clean(strTemp);
    splitReceivers(strTemp, newMessage.strReceivers, &newMessage.numReceivers);

    printf("Write your message. Type 'END' on a new line to finish:\n");
    while (!done && numLines < MAX_LINES) {
        fgets(strTemp, sizeof(strTemp), stdin);
        clean(strTemp);
        if (strcmp(strTemp, "END") == 0)
            done = 1;
        else {
            strcpy(newMessage.strMessageEntry[numLines], strTemp);
            numLines++;
        }
    }
    newMessage.numLines = numLines;

    SavedMessages[*savedCount] = newMessage;
    (*savedCount)++;

    saveMessagesToFile(SavedMessages, *savedCount);
    printf("✅ Message composed successfully!\n");
}


int main() {
    messageTag messages[MAX_MESSAGES];
    messageTag sentMessages[MAX_MESSAGES];
    messageTag Announcements[MAX_MESSAGES];
    messageTag Received[MAX_MESSAGES];
    int sentCount = 0;
    int announcementCount = 0;
    int receiveCount = 0;
    int numMessages = 0;
    int bEnd = 0;
    int choice;
    char sender[21] = "JP";

    // Load existing messages from file
    loadAllMessagesFile(messages, &numMessages);

    int i, r, j;
    for (i = 0; i < numMessages; i++) {
        printf("📨 Sent From User: %s\n", messages[i].strSender);

        printf("To      : ");
        for (r = 0; r < messages[i].numReceivers; r++) {
            printf("%s", messages[i].strReceivers[r]);
            if (r < messages[i].numReceivers - 1) {
                printf(", ");
            }
        }

        printf("\n");

        printf("Subject : %s\n", messages[i].strSubject);
        printf("Message :\n");

        for (j = 0; j < messages[i].numLines; j++) {  
            printf(" %s\n", messages[i].strMessageEntry[j]);
        }
    }

    do {
        // Display menu
        printf("\n===== MESSAGE SYSTEM =====\n");
        printf("1. Compose a Personal Message\n");
        printf("2. Inbox\n");
        printf("3. Sent\n");
        printf("4. View Announcements\n");
        printf("5. Modify Personal Contents\n");
        printf("6. Modify Account Security\n");
        printf("7. Modify Personal Connections\n");
        printf("8. Browse Users\n");
        printf("9. Log Out\n");

        // Get valid choice from user
        choice = getValidChoice(1, 9);

		switch (choice) {
		    case 1:
                composeMessage(sender, messages, &numMessages);
				break;
            case 2:
                viewReceivedMessages(sender, messages, numMessages, Received, &receiveCount);
				break;
            case 3:
                viewSentMessages(sender, messages, numMessages, sentMessages, &sentCount);
                // in progress
				break;
            case 4:
                viewAnnouncements(messages, numMessages, Announcements, &announcementCount);
				break;
            case 5:
                // in progress
				break;
            case 6:
                // in progress
				break;
            case 7:
                // in progress
				break;
            case 8:
                // in progress
				break;
		    case 9:
			    printf("\nExiting... Goodbye!\n");
			    bEnd = 1;
			    break;
		}

    } while (!bEnd);

    return 0;
}