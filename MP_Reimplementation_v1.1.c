#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CHAR_MESSAGE 1000 // Maximum no. of Characters for each message / announcement
#define MAX_NO_MESSAGE 1000 //Maximum no. of Messages that can exist in the program
#define MAX_USERS 30 //Maximum no. of Users that can be registered
#define MAX_CHAR_PASS 21 // Maximum no. of characters for password
#define MAX_CHAR_SEC 101 //Maximum number of Characters for Security Questions & Answers

#define USERSFILE "UsersFile.txt"
#define ADMINPASSFILE "AdminPassFile.txt"

typedef char String[101];

typedef struct {
    char name[21];
    char username[21];
    char password[MAX_CHAR_PASS];
    char securityQuestion[MAX_CHAR_SEC];
    char securityAnswer[MAX_CHAR_SEC];
    char description[101]; 
} UserInfo;


/***********Utility Functions****************/
void clean(String strTemp);
int getValidChoice(int lower, int upper);

/****************************USER CREATION AND LOG IN FUNCTIONS*****************************/
void saveToUsersFile(UserInfo newUser[MAX_USERS], int numUsers);
void loadFromUsersFile(UserInfo newUser[MAX_USERS], int *numUsers);
int isUserValid(String username, String password, UserInfo newUser[MAX_USERS], int numUsers);
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
void userModulePage(UserInfo newUser[MAX_USERS], int numUsers, char *username);
void modifyPersonalContents(UserInfo newUser[MAX_USERS], int numUsers, char *username);
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

void modifyAccountSecurity(UserInfo newUser[MAX_USERS], int numUsers, char *username) {
    UserInfo userTemp;
    char currentPassword[MAX_CHAR_PASS];
    int i, userIndex = -1, verified = 0, done = 0;

    // Find user by username
    for (i = 0; i < numUsers && userIndex == -1; i++) {
        if (strcmp(newUser[i].username, username) == 0) {
            userIndex = i;
        }
    }

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

void modifyPersonalContents(UserInfo newUser[MAX_USERS], int numUsers, char *username) {
    UserInfo userTemp;
    char tempName[21], tempDescription[101];
    char passwordCheck[MAX_CHAR_PASS];
    int i, userIndex = -1, verified = 0, done = 0, inputValid = 0;
	clean(username);

    // Find user by username
    for (i = 0; i < numUsers && userIndex == -1; i++) {
        if (!strcmp(newUser[i].username, username)) {
            userIndex = i;
        }
    }

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

void userModulePage(UserInfo newUser[MAX_USERS], int numUsers, char *username) {
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
                modifyPersonalContents(newUser, numUsers, username);
                break;
            case 6:
                modifyAccountSecurity(newUser, numUsers, username);
                break;
            case 7:
                //modifyPersonalConnections(newUser, numUsers, username);
                break;
            case 8:
                //browseUsers(newUser, numUsers);
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
            // Ensure all 6 fields are read correctly
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
            return 1; // Valid user
        }
    }
    return 0; // Invalid user
}

void Login(UserInfo newUser[MAX_USERS], int *numUsers) {
    String nameInput;
    String passInput;
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
				userModulePage(newUser, *numUsers, nameInput);
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





