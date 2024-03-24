#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Book structure
struct Book {
    int bookID;
    char title[50];
    char author[50];
    int isIssued;
    struct Book *next;
};

// User structure
struct User {
    int userID;
    char username[50];
    int isBlocked;
    struct User *next;
};


// Define maximum number of users
#define MAX_USERS 10

// Structure to store user information
struct admin {
    char username[50];
    char password[50];
};

// Function prototypes
void registerUser(struct admin users[], int *userCount);
int loginUser(struct admin users[], int userCount);
void displayMenu();

struct Book *createBook(int bookID, const char *title, const char *author);
struct User *createUser(int userID, const char *username);
void addBook(struct Book **library, int bookID, const char *title, const char *author);
void deleteBook(struct Book **library, int bookID);
void searchBook(struct Book *library, int bookID);
void addUser(struct User **users, int userID, const char *username);
void deleteUser(struct User **users, int userID);
void blockUser(struct User *users, int userID);
void freezeUser(struct User *users, int userID);
void issueBook(struct Book *library, struct User *users, int bookID, int userID);
void findDues(struct Book *library, struct User *users, int userID);
void totalBooks(struct Book *library);


int login() {
    struct admin users[MAX_USERS];
    int userCount = 0;
    int choice;

    do {
        printf("\nLibrary Management System Authentication\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerUser(users, &userCount);
                break;
            case 2:
                if (loginUser(users, userCount))
                   { printf("Login successful!\n");
                     displayMenu();
                     printf("Enter your choice: ");
                     scanf("%d", &choice);
                     }
                else
                    printf("Login failed. Invalid username or password.\n");
                break;
            case 3:
                printf("Exiting the program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);

    return 0;
}

// Function to register a new user
void registerUser(struct admin users[], int *userCount) {
    if (*userCount < MAX_USERS) {
        printf("Enter username: ");
        scanf("%s", users[*userCount].username);

        printf("Enter password: ");
        scanf("%s", users[*userCount].password);

        (*userCount)++;
        printf("User registered successfully!\n");
    } else {
        printf("Maximum number of users reached. Cannot register more users.\n");
    }
}

// Function to login a user
int loginUser(struct admin users[], int userCount) {
    char username[50];
    char password[50];

    printf("Enter username: ");
    scanf("%s", username);

    printf("Enter password: ");
    scanf("%s", password);

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return 1; // Login successful
        }
    }

    return 0; // Login failed
}



// Function to initialize a new book
struct Book *createBook(int bookID, const char *title, const char *author) {
    struct Book *newBook = (struct Book *)malloc(sizeof(struct Book));
    newBook->bookID = bookID;
    strcpy(newBook->title, title);
    strcpy(newBook->author, author);
    newBook->isIssued = 0;
    newBook->next = NULL;
    return newBook;
}

// Function to initialize a new user
struct User *createUser(int userID, const char *username) {
    struct User *newUser = (struct User *)malloc(sizeof(struct User));
    newUser->userID = userID;
    strcpy(newUser->username, username);
    newUser->isBlocked = 0;
    newUser->next = NULL;
    return newUser;
}

// Function to add a book to the library
void addBook(struct Book **library, int bookID, const char *title, const char *author) {
    struct Book *newBook = createBook(bookID, title, author);
    newBook->next = *library;
    *library = newBook;
    printf("Book added successfully.\n");
}

// Function to delete a book from the library
void deleteBook(struct Book **library, int bookID) {
    struct Book *current = *library;
    struct Book *prev = NULL;

    while (current != NULL && current->bookID != bookID) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Book not found.\n");
        return;
    }

    if (prev == NULL) {
        *library = current->next;
    } else {
        prev->next = current->next;
    }

    free(current);
    printf("Book deleted successfully.\n");
}

// Function to search for a book in the library
void searchBook(struct Book *library, int bookID) {
    struct Book *current = library;

    while (current != NULL) {
        if (current->bookID == bookID) {
            printf("Book found:\n");
            printf("ID: %d\nTitle: %s\nAuthor: %s\n", current->bookID, current->title, current->author);
            return;
        }
        current = current->next;
    }

    printf("Book not found.\n");
}

// Function to add a user to the system
void addUser(struct User **users, int userID, const char *username) {
    struct User *newUser = createUser(userID, username);
    newUser->next = *users;
    *users = newUser;
    printf("User added successfully.\n");
}

// Function to delete a user from the system
void deleteUser(struct User **users, int userID) {
    struct User *current = *users;
    struct User *prev = NULL;

    while (current != NULL && current->userID != userID) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("User not found.\n");
        return;
    }

    if (prev == NULL) {
        *users = current->next;
    } else {
        prev->next = current->next;
    }

    free(current);
    printf("User deleted successfully.\n");
}

// Function to block a user
void blockUser(struct User *users, int userID) {
    struct User *current = users;

    while (current != NULL) {
        if (current->userID == userID) {
            current->isBlocked = 1;
            printf("User blocked successfully.\n");
            return;
        }
        current = current->next;
    }

    printf("User not found.\n");
}

// Function to freeze a user
void freezeUser(struct User *users, int userID) {
    struct User *current = users;

    while (current != NULL) {
        if (current->userID == userID) {
            current->isBlocked = 0;
            printf("User unfrozen successfully.\n");
            return;
        }
        current = current->next;
    }

    printf("User not found.\n");
}

// Function to issue a book to a user
void issueBook(struct Book *library, struct User *users, int bookID, int userID) {
    struct Book *book = library;
    struct User *user = users;

    while (book != NULL && book->bookID != bookID) {
        book = book->next;
    }

    while (user != NULL && user->userID != userID) {
        user = user->next;
    }

    if (book == NULL || user == NULL) {
        printf("Book or user not found.\n");
        return;
    }

    if (book->isIssued) {
        printf("Book is already issued.\n");
        return;
    }

    if (user->isBlocked) {
        printf("User is blocked. Cannot issue the book.\n");
        return;
    }

    book->isIssued = 1;
    printf("Book issued successfully.\n");
}

// Function to find dues for a user
void findDues(struct Book *library, struct User *users, int userID) {
    struct User *user = users;

    while (user != NULL && user->userID != userID) {
        user = user->next;
    }

    if (user == NULL) {
        printf("User not found.\n");
        return;
    }

    struct Book *book = library;
    int dueCount = 0;

    while (book != NULL) {
        if (book->isIssued && book->bookID == user->userID) {
            dueCount++;
        }
        book = book->next;
    }

    printf("User %s has %d book(s) overdue.\n", user->username, dueCount);
}

// Function to display the total number of books in the library
void totalBooks(struct Book *library) {
    int count = 0;
    struct Book *current = library;

    while (current != NULL) {
        count++;
        current = current->next;
    }

    printf("Total number of books in the library: %d\n", count);
}

// Function to display the main menu
void displayMenu(struct Book *library, struct User *users) {
    int choice;
    printf("\t\t\t     ***     Library Management System     ***\n");  


    printf("1. Add Book\n");
    printf("2. Delete Book\n");
    printf("3. Search Book\n");
    printf("4. Issue Book\n");
    printf("5. Total Books\n");
    printf("6. Add User\n");
    printf("7. Delete User\n");
    printf("8. Block User\n");
    printf("9. Freeze User\n");
    printf("10. Find Dues\n");
    printf("0. Exit\n");




    do {
        
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                int bookID;
                char title[50], author[50];
                printf("Enter Book ID: ");
                scanf("%d", &bookID);
                printf("Enter Title: ");
                scanf("%s", title);
                printf("Enter Author: ");
                scanf("%s", author);
                addBook(&library, bookID, title, author);
                break;
            }
            case 2: {
                int bookID;
                printf("Enter Book ID to delete: ");
                scanf("%d", &bookID);
                deleteBook(&library, bookID);
                break;
            }
            case 3: {
                int bookID;
                printf("Enter Book ID to search: ");
                scanf("%d", &bookID);
                searchBook(library, bookID);
                break;
            }
            case 4: {
                int bookID, userID;
                printf("Enter Book ID to issue: ");
                scanf("%d", &bookID);
                printf("Enter User ID: ");
                scanf("%d", &userID);
                issueBook(library, users, bookID, userID);
                break;
            }
            case 5: {
                totalBooks(library);
                break;
            }
            case 6: {
                int userID;
                char username[50];
                printf("Enter User ID: ");
                scanf("%d", &userID);
                printf("Enter Username: ");
                scanf("%s", username);
                addUser(&users, userID, username);
                break;
            }
            case 7: {
                int userID;
                printf("Enter User ID to delete: ");
                scanf("%d", &userID);
                deleteUser(&users, userID);
                break;
            }
            case 8: {
                int userID;
                printf("Enter User ID to block: ");
                scanf("%d", &userID);
                blockUser(users, userID);
                break;
            }
            case 9: {
                int userID;
                printf("Enter User ID to freeze: ");
                scanf("%d", &userID);
                freezeUser(users, userID);
                break;
            }
            case 10: {
                int userID;
                printf("Enter User ID to find dues: ");
                scanf("%d", &userID);
                findDues(library, users, userID);
                break;
            }
            case 0: {
                printf("Exiting...\n");
                break;
            }
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);

    // Free allocated memory before exiting
    struct Book *tempBook;
    while (library != NULL) {
        tempBook = library;
        library = library->next;
        free(tempBook);
    }

    struct User *tempUser;
    while (users != NULL) {
        tempUser = users;
        users = users->next;
        free(tempUser);
    }

    
}



int main() {
    struct Book *library = NULL;
    struct User *users = NULL;
    int choice;

        login();
      return 0;
} 