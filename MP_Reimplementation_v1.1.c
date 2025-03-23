#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CONNECTIONS 10
#define MAX_CHAR_USER 21
#define MAX_CHAR_PASS 21
#define MAX_CHAR_MESSAGE 1000 // Maximum no. of Characters for each message / announcement
#define MAX_NO_MESSAGE 1000 //Maximum no. of Messages that can exist in the program
#define MAX_USERS 30 //Maximum no. of Users that can be registered
#define MAX_CHAR_PASS 21 // Maximum no. of characters for password
#define MAX_CHAR_SEC 101 //Maximum number of Characters for Security Questions & Answers


#define USERSFILE "UsersFile.txt"
#define ADMINPASSFILE "AdminPassFile.txt"

typedef char String[101];

typedef struct {
	int numConnections;
    char name[MAX_CHAR_USER];
    char username[MAX_CHAR_USER];
    char password[MAX_CHAR_PASS];
    char securityQuestion[MAX_CHAR_SEC];
    char securityAnswer[MAX_CHAR_SEC];
    char description[101]; 
    char connections[MAX_CONNECTIONS][MAX_CHAR_USER];
} UserInfo;


/***********Utility Functions****************/
void clean(String strTemp);
int getValidChoice(int lower, int upper);

/****************************USER CREATION AND LOG IN FUNCTIONS*****************************/
void saveToUsersFile(UserInfo newUser[MAX_USERS], int numUsers);
void loadFromUsersFile(UserInfo newUser[MAX_USERS], int *numUsers);
int isUserValid(String username, String password, UserInfo newUser[MAX_USERS], int numUsers);
int getUserIndex(String username, String password, UserInfo newUser[MAX_USERS], int numUsers);
void Login(UserInfo newUser[MAX_USERS], int *numUsers);
void LoginPage(UserInfo newUser[MAX_USERS], int *numUsers);
int isUsernameTaken(UserInfo newUser[MAX_USERS], int numUsers, char *username);
void createNewAccount(UserInfo newUser[MAX_USERS], int *numUsers);
/********************************************************************************************/

/************************************ADMIN FUNCTIONS*****************************************/
void saveToAdminPassFile(char adminPass[MAX_CHAR_PASS]);
void loadFromAdminPassFile(char adminPass[MAX_CHAR_PASS]);
void createNewAdminPass(char adminPass[MAX_CHAR_PASS]);
void changeAdminPass(char adminPass[MAX_CHAR_PASS]);
void AdminModuleLogin(char adminPass[MAX_CHAR_PASS]);
void AdminModulePage(char adminPass[MAX_CHAR_PASS]);
/********************************************************************************************/

/***********************************USER MODULE FUNCTIONS************************************/
void viewUserConnections(UserInfo newUser[MAX_USERS], int numUsers, int userIndex); // 7.4
void removePersonalConnection(UserInfo newUser[MAX_USERS], int userIndex); // 7.3
void viewPersonalConnections(UserInfo newUser[MAX_USERS], int userIndex); // 7.2
void addPersonalConnection(UserInfo newUser[MAX_USERS], int numUsers, int userIndex); // 7.1
void modifyPersonalConnectionsPage(UserInfo newUser[MAX_USERS], int numUsers, char *username, int userIndex); //  7
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
**********************/

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


void viewUserPage(UserInfo newUser[MAX_USERS], int numUsers, int targetIndex, int userIndex) { // 8.3
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
	        //sendPrivateMessage(newUser, numUsers, targetIndex);
	    }
		else if (choice ==3)
			bGoBack = 1;
	} while (!bGoBack);
}

void filterUsersByName(UserInfo newUser[MAX_USERS], int numUsers, int userIndex) { // 8.2
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
        viewUserPage(newUser, numUsers, targetIndex, userIndex);  
    }
}

void viewAllUsers(UserInfo newUser[MAX_USERS], int numUsers, int userIndex) { // 8.1
    int i, nChoice;
	int targetIndex;
    system("cls");
    printf("Users Registered in The Application:\n\n");

    for (i = 0; i < numUsers; i++) {
        printf("[%d] Username: %s\n", i + 1, newUser[i].username);
        printf("    Name: %s\n", newUser[i].name);

        if (strlen(newUser[i].description) == 0) {
            printf("    Description: DEFAULT USER\n\n");
        } else {
            printf("    Description: %s\n\n", newUser[i].description);
        }
    }

    printf("[0] - Go back\n\n");
    nChoice = getValidChoice(0, numUsers);
    targetIndex = nChoice - 1;
    if (nChoice != 0) {
        viewUserPage(newUser, numUsers, targetIndex, userIndex);  
    }
}

void browseUsers(UserInfo newUser[MAX_USERS], int numUsers, int userIndex) { // 8
    int nChoice;
    
    do {
        system("cls");
        printf("Browse Users:\n");
        printf("[1] - View All Users\n");
        printf("[2] - Search by Name\n");
        printf("[3] - Go back\n\n");
        
        nChoice = getValidChoice(1, 3);

        if (nChoice == 1) {
            viewAllUsers(newUser, numUsers, userIndex);
        } 
        else if (nChoice == 2) {
            filterUsersByName(newUser, numUsers, userIndex);
        }
    } while (nChoice != 3);
}

void viewUserConnections(UserInfo newUser[MAX_USERS], int numUsers, int userIndex) { // 7.4
    int i, j, found = 0;

    system("cls");
    printf("Users Who Have You in Their Connections:\n\n");

    for (i = 0; i < numUsers; i++) {
        if (i != userIndex) {  // Skip checking current user
            for (j = 0; j < newUser[i].numConnections; j++) {
                if (strcmp(newUser[i].connections[j], newUser[userIndex].username) == 0) {
                    printf("- %s\n", newUser[i].username);
                    found = 1;
                }
            }
        }
    }

    if (!found) {
        printf("No users have added you to their connections.\n");
    }

    system("pause");
}

void removePersonalConnection(UserInfo newUser[MAX_USERS], int userIndex) { // 7.3
    char connectionName[MAX_CHAR_USER];
    int i, j, connectionIndex = -1, found = 0;

    system("cls");
    printf("Remove Personal Connection\n\n");

    if (newUser[userIndex].numConnections > 0) {
        printf("Your Current Connections:\n");
        for (i = 0; i < newUser[userIndex].numConnections; i++) {
            printf("%d. %s\n", i + 1, newUser[userIndex].connections[i]);
        }

        printf("\nEnter the username to remove: ");
        fgets(connectionName, sizeof(connectionName), stdin);
        clean(connectionName);

        // Check if the connection exists
        for (i = 0; i < newUser[userIndex].numConnections; i++) {
            if (strcmp(newUser[userIndex].connections[i], connectionName) == 0) {
                connectionIndex = i;
                found = 1;
            }
        }

        // If connection found, proceed with removal
        if (found) {
            for (j = connectionIndex; j < newUser[userIndex].numConnections - 1; j++) {
                strcpy(newUser[userIndex].connections[j], newUser[userIndex].connections[j + 1]);
            }
            newUser[userIndex].numConnections--;
            printf("Connection removed successfully!\n");
        } 
        else {
            printf("User not found in your personal connections.\n");
        }
    } 
    else {
        printf("You have no personal connections to remove.\n");
    }

    system("pause");
}

void viewPersonalConnections(UserInfo newUser[MAX_USERS], int userIndex) { // 7.2
    int i, hasConnections = 0;

    system("cls");
    printf("Your Personal Connections:\n\n");

    // Check if the user has any connections
    if (newUser[userIndex].numConnections > 0) {
        hasConnections = 1;  // User has connections
        for (i = 0; i < newUser[userIndex].numConnections; i++) {
            printf("%d. %s\n", i + 1, newUser[userIndex].connections[i]);
        }
    }

    if (!hasConnections) {
        printf("You have no personal connections.\n");
    }

    system("pause");
}

void addPersonalConnection(UserInfo newUser[MAX_USERS], int numUsers, int userIndex) { // 7.1
    char connectionName[MAX_CHAR_USER];
    int connectionIndex = -1, alreadyConnected = 0, validConnection = 0;
    int i, j;

    printf("\nEnter the username to add: ");
    fgets(connectionName, sizeof(connectionName), stdin);
    clean(connectionName);

    // Check if the username matches the current user (cannot add yourself)
    if (strcmp(connectionName, newUser[userIndex].username) == 0) {
        printf("You cannot add yourself as a personal connection.\n");
        validConnection = 0;  // Prevent proceeding
    } 
	else {
        // Find if the username exists in the list of users
        for (i = 0; i < numUsers; i++) {
            if (strcmp(newUser[i].username, connectionName) == 0) {
                connectionIndex = i;
                validConnection = 1;  // User found, connection is valid
            }
        }
    }

    // Check if the user exists and is not the current user
    if (validConnection) {
        // Check if the user is already connected
        for (j = 0; j < newUser[userIndex].numConnections && !alreadyConnected; j++) {
            if (strcmp(newUser[userIndex].connections[j], connectionName) == 0) {
                alreadyConnected = 1;
            }
        }

        // Proceed to add connection if not already connected
        if (!alreadyConnected) {
            if (newUser[userIndex].numConnections < MAX_CONNECTIONS) {
                strcpy(newUser[userIndex].connections[newUser[userIndex].numConnections], connectionName);
                newUser[userIndex].numConnections++;
                printf("Connection added successfully!\n");
            } else {
                printf("You have reached the maximum number of personal connections.\n");
            }
        } else {
            printf("User is already in your personal connections.\n");
        }
    } 
    else if (connectionIndex == -1) {
        printf("Invalid username or user not found.\n");
    }

    system("pause");
}

void modifyPersonalConnectionsPage(UserInfo newUser[MAX_USERS], int numUsers, char *username, int userIndex) { // 7
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
	                removePersonalConnection(newUser, userIndex);
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

void modifyPersonalContents(UserInfo newUser[MAX_USERS], int numUsers, char *username, int userIndex) {// 5
    UserInfo userTemp;
    char tempName[21], tempDescription[101];
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
            if (strcmp(userTemp.description, "DEFAULT USER") == 0) {
                printf("Current Description: DEFAULT USER\n");
            } 
            else {
                printf("Current Description: %s\n", userTemp.description);
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
                printf("Enter New Description: ");
                fgets(tempDescription, sizeof(tempDescription), stdin);
                clean(tempDescription);

                if (strcmp(tempDescription, "back") == 0) {
                    done = 1;
                } else {
                    inputValid = 1;
                }
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
                    // Update name and description
                    strcpy(userTemp.name, tempName);
                    if (strlen(tempDescription) == 0) {
                        strcpy(userTemp.description, "DEFAULT USER");
                    } 
                    else {
                        strcpy(userTemp.description, tempDescription);
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
    int nChoice, bQuit = 0, bLogout = 0;

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
        printf("[10] - Previous Page\n");
        printf("\n");

        nChoice = getValidChoice(1, 10);

        switch (nChoice) {
            case 1:
                //composeMessagePage(username);
                break;
            case 2:
                //viewInbox(username);
                break;
            case 3:
                //viewSentMessages(username);
                break;
            case 4:
                //viewAnnouncements();
                break;
            case 5:
                modifyPersonalContents(newUser, numUsers, username, userIndex);
                break;
            case 6:
                modifyAccountSecurity(newUser, numUsers, username, userIndex);
                break;
            case 7:
                modifyPersonalConnectionsPage(newUser, numUsers, username, userIndex);
                break;
            case 8:
                browseUsers(newUser, numUsers, userIndex);
                break;
            case 9:
                printf("\nLogging out...\n");
                bLogout = 1;
                bQuit = 1;  // Exit the module when logging out
                break;
            case 10:
                bQuit = 1;  // Go back to the previous page
                break;
        }
    } while (!bQuit);
}


/********************************
USER CREATION AND LOGIN FUNCTIONS
*********************************/

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

void saveToUsersFile(UserInfo newUser[MAX_USERS], int numUsers) { //Saves user information into file
    FILE *pFile = fopen(USERSFILE, "wt"); // Open in write mode
    int i;

    if (pFile != NULL) {
        // Save the number of users first
        fprintf(pFile, "%d\n", numUsers);

        // Loop through all users and write their information
        for (i = 0; i < numUsers; i++) {  // FIXED HERE
            fprintf(pFile, "%s-%s-%s-%s-%s-%s\n", 
                newUser[i].name,
                newUser[i].username,
                newUser[i].password,
                newUser[i].securityQuestion,
                newUser[i].securityAnswer,
                newUser[i].description);
        }
        fclose(pFile);
    } else {
        printf("Error generating %s\n", USERSFILE);
    }
}

void loadFromUsersFile(UserInfo newUser[MAX_USERS], int *numUsers) {
    FILE *pFile = fopen(USERSFILE, "rt"); // Open in read mode
    String strTemp;
    int i = 0;

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
            
            if (sscanf(strTemp, "%[^-]-%[^-]-%[^-]-%[^-]-%[^-]-%[^\n]",
                       newUser[i].name,
                       newUser[i].username,
                       newUser[i].password,
                       newUser[i].securityQuestion,
                       newUser[i].securityAnswer,
                       newUser[i].description) == 6) {
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

int getUserIndex(String username, String password, UserInfo newUser[MAX_USERS], int numUsers) { //Loops through the UsersFile to determine whether user is valid or not 
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(newUser[i].username, username) == 0 && strcmp(newUser[i].password, password) == 0) {
            return i; //Returns the found user index
        }
    }
    return -1; // User index not found
}
void Login(UserInfo newUser[MAX_USERS], int *numUsers) {
    String nameInput;
    String passInput;
    int userIndex;
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

            if (isUserValid(nameInput, passInput, newUser, *numUsers)) {
                printf("Login successful!\n");
                system("pause");
                userIndex = getUserIndex(nameInput, passInput, newUser, *numUsers);
				userModulePage(newUser, *numUsers, nameInput, userIndex);
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
    } else if (attempts == 0) {
        printf("Too many failed attempts. You are now locked out.\n");
    }
    system("pause");
}

void LoginPage(UserInfo newUser[MAX_USERS], int *numUsers) { //Login Page UI

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
                //AdminModuleLogin(adminPass);
                break;
            case 4:
                // Forgot Password
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
    pFile = fopen("adminPass.txt", "rt");

    if (pFile != NULL) {
    	fgets(adminPass, MAX_CHAR_PASS, pFile);
        clean(adminPass); // Clean newline if any
        fclose(pFile);
        
    }
    else {
    	createNewAdminPass(adminPass);
	}
}

void AdminModuleLogin(char adminPass[MAX_CHAR_PASS]) {
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
            AdminModulePage(adminPass);
			system("pause");
        } else {
            attempts--;
            printf("Incorrect password. Attempts left: %d\n", attempts);
            system("pause");
        }
    }
}

void AdminModulePage(char adminPass[MAX_CHAR_PASS]) {
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
//                adminHandleUsersModule();
                break;
            case 2:
//                adminHandleMessagesModule();
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
	int nChoice, bQuit = 0; //Flags
	int numUsers; // No. of users
	char adminPass[MAX_CHAR_PASS];	
	UserInfo newUser[MAX_USERS];
	
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
        	LoginPage(newUser, &numUsers);
		}
        else if (nChoice == 2) {
            AdminModuleLogin(adminPass);
		}
        else if (nChoice == 3) {
        	bQuit = 1;
		}
			
	} while (!bQuit);
	
	return 0;
}





