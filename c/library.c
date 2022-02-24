/* C File for Library Management System.
Author-PEDISETTI VENKATESH (18MS014)
Description:This project was done for the assignment given in CS3101-(2020 Autumn) course at IISER KOLKATA. */

#include <stdio.h>  //including Standard input output header file
#include <stdlib.h> //including Standard library header file
#include <string.h> //including String header file
#include <ctype.h>  //including the ctype header file
#include <time.h>
#include <unistd.h>

void clearScreen()
{
    const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
    write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}

int a, b, i, OD = 0, FB = 0, login = 0, TotalBooks, TotalUsers, X = 0, Y, Z;                                                                                                                                             // declaring Global integer variables
char Log[20] = "";                                                                                                                                                                                                       // declaring Global string variable, we use Log to store the ID of online user
const char DevPass[20] = "QwM!PP12#3@";                                                                                                                                                                                  // Pasword for admin Login                                                                                                                                                                           //declaring Global const char variable, we use Devpass to verify admin registration
char aa[10], bb[10], uu[20], pp[20], reID[9], buff[150], mid[10];                                                                                                                                                        // declaring some char variables that we may use any where
int dd, mm, yyyy;                                                                                                                                                                                                        // declaring int variables to store date given by admin
char DatePath[50] = "Date.txt", UsersPath[50] = "Users.txt", UsersPath2[50] = "UserTemp.txt", BooksPath[50] = "Books.txt", BooksPath2[50] = "BooksTemp.txt", ReqPath[50] = "Requests.txt", ReqPath2[50] = "ReqTemp.txt"; // declaring the char variables for the paths of files we use.
FILE *fp, *fp2, *fptr, *fptr2, *fd;                                                                                                                                                                                      // declaring FILE pointers

typedef struct // defining a structure with typedef BOOK
{
    char name[50], author[20], id[9];          // declaring char in struct to store book name, author and id
    int available, total, DD, MM, YYYY, Force; // declaring integers in struct to store Available and total copies of Book, we use DD,MM,YYYY to say the due date to user
} BOOK;                                        // struct is called as BOOK

typedef struct // defining a structure with typedef USER
{
    char name[20], id[10], password[20]; // declaring char in struct to store user name, id and password
    int hold;                            // declaring int in struct to store how many books the user hold
    BOOK b[10];                          // declaring a struct of BOOK in struct for storing the details of books that use hold.
} USER;                                  // struct is called as USER

typedef struct // defining a structure with typedef REQUEST
{
    char fromName[20], fromID[20], message[150]; // declaring all char variables
    int revDD, revMM, revYYYY;                   // Declaring all int variables
} REQUEST;                                       // struct is called as REQUEST

USER userOnline; // declaring a struct of USER

void Date() // This function reads date from file "Date.txt"
{
    dd = mm = yyyy = 0; // resetting to 0
    time_t t = time(NULL);
    struct tm date = *localtime(&t);
    dd = date.tm_mday;
    mm = date.tm_mon + 1;
    yyyy = date.tm_year + 1900;
    // fd=fopen(DatePath,"r"); //opening file for reading
    // fscanf(fd, "%d %d %d", &dd, &mm, &yyyy);    //copying data from file
    // fclose(fd); //Closing file
}

void DatePlus(int days) // This function adds number of days to current date
{
    if (mm == 2 && yyyy % 4 != 0) // 28days---for the case of feb in non-leap year
    {
        if (dd > 28 - days) // if the current day is close to end of month
        {
            dd = dd + days - 28; // moving to the date in next month
            mm++;
        }
        else
        {
            dd = dd + days; // if not close to end month, the just adding the days
        }
    }
    else if (mm == 2 && yyyy % 4 == 0) // 29days----for the case of fen in leap year
    {
        if (dd > 29 - days) // similar to previous if confdition
        {
            dd = dd + days - 29;
            mm++;
        }
        else
        {
            dd = dd + days;
        }
    }
    else if (mm == 4 || mm == 6 || mm == 9 || mm == 11) // 30days----for months with 30days
    {
        if (dd > 30 - days)
        {
            dd = dd + days - 30;
            mm++;
        }
        else
        {
            dd = dd + days;
        }
    }
    else if (mm == 1 || mm == 3 || mm == 5 || mm == 7 || mm == 8 || mm == 10) // 31days----fro moths with 31 days
    {
        if (dd > 31 - days)
        {
            dd = dd + days - 31;
            mm++;
        }
        else
        {
            dd = dd + days;
        }
    }
    else if (mm == 12) // 31days and end of year--- for the case december
    {
        if (dd > 31 - days)
        {
            dd = dd + days - 31;
            mm = 1;
            yyyy++;
        }
        else
        {
            dd = dd + days;
        }
    }
}

void welcomeScreen() // defining a function welcomeScreen it prints the welcome screen
{
    printf("\n\n\n\n\n\t");
    for (i = 0; i < 120; i++)
    {
        printf("*"); // printing '*' symbol for line to separate
    }
    printf("\n\t");
    for (i = 0; i < 120; i++)
    {
        printf("*"); // printing '*' symbol for line to separate
    }
    printf("\n\t%62s\n\t%61s\n\t%62s\n\t%62s\n\t", "WELCOME", "TO THE", "LIBRARY", "CATALOG"); // Title of the program
    for (i = 0; i < 120; i++)
    {
        printf("*");
    }
    printf("\n\t");
    for (i = 0; i < 120; i++)
    {
        printf("*");
    }
    printf("\n\n\t%66s\n\t%68s\n\t%62s\n\t%65s\n\t%61s\n\n\t", "Enter the value", "1 for Search Book", "2 for Login", "3 for Register", "4 for Exit"); // printing 3 options for login, register and exit.
    for (i = 0; i < 120; i++)
    {
        printf("*");
    }
    printf("\n\tYour Choice : "); // asking for the choice, --- we take the choice in main function
}

void ReCopyBooks() // For re writing the editted data to original file
{
    BOOK bookC;
    FILE *BRead, *BWrite;

    BWrite = fopen(BooksPath, "w"); // clearing books file
    fclose(BWrite);

    BRead = fopen(BooksPath2, "r"); // copying from temp to book file
    while (fread(&bookC, sizeof(BOOK), 1, BRead))
    {
        BWrite = fopen(BooksPath, "a");
        fwrite(&bookC, sizeof(BOOK), 1, BWrite);
        fclose(BWrite);
    }
    fclose(BRead);

    BWrite = fopen(BooksPath2, "w"); // clearing temp file
    fclose(BWrite);
}

void ReCopyUsers() // For re writing the editted data to original file
{
    USER userC;
    FILE *URead, *UWrite;
    UWrite = fopen(UsersPath, "w"); // clearing users file
    fclose(UWrite);

    URead = fopen(UsersPath2, "r"); // copying from temp file to original file
    while (fread(&userC, sizeof(USER), 1, URead))
    {
        UWrite = fopen(UsersPath, "a");
        fwrite(&userC, sizeof(USER), 1, UWrite);
        fclose(UWrite);
    }
    fclose(URead);

    UWrite = fopen(UsersPath2, "w"); // clearing temp file for next use
    fclose(UWrite);
}

void ReCopyRequests()
{
    REQUEST Rr;
    FILE *RRead, *RWrite;
    RWrite = fopen(ReqPath, "w"); // clearing users file
    fclose(RWrite);

    RRead = fopen(ReqPath2, "r"); // copying from temp file to original file
    while (fread(&Rr, sizeof(REQUEST), 1, RRead))
    {
        RWrite = fopen(ReqPath, "a");
        fwrite(&Rr, sizeof(REQUEST), 1, RWrite);
        fclose(RWrite);
    }
    fclose(RRead);

    RWrite = fopen(ReqPath2, "w"); // clearing temp file for next use
    fclose(RWrite);
}

void Register() // defining a function for regestring new users
{
    USER newuser, user;
    int xo = 0, yo = 0, ko = 0, lo = 0;

    fp = fopen(UsersPath, "a"); // Opening a file name Userspath and appending it----This is to creat a new file if the file does not exists.
    fclose(fp);                 // close the filepointer using fclose() function

    printf("\n\n\tYou are in registration page.");
    printf("\n\tEnter Your NAME : "); // asking for name
    getchar();
    gets(newuser.name); // reads a string "newuser.name?
    do                  // A do while loop begins
    {
        printf("\n\tWho are you?"); // asking who is it
        printf("\n\tEnter 'F' if you are Faculty (or) 'S' if you are Student : ");
        scanf("%s", &aa);                                 // Scanning a string a const. char variable aa
        if (strcmp(aa, "F") == 0 || strcmp(aa, "S") == 0) // Comparing strings aa,"F" and aa,"S"
        {
            xo = 1; // proceeding for user registration
        }
        else if (strcmp(aa, "A") == 0) // Comparing Strings aa,"A" since ?A? is for Admin
        {
            printf("\n\tEnter the Developer Password to register as ADMIN\n\tDev. Password : ");
            scanf("%s", &bb);             // Scanning a string a const. Char variable bb
            if (strcmp(DevPass, bb) == 0) // Checking if Password matches with developer password, and if match, run the following code.
            {
                printf("\n\tAdmin registration is accessed.\n"); // proceeding for admin registration
                xo = 1;
            }
            else
            {
                xo = 0;
                ko++;
                printf("\n\t%s,You have Entered Wrong Password. Only Admin allowed. Please don't mess here.\n\n", newuser.name);
            }
        }
        else // if neither F S or A then ask for proper charecter
        {
            xo = 0;
            ko++;
            printf("\n\tPlease Enter proper Character. Try again. %d chances left\n\n", 3 - ko); // Must enter Either ?F? or ?S? or ?A?
            if (ko == 3)
            {
                printf("\n\tReached maximum tries.");
                exit(0);
            }
        }

    } while (xo == 0);

    // xo=0;
    // ID:
    while (yo == 0)
    {
        printf("\n\tEnter your ID/Roll : "); // asking for id
        scanf("%s", &bb);                    // Copying the roll number to ?bb?
        strcpy(newuser.id, aa);              // Copying ?aa? data(F/S/A to newuser.id
        strcat(newuser.id, bb);              // Joining 2 strings(i.e F/S/A + Rollnumber)
        yo = 1;

        fp = fopen(UsersPath, "r");               // Opening Userspath file in reading mode
        while (fread(&user, sizeof(USER), 1, fp)) // Storing fp data in Struct ?user? with size of USER
        {
            if (strcmp(user.id, newuser.id) == 0) // Comparing new ID with already existing ID and they match each other
            {
                printf("\n\tThe User ID/Roll already exists, Please try another. %d chances left\n\t Or contact Admin/Librarian\n", 2 - lo);
                yo = 0;
                // xo=0;
                lo++; // id exists so continue the loop
                if (lo == 3)
                {
                    printf("\n\tYou ran out of max chances.");
                    exit(0);
                }
                // fclose(fp);
                // goto ID;
            }
            else
            {
                yo = 1;
                // xo=1;
            }
        }
        fclose(fp); // Closing fp if
    }

    printf("\tEnter new password (less than 20 characters) : ");
    scanf("%s", newuser.password); // Copying the above New password into newuser.password string
    newuser.hold = 0;              // Defining a string where user holds number of books as null

    for (i = 0; i < 10; i++) // initialising all the data if new user with nothing/null
    {
        strcpy(newuser.b[i].name, "");
        strcpy(newuser.b[i].author, "");
        strcpy(newuser.b[i].id, "");
        newuser.b[i].available = 0;
        newuser.b[i].total = 0;
        newuser.b[i].DD = 0; // all due dates will sho 0/0/0000
        newuser.b[i].MM = 0;
        newuser.b[i].YYYY = 0;
        newuser.b[i].Force = 0; // initial force is 0.
    }

    fp = fopen(UsersPath, "a");                                                                                                           // Opening Userspath file and appending it
    printf("\n\tHELLO %s\n\tRemember Your User ID is ' %s '\n\tand Password is ' %s '.\n\t", newuser.name, newuser.id, newuser.password); // Showing them their userIDs and Password to them
    fwrite(&newuser, sizeof(USER), 1, fp);                                                                                                // Writing the newuser data from above into ?fp?
    fclose(fp);                                                                                                                           // Closing file pointer ?fp?

    for (i = 0; i < 120; i++)
    {
        printf("*");
    }
    printf("\n");
}

int Login() // defining a function for Login checking passwords
{
    int loginf = 0, no = 0, mo = 0, j = 0, k = 0; // Defining loginf as integer local Variable
    USER userL;                                   // declaring a USER struct as user
    FILE *LRead;
    LRead = fopen(UsersPath, "r"); // opening UsersPath file in reading mode
    if (LRead == NULL)             // If the file cant be opened
    {
        printf("\n\n\tThere are no users yet please register and be the First one.\n\tPress Enter key to register.");
        fclose(LRead); // Closing the read file if the above condition is satisfied
        Register();    // Calling out the register function to register again from the user
    }
    fclose(LRead); // Closing the read file if the above condition is not satisfied

    printf("\n\n\n\tYou are in Login Page.\n"); // printing dailog
    // Login:
    while (no == 0)
    {
        printf("\n\tEnter the User ID : "); // asking id
        scanf("%s", &uu);                   // Storing USer ID in ?uu?

        LRead = fopen(UsersPath, "r");                // opening UsersPath file in reading mode for cross checking id
        while (fread(&userL, sizeof(USER), 1, LRead)) // Storing read data in Struct ?user? with size of USER
        {
            if (strcmp(userL.id, uu) == 0) // Checking if user.id and uu strings match each other for logging IN
            {
                no = 1;
                // Password:
                while (mo == 0)
                {
                    printf("\tEnter Password : ");       // If userids match then we ask for Password
                    scanf("%s", &pp);                    // Storing Password in ?pp?
                    if (strcmp(pp, userL.password) == 0) // Checking if user.password and pp strings match each other and if they match
                    {
                        mo = 1;
                        loginf = 1;            // the person logs in successfully
                        strcpy(Log, userL.id); // Copying ?user.id? data to global char Log where it is A global Variable where all userIDs are stored
                        if (uu[0] == 'A')      // If the first character is ?A? then the user is admin
                        {
                            loginf = 2; // The person logs in successfully as an Admin
                        }
                    }
                    else
                    {
                        printf("\n\n\tWrong Password! Try again. %d more chances\n", 2 - j); // if the pass is wrong
                        j++;
                        if (j == 3)
                        {
                            printf("\tYou are out of chances.");
                            exit(0);
                        }
                        // goto Password;
                    }
                }
            }
        }
        fclose(LRead);

        if (strcmp(Log, "") == 0) // global char Log is initialised with "", if remains same after login attempt, it mean the id does not exists in database.
        {
            printf("\n\tThe User Does not exists. Please try again. %d more chances\n", 2 - k);
            k++;
            if (k == 3)
            {
                printf("\tYou are out of chances.");
                exit(0);
            }
            // goto Login;
        }
    }
    while (no == 0)
        ;

    return loginf; // function returns loginf value either 0,1 or 2.
}

void ForcePullBook(char Bid[10]) // This function Takes away all the specific book from users.
{
    BOOK bookF, bookR;
    USER userF, userR;
    int l;
    FILE *FR1, *FW2, *GR1, *GW2;

    printf("\n\tThe book %s is being pulled forcefully.", Bid);

    FR1 = fopen(UsersPath, "r"); // reading for required book in all users
    while (fread(&userF, sizeof(USER), 1, FR1))
    {
        for (l = 0; l < 5; l++) // Saving the removed book for show messages
        {
            if (strcmp(userF.b[l].id, Bid) == 0)
            {
                GR1 = fopen(BooksPath, "r");
                while (fread(&bookF, sizeof(BOOK), 1, GR1))
                {
                    if (strcmp(bookF.id, Bid) != 0)
                    {
                        GW2 = fopen(BooksPath2, "a");
                        fwrite(&bookF, sizeof(BOOK), 1, GW2);
                        fclose(GW2);
                    }
                    else
                    {
                        bookF.available++; // increasing for each time/ book as it is inside the while loop
                        strcpy(userF.b[l + 5].name, userF.b[l].name);
                        strcpy(userF.b[l + 5].id, userF.b[l].id);
                        strcpy(userF.b[l + 5].author, userF.b[l].author);
                        userF.b[l + 5].Force = 1;

                        strcpy(userF.b[l].name, ""); // removing the original slot
                        strcpy(userF.b[l].id, "");
                        strcpy(userF.b[l].author, "");
                        userF.b[l].DD = 0;
                        userF.b[l].MM = 0;
                        userF.b[l].YYYY = 0;
                        userF.hold--; // decreasing the hold number

                        GW2 = fopen(BooksPath2, "a");
                        fwrite(&bookF, sizeof(BOOK), 1, GW2); // appending book data after all changes
                        fclose(GW2);
                    }
                }
                fclose(GR1);
                ReCopyBooks(); // recopying books to original file
            }
        }
        FW2 = fopen(UsersPath2, "a"); // appending the user data after all changes
        fwrite(&userF, sizeof(USER), 1, FW2);
        fclose(FW2);
    }

    ReCopyUsers(); // recopying user data to original file

    printf("\n\tThe Book %s is force pulled successfully.\n\n", Bid);
}

void ClearForce(USER userC) // This function clears the forces and pulled books
{
    USER CC;
    int s;
    FILE *Red, *Wri;
    Red = fopen(UsersPath, "r"); // reading for user match
    while (fread(&CC, sizeof(USER), 1, Red))
    {
        if (strcmp(userC.id, CC.id) != 0) // not changing any unmatched id
        {
            Wri = fopen(UsersPath2, "a");
            fwrite(&CC, sizeof(USER), 1, Wri);
            fclose(Wri);
        }
        else
        {
            for (s = 5; s < 10; s++) // clearing the force when id matched.
            {
                strcpy(CC.b[s].name, "");
                strcpy(CC.b[s].author, "");
                strcpy(CC.b[s].id, "");
                CC.b[s].Force = 0;
            }
            Wri = fopen(UsersPath2, "a");
            fwrite(&CC, sizeof(USER), 1, Wri);
            fclose(Wri);
        }
    }
    fclose(Red);

    FB = 0;

    ReCopyUsers(); // recopying the user data to original file
}

int userPage(USER userP) // defining function for displaying the user details
{
    USER U;                                // declaring an USER U
    fp = fopen(UsersPath, "r");            // Opening UsersPath file in reading mode
    while (fread(&U, sizeof(USER), 1, fp)) // Reading the ?fp? and storing the data in ?U?
    {
        if (strcmp(userP.id, U.id) == 0) // Comparing strings userP.id of argument, U.id and they are true
        {
            strcpy(userP.name, U.name); // Copies data in U's data to userP's
            strcpy(userP.id, U.id);
            strcpy(userP.password, U.password);
            userP.hold = U.hold;
            for (i = 0; i < 10; i++) // printing all the books
            {
                strcpy(userP.b[i].name, U.b[i].name);
                strcpy(userP.b[i].author, U.b[i].author);
                strcpy(userP.b[i].id, U.b[i].id);
                userP.b[i].DD = U.b[i].DD;
                userP.b[i].MM = U.b[i].MM;
                userP.b[i].YYYY = U.b[i].YYYY;
                userP.b[i].Force = U.b[i].Force;
            }
            break; // breaking loop after we found the required user
        }
    }
    fclose(fp); // and closing the file after copying

    printf("\n\t"); // printing the above copied user?s data on user page on console
    for (i = 0; i < 120; i++)
    {
        printf("*");
    }
    printf("\n\tWelcome %s, This is your Page.\n\tYou have %d Books with you.\n\t", userP.name, userP.hold); // Letting them to know that so and so books are with the logged in user
    for (i = 0; i <= 120; ++i)
    {
        printf("_");
    }
    printf("\n\t%2s  %-50s %-20s %-9s %-11s\n\t", "No", "Book Name", "Author", "Book Id", "Due Date");
    for (i = 0; i <= 120; ++i)
    {
        printf("-");
    }
    for (i = 0; i < 5; i++)
    {
        printf("\n\t%2d  %-50s %-20s %-9s %02d/%02d/%04d", i + 1, userP.b[i].name, userP.b[i].author, userP.b[i].id, userP.b[i].DD, userP.b[i].MM, userP.b[i].YYYY); // printing user name and his books,author name, book ID, User ID,Due date
        if (userP.id[0] == 'S' && i == 2)                                                                                                                            // If the user is a student then the loop gets broken when the number of books reaches 3
        {
            break; // the 4 and 5 book slot will load only if it is faculty
        }
    }
    printf("\n\t");
    for (i = 0; i <= 120; ++i)
    {
        printf("_");
    }

    FB = 0; // checking Forces if any
    for (i = 5; i < 10; i++)
    {
        if (userP.b[i].Force == 1) // priinting for each force
        {
            printf("\n\t*NOTE: The Book '%s' with ID '%s' is pulled forcefully by admin for the sake of Updating.", userP.b[i].name, userP.b[i].id);
            FB = 1;
        }
    }
    if (FB == 1) // If forces print messages
    {
        printf("\n\tPlease Borrow Book again if needed.\n\t(The Above NOTE will be removed, at next view.)\n");
    }

    Date();                 // reading the current date
    OD = 0;                 // initialising Overdue books are NIL
    for (i = 0; i < 5; i++) // looping all  the books
    {
        if (userP.b[i].YYYY != 0 && userP.b[i].YYYY - yyyy < 0) // checking year overdue
        {
            OD = 1; // Overdue Books are present
            break;  // breaking because if overdue is for 1 book, we can call it as overdue.
        }
        else if (userP.b[i].YYYY != 0 && userP.b[i].YYYY - yyyy == 0) // if same year checking month due
        {
            if (userP.b[i].MM != 0 && userP.b[i].MM - mm < 0)
            {
                OD = 1;
                break;
            }
            else if (userP.b[i].MM != 0 && userP.b[i].MM - mm == 0) // if same month checking day due
            {
                if (userP.b[i].DD != 0 && userP.b[i].DD - dd < 0)
                {
                    OD = 1;
                    break;
                }
            }
        }
    }

    if (OD == 1) // if there is an overdue the print Note
    {
        printf("\n\n\t*NOTE: YOU HAVE BOOKS WITH OVERDUE, PLEASE RETURN THE OVERDUE BOOKS IMMEDIATELY.\n");
    }

    return userP.hold; // returning the number of books the user hold
}

void Catalog() // defining a function for displaying the books in catalog
{
    printf("\n\tCATALOG\n\t");
    for (i = 0; i <= 120; ++i)
    {
        printf("_");
    }
    printf("\n\t|%4s | %-58s | %-20s| %-9s| %10s/%6s |\n\t", "No.", "Book Name", "Author", "ID", "Available", "Total"); // Printing the headings in a catalogue table i.e serial number,book name, author,ID availability,Totalcopies
    for (i = 0; i <= 120; ++i)
    {
        printf("-");
    }

    BOOK book; // declaring a BOOK book

    fp = fopen(BooksPath, "r"); // Opening Bookspath file in reading mode
    if (fp == NULL)             // If the number of books are zero with the user then the below print function is printed
    {
        printf("\n\n\tThere are no Books in the Catalog. Ask the Admin/Librarian to Add Some.\n\n\t");
    }
    i = 0;                                    // initiallising i to 0
    while (fread(&book, sizeof(BOOK), 1, fp)) // Reading the data from fp and storing it in book
    {
        i++;                                                                                                                   // beging with i=1 and incrementing each time
        printf("\n\t|%4d | %-58s | %-20s| %-9s| %10d/%-6d |", i, book.name, book.author, book.id, book.available, book.total); // printing number i++ and book details
    }
    fclose(fp); // Closing the fp file after completing by printing all books in catalogue

    printf("\n\t");
    for (i = 0; i <= 120; ++i)
    {
        printf("_");
    }
    printf("\n\t");
}

int BookIDChk(char IDChk[10]) // This fuction check if that book id exists
{
    int check = 0;
    FILE *BBBR;
    BOOK Bchek;
    BBBR = fopen(BooksPath, "r");
    while (fread(&Bchek, sizeof(BOOK), 1, BBBR))
    {
        if (strcmp(Bchek.id, IDChk) == 0) // if exits, then check is returned as 1
        {
            check = 1;
        }
    }
    return check;
}

void AddBook() // This is for adding books to catalogue
{
    FILE *fpp;

    fpp = fopen(BooksPath, "a"); // Opening Bookspath file in appending it whether books are present or not
    fclose(fpp);

    int bbo = 0, cco = 0;
    BOOK addBook, book; // Defining addbook and book variables

BookName:                                                 // naming line
                                                          //  do
                                                          //  {
    printf("\n\tEnter the Book Name (50 char. max.) : "); // Asking for Book name
    getchar();
    gets(addBook.name); // Waits for the user to enter book name and press enter

    fpp = fopen(BooksPath, "r");               // Opening Bookspath file in reading mode
    while (fread(&book, sizeof(BOOK), 1, fpp)) // reading the data from ?fp? and storing it in ?book?
    {
        if (strcmp(book.name, addBook.name) == 0) // Checking the user?s entered book name and catalogue?s book name and they are matching
        {
            bbo = 0;
            printf("\n\tThe Book Name already exists, Try again.\n\n");
            fclose(fpp);
            goto BookName;
            break;
        }
        // else
        // {
        //     bbo=1;
        // }
    }
    fclose(fpp);
    // }while(bbo==0);

    printf("\tEnter The Author name (20 char. max.) : ");
    // getchar();
    gets(addBook.author); // Waiting for the user to type author name and press enter

BookID: // naming line
        //  do
        //  {
    printf("\tEnter a ID for Book (Ex: P001) : ");
    // getchar();
    gets(addBook.id); // Waiting for the user to type Book ID and press enter

    fpp = fopen(BooksPath, "r");               // Opening Bookspath file in reading mode
    while (fread(&book, sizeof(BOOK), 1, fpp)) // reading the data from ?fp? and storing it in ?book?
    {
        if (strcmp(addBook.id, book.id) == 0) // Checking the user?s entered book ID and catalogue?s book ID and they are matching
        {
            printf("\n\tThe Book ID is Taken, Try another.\n\n");
            cco = 0;
            fclose(fpp);
            goto BookID;
            break;
        }
        // else
        // {
        //     cco=1;
        // }
    }
    fclose(fpp);
    // }while(cco==0);

    printf("\tEnter total number of copies : ");
    scanf("%d", &addBook.total); // Storing the total number of copies to String addBook.total
    addBook.available = addBook.total;
    addBook.DD = addBook.MM = addBook.YYYY = 0;

    fpp = fopen(BooksPath, "a");            // Opening Bookspath file in reading mode
    fwrite(&addBook, sizeof(BOOK), 1, fpp); // writing books details in fp
    fclose(fpp);

    Catalog();
}

void UsersList() // Admin having control to check users list
{
    printf("\n\t");
    for (i = 0; i <= 120; ++i)
    {
        printf("_");
    }
    printf("\n\t|%4s|%-20s|%-20s|%-16s|%-5s|%-9s|%-9s|%-9s|%-9s|%-9s|\n\t", "No.", "User Name", "User ID", "Student/Faculty", "Hold", "Book1", "Book2", "Book3", "Book4", "Book5"); // printing headings of users table
    for (i = 0; i <= 120; ++i)
    {
        printf("-");
    }

    USER user; // Declaring a USER user

    fptr = fopen(UsersPath, "r"); // opening UsersPath file in reading mode
    if (fptr == NULL)             // If the file pointer fptr is empty(i.e NULL) that mean no book is added till now
    {
        printf("\n\n\tThere are No Users. Ask Students and Faculty to get registered.\n");
    }
    i = 0;                                      // initialising i as 0
    while (fread(&user, sizeof(USER), 1, fptr)) // Reading the data from fptr and storing it in user
    {
        i++;                                                                                                                                                                                     // increasing i value so that it start printing from 1
        printf("\n\t|%4d|%-20s|%-20s|%-16c|%-5d|%-9s|%-9s|%-9s|%-9s|%-9s|", i, user.name, user.id, user.id[0], user.hold, user.b[0].id, user.b[1].id, user.b[2].id, user.b[3].id, user.b[4].id); // Printing User?s name and their data to admin
    }
    fclose(fptr); // Closing file
    printf("\n\t");
    for (i = 0; i <= 120; ++i)
    {
        printf("_");
    }
}

void RemoveBook() // This function Remove book from catalog
{
    Catalog(); // showing the catalog to book ids
    BOOK book; // declaring BOOK book
    char ot;
    printf("\n\tPlease ensure that all copies of book are available before removing.");
    printf("\n\tEnter the Book ID you want to REMOVE : "); // asking id of book you want to remove
    scanf("%s", &reID);                                    // storing that id in reID

    fp = fopen(BooksPath, "r"); // opening books path
    while (fread(&book, sizeof(BOOK), 1, fp))
    {
        if (strcmp(reID, book.id) == 0) // if book ID not match reid , then copying to temp file
        {
            if (book.available != book.total) // if total books are available then copying to include, so that it does not removed
            {
                printf("\n\tTotal Books are not available. Ask the users to return all copies and then remove.\n");
                printf("\n\n\n\tHowever, you can force remove the book.\n\tBut, doing so The book at users end will be removed also.\n\tDo you want to FORCE REMOVE BOOK?(y/n)\n\tYour choice : ");
                scanf("%s", &ot);
                if (ot == 'y')
                {
                    ForcePullBook(book.id);
                }
            }
            break;
        }
    }
    fclose(fp); // closing books file

    fp = fopen(BooksPath, "r");               // opening books path
    while (fread(&book, sizeof(BOOK), 1, fp)) // reading each book and writing in BOOK book
    {
        if (strcmp(reID, book.id) != 0) // if book ID not match reID , then copying to temp file
        {
            fptr = fopen(BooksPath2, "a");
            fwrite(&book, sizeof(BOOK), 1, fptr);
            fclose(fptr);
        }
        else // if book id match reID the making changes
        {
            if (book.available != book.total) // if total books are available then copying to include, so that it does not removed
            {
                printf("\n\tTotal Books are still not available. Please try again.\n");
                fptr = fopen(BooksPath2, "a");
                fwrite(&book, sizeof(BOOK), 1, fptr);
                fclose(fptr);
            }
            else // if all copies available then excluding to copy to temp path
            {
                printf("\n\tThe Book '%s' with book id '%s' is Removed.\n", book.name, book.id);
            }
        }
    }
    fclose(fp); // closing books file

    ReCopyBooks();

    // Catalog(); //already called in main function so commented.
}

void BorrowBook(USER userB) // defining function for borrowing book from catalog
{
    Catalog(); // Showing catalog for user
    USER U;    // declaring USER U
    BOOK B;    // declaring BOOK B

    Date();                       // reading current date
    fptr = fopen(UsersPath, "r"); // opening users file to read
    while (fread(&U, sizeof(USER), 1, fptr))
    {
        if (strcmp(U.id, userB.id) != 0) // if id doesnot match with id in argument then copy all to temp file
        {
            fptr2 = fopen(UsersPath2, "a");
            fwrite(&U, sizeof(USER), 1, fptr2);
            fclose(fptr2);
        }
        else // if id's are matching ask for book id to borrow
        {
            printf("Enter the Book ID to Borrow : ");
            scanf("%s", &aa);           // storing book id in aa
            fp = fopen(BooksPath, "r"); // opening books file to read
            while (fread(&B, sizeof(BOOK), 1, fp))
            {
                if (strcmp(B.id, aa) != 0) // if book id not match with requested book then copy to temp
                {
                    fp2 = fopen(BooksPath2, "a");
                    fwrite(&B, sizeof(BOOK), 1, fp2);
                    fclose(fp2);
                }
                else // if book id's not match the make changes
                {
                    if (U.id[0] == 'F' && B.available > 0) // faculty can take books with min availability 1
                    {
                        B.available--; // reduscing available by 1
                        for (i = 0; i < 5; i++)
                        {
                            if (strcmp(U.b[i].name, "") == 0) // copying to the first encountered empty slot in yser data
                            {
                                strcpy(U.b[i].name, B.name);
                                strcpy(U.b[i].author, B.author);
                                strcpy(U.b[i].id, B.id);
                                Date();         // reading current date
                                DatePlus(20);   // increase date by due time
                                U.b[i].DD = dd; // copying the due date
                                U.b[i].MM = mm;
                                U.b[i].YYYY = yyyy;
                                U.hold++;
                                Date(); // reading back the current date
                                break;
                            }
                        }
                        fp2 = fopen(BooksPath2, "a"); // copying after changes made to temp
                        fwrite(&B, sizeof(BOOK), 1, fp2);
                        fclose(fp2);
                    }
                    else if (U.id[0] == 'S' && B.available > 3) // student can take books with min availability 4
                    {
                        B.available--; // same as done in faculty, making changes and copying
                        for (i = 0; i < 5; i++)
                        {
                            if (strcmp(U.b[i].name, "") == 0)
                            {
                                strcpy(U.b[i].name, B.name);
                                strcpy(U.b[i].author, B.author);
                                strcpy(U.b[i].id, B.id);
                                Date();
                                DatePlus(10);
                                U.b[i].DD = dd;
                                U.b[i].MM = mm;
                                U.b[i].YYYY = yyyy;
                                U.hold++;
                                Date();
                                break;
                            }
                        }
                        fp2 = fopen(BooksPath2, "a"); // copying
                        fwrite(&B, sizeof(BOOK), 1, fp2);
                        fclose(fp2);
                    }
                    else if (U.id[0] == 'F' && B.available <= 0) // printing exceptional dailogs
                    {
                        printf("\n\tThere are no copies of the Book you ask. Please come back and check.");
                        fp2 = fopen(BooksPath2, "a"); // copying without changes
                        fwrite(&B, sizeof(BOOK), 1, fp2);
                        fclose(fp2);
                    }
                    else if (U.id[0] == 'S' && B.available <= 3) // printing exceptional dailogs
                    {
                        printf("\n\tSorry...\n\tThe available Copies of the Book you ask are less than or equal to 3.\n\tThese Books are reserved only for faculty.\n\tPlease come back later and check.");
                        fp2 = fopen(BooksPath2, "a"); // copying without changes
                        fwrite(&B, sizeof(BOOK), 1, fp2);
                        fclose(fp2);
                    }
                }
            }
            fclose(fp);

            fptr2 = fopen(UsersPath2, "a"); // copying user data after changes
            fwrite(&U, sizeof(USER), 1, fptr2);
            fclose(fptr2);
        }
    }
    fclose(fptr);

    ReCopyUsers();

    ReCopyBooks();

    Date(); // re reading current date
}

void RemoveUser() // defining a function for removing a user, Only admin can access
{
    UsersList(); // Showing list of users
    USER user;   // declaring USER user
    X = 0;
    do
    {
        printf("\n\tMake sure the User does not hold any Books.\n\tEnter User ID to REMOVE : ");
        scanf("%s", &reID); // asking for id to remove
        X = 1;
        if (strcmp(reID, Log) == 0) // if he is removing his own accout then warn him
        {
            printf("\n\tAre you sure you want to delete your Account (y/n)\n\tYou will be Logged Out if you remove your own account.\n\tYour Choice : ");
            scanf("%s", &aa);
            if (aa[0] == 'n') // while loop will run
            {
                X = 0;
            }
        }
    } while (X == 0);

    fp = fopen(UsersPath, "r"); // opening users file for reading
    while (fread(&user, sizeof(USER), 1, fp))
    {
        if (strcmp(reID, user.id) != 0) // if id not match then copy all to temp
        {
            fptr = fopen(UsersPath2, "a");
            fwrite(&user, sizeof(USER), 1, fptr);
            fclose(fptr);
        }
        else // if id match the
        {
            if (user.hold != 0) // check that he have any books with him and dont remove
            {
                printf("\n\tThe User Hold Some Books. Please ask user to return all books and then remove.\n");
                fptr = fopen(UsersPath2, "a");
                fwrite(&user, sizeof(USER), 1, fptr);
                fclose(fptr);
            }
            else // if there are no books then remove by excluding to copy
            {
                printf("\n\tThe User '%s' with user id '%s' is Removed.\n", user.name, user.id);
            }
        }
    }
    fclose(fp);

    ReCopyUsers();

    if (strcmp(reID, Log) == 0) // if he removed his own account then logout and exit
    {
        printf("\n\tYour account is removed.");
        exit(0);
    }
    else // else show users list
    {
        UsersList();
    }
}

void ReturnBook(USER userR) // Defining a function for returning a book to catalog
{
    userPage(userR); // showing user page
    USER U;
    BOOK B;

    fptr = fopen(UsersPath, "r"); // opening user file for reading
    while (fread(&U, sizeof(USER), 1, fptr))
    {
        if (strcmp(U.id, userR.id) != 0) // if userid not match with id in argument then copy to temp
        {
            fptr2 = fopen(UsersPath2, "a");
            fwrite(&U, sizeof(USER), 1, fptr2);
            fclose(fptr2);
        }
        else // if match then
        {
            printf("\n\tEnter The BOOK ID to RETURN : "); // ask the book id to return
            scanf("%s", &aa);                             // store id in aa
            fp = fopen(BooksPath, "r");                   // open and read books file
            while (fread(&B, sizeof(BOOK), 1, fp))
            {
                if (strcmp(B.id, aa) != 0) // if book id not match then copy to temp
                {
                    fp2 = fopen(BooksPath2, "a");
                    fwrite(&B, sizeof(BOOK), 1, fp2);
                    fclose(fp2);
                }
                else // if match make changes
                {
                    B.available++;          // increase book available by 1
                    for (i = 0; i < 5; i++) // clear the slot that contain this book
                    {
                        if (strcmp(U.b[i].id, aa) == 0)
                        {
                            strcpy(U.b[i].name, "");
                            strcpy(U.b[i].author, "");
                            strcpy(U.b[i].id, "");
                            U.b[i].DD = 0; // rest due date to 0
                            U.b[i].MM = 0;
                            U.b[i].YYYY = 0;
                            U.hold--; // decrease hold by 1
                            break;
                        }
                    }
                    fp2 = fopen(BooksPath2, "a"); // copy to temp after changes
                    fwrite(&B, sizeof(BOOK), 1, fp2);
                    fclose(fp2);
                }
            }
            fclose(fp);

            fptr2 = fopen(UsersPath2, "a"); // copy t temp after changes
            fwrite(&U, sizeof(USER), 1, fptr2);
            fclose(fptr2);
        }
    }
    fclose(fptr); // closing file

    ReCopyUsers();

    ReCopyBooks();
}

void FillEmptyBookSlot()
{
    USER Uu;
    fptr = fopen(UsersPath, "r");
    while (fread(&Uu, sizeof(USER), 1, fptr))
    {
        for (i = 0; i < 4; i++)
        {
            if (strcmp(Uu.b[i].name, "") == 0)
            {
                strcpy(Uu.b[i].name, Uu.b[i + 1].name);
                strcpy(Uu.b[i + 1].name, "");
                strcpy(Uu.b[i].author, Uu.b[i + 1].author);
                strcpy(Uu.b[i + 1].author, "");
                strcpy(Uu.b[i].id, Uu.b[i + 1].id);
                strcpy(Uu.b[i + 1].id, "");
                Uu.b[i].DD = Uu.b[i + 1].DD; // rest due date to 0
                Uu.b[i + 1].DD = 0;          // rest due date to 0
                Uu.b[i].MM = Uu.b[+1].MM;
                Uu.b[i + 1].MM = 0;
                Uu.b[i].YYYY = Uu.b[i + 1].YYYY;
                Uu.b[i + 1].YYYY = 0;
            }
        }
        fptr2 = fopen(UsersPath2, "a");
        fwrite(&Uu, sizeof(USER), 1, fptr2);
        fclose(fptr2);
    }
    fclose(fptr);
    ReCopyUsers();
}

void Credits() // This a simple function that prints credits
{
    printf("\n\t");
    for (i = 0; i < 120; i++)
    {
        printf("*");
    }
    printf("\n\t%63s\n\n\n", "Thank You!!");
    printf("%67s\n\n", "CREDITS");
    printf("%63s %6s\n\n\t", "VENKATESH", "18MS014");
    for (i = 0; i < 120; i++)
    {
        printf("*");
    }
}

/* void SetDate() //Defining a function for admin to set the current date
{
    X=0;
    do //starting a while loop while X=0
    {
        X=1; //changing X to 1
        printf("\n\tEnter proper Date in format dd/mm/yyyy respectively\n\tdd : ");
        scanf("%d", &dd); //Asking and storing date in dd,mm,yyyy
        printf("\tmm : ");
        scanf("%d", &mm);
        printf("\tyyyy : ");
        scanf("%d", &yyyy);
        //checking if any error in date, as of in a real calender
        if(mm==2 && yyyy%4!=0) //28days
        {
            if(dd>28 || dd<1)
            {
                printf("\n\tThere is error in Date. try again.");
                X=0;
            }
        }
        else if(mm==2 && yyyy%4==0) //29days
        {
            if(dd>29 || dd<1)
            {
                printf("\n\tThere is error in Date. try again.");
                X=0;
            }
        }
        else if(mm==4 || mm==6 || mm==9 || mm==11) //30days
        {
            if(dd>30 || dd<1)
            {
                printf("\n\tThere is error in Date. try again.");
                X=0;
            }
        }
        else if(mm==1 || mm==3 || mm==5 || mm==7 || mm==8 || mm==10 || mm==12) //31days
        {
            if(dd>31 || dd<1)
            {
                printf("\n\tThere is error in Date. try again.");
                X=0;
            }
        }
        if(mm>12 || mm<1)
        {
            printf("\n\tThere is error in Month. try again.");
            X=0;
        }
        else if(yyyy<2020 || yyyy>2999) //only these years are allowed for now
        {
            printf("\n\tThere is error in Year.\n\tYear can not be before 2020 or exceed 2999. Try again.");
            X=0;
        }
    } while (X==0); //while loop
    fp = fopen(DatePath,"w"); //overwriting date inn date path
    fprintf(fp, "%d %d %d", dd, mm, yyyy);
    fclose(fp);
    Date(); //reading the current date
    printf("\n\tDate Set to %d/%d/%d",dd,mm,yyyy); //printing date changed.
} */

void ChangePassword(USER userC) // Defining a function for changing Password
{
    USER U;

    fptr = fopen(UsersPath, "r"); // opening user file for reading
    while (fread(&U, sizeof(USER), 1, fptr))
    {
        if (strcmp(U.id, userC.id) != 0) // copy all users to temp file if id doesn't match
        {
            fptr2 = fopen(UsersPath2, "a");
            fwrite(&U, sizeof(USER), 1, fptr2);
            fclose(fptr2);
        }
        else // if id match then making changes
        {
            printf("\n\tEnter Present Password : "); // asking for old password
            scanf("%s", &pp);
            if (strcmp(pp, U.password) == 0) // if old pass is correct the ask for new pass and make changes
            {
                printf("\n\tEnter New Password : ");
                scanf("%s", &pp);
                strcpy(U.password, pp);
                printf("\n\n\tYour Password changed.");
            }
            else // print old pass is wrong
            {
                printf("\n\tWrong Password. Please try again.");
            }
            fptr2 = fopen(UsersPath2, "a"); // copying changes after making changes
            fwrite(&U, sizeof(USER), 1, fptr2);
            fclose(fptr2);
        }
    }
    fclose(fptr); // close file

    ReCopyUsers();
}

char LowerString(char TheString[51]) // This function lowers all char in string
{
    int j;
    for (j = 0; j < 51; j++)
    {
        TheString[j] = tolower(TheString[j]);
    }
}

void Search() // This is a free search fuction for Book name and author
{
    char sea[40], Name[50], Author[20];
    char *P, *Q;
    BOOK S;
    int v = 0;
    printf("\n\tSEARCH\n\tEnter the Book Name or Author Name : ");
    getchar();
    gets(sea);
    LowerString(sea); // lowering for making no case sensitivity
    printf("\n\tSEARCH Results are...");
    printf("\n\t");
    for (i = 0; i <= 120; ++i)
    {
        printf("_");
    }
    printf("\n\t|%4s | %-58s | %-20s| %-9s| %10s/%6s |\n\t", "No.", "Book Name", "Author", "ID", "Available", "Total"); // Printing the headings in a catalogue table i.e serial number,book name, author,ID availability,Totalcopies
    for (i = 0; i <= 120; ++i)
    {
        printf("-");
    }
    i = 0;
    fp = fopen(BooksPath, "r");
    while (fread(&S, sizeof(BOOK), 1, fp))
    {
        strcpy(Name, S.name);
        strcpy(Author, S.author);
        LowerString(Name); // lowering to make not case sensitive
        LowerString(Author);
        P = strstr(Name, sea);
        Q = strstr(Author, sea);
        if (P || Q)
        {
            i++;
            v = 1;
            printf("\n\t|%4d | %-58s | %-20s| %-9s| %10d/%-6d |", i, S.name, S.author, S.id, S.available, S.total); // printing number i++ and book details
        }
    }
    fclose(fp);

    if (v == 0)
    {
        printf("\n\tSearch Ended. There is no match, try another search.");
    }

    printf("\n\t");
    for (i = 0; i <= 120; ++i)
    {
        printf("_");
    }
}

void Request(USER userM) // Defining a function to collect requests from users
{
    USER U;
    REQUEST R;
    fp = fopen(UsersPath, "r"); // opening users file for reading
    while (fread(&U, sizeof(USER), 1, fp))
    {
        if (strcmp(userM.id, U.id) == 0) // if the id match the collect user data
        {
            Date();                     // reading curren date
            strcpy(R.fromName, U.name); // reading name, id
            strcpy(R.fromID, U.id);
            printf("\n\tWrite your Message/Request.\n\tPlease be polite.(Press Enter key to send the Message.)\n\n\n\tYour Message(150 char): "); // asking for his message
            getchar();
            gets(R.message); // storing msg
            R.revDD = dd;    // storing current date
            R.revMM = mm;
            R.revYYYY = yyyy;

            fptr = fopen(ReqPath, "a"); // writing all the data in requests file
            fwrite(&R, sizeof(REQUEST), 1, fptr);
            fclose(fptr);

            printf("\n\tRequest Sent");
        }
    }
    fclose(fp); // closing file
}

void DeleteRequest()
{
    FILE *Gp, *Gpt;
    REQUEST DR;
    int Cho = 0, Ind, Ii = 0;
    printf("\n\t1:Delete All\t2:Delete n'th message\t(or any other number to continue)\n\tYour Choice : ");
    scanf("%d", &Cho);
    if (Cho == 1)
    {
        Gp = fopen(ReqPath, "w");
        fclose(Gp);
    }
    else if (Cho == 2)
    {
        printf("\n\tEnter the index of message to DELETE it : ");
        scanf("%d", &Ind);
        Gpt = fopen(ReqPath, "r");
        Ii = 0;
        while (fread(&DR, sizeof(REQUEST), 1, Gpt))
        {
            Ii++;
            if (Ii != Ind)
            {
                Gp = fopen(ReqPath2, "a");
                fwrite(&DR, sizeof(REQUEST), 1, Gp);
                fclose(Gp);
            }
        }
        fclose(Gpt);
        ReCopyRequests();
    }
}

void ReadRequest() // Defining a function to read requests
{
    REQUEST RR;
    int go = 0;
    fptr = fopen(ReqPath, "r"); // opening requests file and reading them
    fseek(fptr, 0, SEEK_END);
    if (fptr == NULL || ftell(fptr) == 0) // If the file cant be opened
    {
        printf("\n\n\tThere are no Requests yet.\n");
        fclose(fptr); // Closing the fptr file if the above condition is satisfied
    }
    else
    {
        fseek(fptr, 0, SEEK_SET);
        i = 0;
        while (fread(&RR, sizeof(REQUEST), 1, fptr))
        {
            i++;
            printf("\n\t%d\n\tFrom: %s\t\tid: %s\tDate: %d/%d/%d\n\tMessage: %s\n\n", i, RR.fromName, RR.fromID, RR.revDD, RR.revMM, RR.revYYYY, RR.message); // printing all the requests
        }
        fclose(fptr); // Closing the fptr file if the above condition is satisfied
        DeleteRequest();
    }
}

void UpdateBook()
{
    Catalog();
    BOOK bookU;
    char Yes;
    printf("\n\tEnter the ID you want to Update : ");
    scanf("%s", &mid);

    int val = 0;
    fp = fopen(BooksPath, "r");
    while (fread(&bookU, sizeof(BOOK), 1, fp))
    {
        if (strcmp(mid, bookU.id) == 0) // if book ID not match mid , then copying to temp file
        {
            if (bookU.available != bookU.total) // if total books are available then copying to include, so that it does not removed
            {
                printf("\n\tTotal Books are not available.\n\tWe suggest to ask the users to return the book and then update it.\n\n\tThe book Shouldn't be updated.\n\tAlthough you can force update it.\n\tIf the book is force updated,\n\tThe book will be pulled away from users that have this book.\n\tDo you want to force update? (y/n)\n\tYour choice : ");
                scanf("%s", &Yes);
                if (Yes == 'y')
                {
                    ForcePullBook(bookU.id);
                }
            }
            break;
        }
    }
    fclose(fp); // closing books file

    fp = fopen(BooksPath, "r");
    while (fread(&bookU, sizeof(BOOK), 1, fp))
    {
        if (strcmp(mid, bookU.id) != 0) // if book ID not match mid , then copying to temp file
        {
            fptr = fopen(BooksPath2, "a");
            fwrite(&bookU, sizeof(BOOK), 1, fptr);
            fclose(fptr);
        }
        else // if book id match reID the making changes
        {
            if (bookU.available != bookU.total) // if total books are available then copying to include, so that it does not removed
            {
                printf("\n\tThe total copies are still not available.\n\tPlease try again.\n");
            }
            else // if all copies available
            {
                printf("\n\tBook present details...");
                printf("\n\t%-23s:%s\n\t%-23s:%s\n\t%-23s:%s\n\t%-23s:%d/%d", "Book Name", bookU.name, "Book ID", bookU.id, "Book Author", bookU.author, "available/Total copies", bookU.available, bookU.total);
                printf("\n\n\tEnter value\n\t1:Change Book Name\t2:Change Book Id\t3:Change Author Name\t4:Change Total Copies\t5:Change Full Book Details\n\tYour Choice : ");
                scanf("%d", &val);
                if (val == 1) // field wise update
                {
                    printf("\n\tEnter New Update Book Name : ");
                    getchar();
                    gets(bookU.name);
                    printf("\n\tUpdates Saved.");
                }
                else if (val == 2)
                {
                    do
                    {
                        printf("\n\tEnter New Update Book Id : ");
                        getchar();
                        gets(bookU.id);
                        if (BookIDChk(bookU.id) == 1)
                        {
                            printf("\n\tThe Book Id Already exists, Try another.");
                        }
                    } while (BookIDChk(bookU.id) == 1);
                    printf("\n\tUpdates Saved.Press Enter to go back.");
                    clearScreen();
                }
                else if (val == 3)
                {
                    printf("\n\tEnter New Update Author Name : ");
                    getchar();
                    gets(bookU.author);
                    printf("\n\tUpdates Saved.");
                }
                else if (val == 4)
                {
                    printf("\n\tEnter New Update Total copies : ");
                    scanf("%d", &bookU.total);
                    bookU.available = bookU.total;
                    printf("\n\tUpdates Saved.");
                }
                else if (val == 5)
                {
                    printf("\n\tEnter New Update Book Name : ");
                    getchar();
                    gets(bookU.name);
                    printf("\tEnter New Update Book Id : ");
                    gets(bookU.id);
                    printf("\tEnter New Update Author Name : ");
                    gets(bookU.author);
                    printf("\tEnter New Update Total copies : ");
                    scanf("%d", &bookU.total);
                    bookU.available = bookU.total;
                    printf("\n\tUpdates Saved.");
                }
                else
                {
                    printf("\n\tNO Changes are made.Please try again");
                }
            }
            fptr = fopen(BooksPath2, "a");
            fwrite(&bookU, sizeof(BOOK), 1, fptr);
            fclose(fptr);
        }
    }
    fclose(fp);

    ReCopyBooks();

    printf("\n");
    Catalog();
    printf("\n\tPress Enter to go next...");
    clearScreen();
}

void main()
{
    int bo = 0, co = 0, eo = 0;
    do
    {
        welcomeScreen(); // Printing welcome screen
        printf("\t");
        scanf("%d", &a); // collecting the choice
        if (a == 1)      // if choice is login
        {
            Search();
            printf("\n\tPress Enter to go next...\n\t");
            getchar();
            system("cls");
            clearScreen();
        }
        else if (a == 2)
        {
            login = Login(); // storing the return value fo login function in login integer
            system("cls");
            clearScreen();
        }
        else if (a == 3) // if choice is regester
        {
            Register(); // calling for registration
            printf("\n\tPress Enter to go next...\n\t");
            getchar();
            getchar();
            system("cls");
            clearScreen();
        }
        else if (a == 4) // choice is to exit
        {
            Credits(); // print credits and exit
            getchar();
            getchar();
            exit(0);
        }
        else // ask to print proper value
        {
            printf("\n\tWorng Entry. Try Again.\n\tPress Enter to go back...\n\t");
            getchar();
            getchar();
            system("cls");
            clearScreen();
            clearScreen();
        }

        if (login == 1) // if user has logged in
        {
            strcpy(userOnline.id, Log); // copy the online user id to log
            co = 0;
            do
            {
                Date();                                 // reading current date
                userOnline.hold = userPage(userOnline); // storing return of user page to useronline hold.  This also prints userpage
                Date();                                 // Reading current date
                printf("\n\tDate: %02d/%02d/%d\n", dd, mm, yyyy);
                printf("\n\tEnter Respective Value\n"); // giving all available options for user
                printf("\n\t%-20s\t%-20s\t%-20s\t%-20s\n\t%-20s\t%-20s\t%-20s\n", "1:Borrow Book", "2:Return Book", "3:View Catalog", "4:Search", "5:Change Password", "6:Request Book", "7:Logout");
                printf("\n\tYour Choice : ");
                scanf("%d", &b); // storing choice
                if (b == 1)      // if choice to borrow
                {
                    system("cls");
                    clearScreen();
                    if (userOnline.id[0] == 'F') // check if user is faculty
                    {
                        if (userOnline.hold < 5) // faculty can take max of 5 books
                        {
                            BorrowBook(userOnline); // if less than 5 the let borrow
                        }
                        else // if 5 the say he cant borrow
                        {
                            printf("\n\tYou can have maximum of 5 Books. Please Return a Book to Borrow another.");
                        }
                    }
                    else if (userOnline.id[0] == 'S') // check if student
                    {
                        if (userOnline.hold < 3) // student hold max of 3 books
                        {
                            BorrowBook(userOnline); // if less than 3 let him borrow
                        }
                        else // if 4 then say he cant borrow
                        {
                            printf("\n\tYou can have maximum of 3 Books. Please Return a Book to Borrow another.");
                        }
                    }
                    printf("\n\tPress Enter to go next...\n\t");
                    getchar();
                    getchar();
                    system("cls");
                    clearScreen();
                }
                else if (b == 2) // if choice return book
                {
                    system("cls");
                    clearScreen();
                    if (userOnline.hold >= 1) // if user has some books
                    {
                        ReturnBook(userOnline); // then return
                    }
                    else // if user have no books
                    {
                        printf("\n\tYou have no books to return. Borrow some Book.");
                    }
                    FillEmptyBookSlot();
                    printf("\n\tPress Enter to go next...\n\t");
                    getchar();
                    getchar();
                    system("cls");
                    clearScreen();
                }
                else if (b == 3) // if choice to see catalog
                {
                    system("cls");
                    clearScreen();
                    Catalog(); // show catalog
                    printf("\n\tPress Enter to go next...\n\t");
                    getchar();
                    getchar();
                    system("cls");
                    clearScreen();
                }
                else if (b == 4) // if choice to search
                {
                    system("cls");
                    clearScreen();
                    Search();
                    printf("\n\tPress Enter to go next...\n\t");
                    getchar();
                    system("cls");
                    clearScreen();
                }
                else if (b == 5) // if choice to change password
                {
                    system("cls");
                    clearScreen();
                    ChangePassword(userOnline); // call change pass function
                    printf("\n\tPress Enter to go next...\n\t");
                    getchar();
                    getchar();
                    system("cls");
                    clearScreen();
                }
                else if (b == 6) // if choice is to send request
                {
                    system("cls");
                    clearScreen();
                    Request(userOnline); // call request function
                    printf("\n\tPress Enter to go next...\n\t");
                    getchar();
                    system("cls");
                    clearScreen();
                }
                else if (b == 7) // if choice is to logout
                {
                    ClearForce(userOnline);
                    strcpy(Log, "");
                    login = 0;
                    co = 1;
                    system("cls");
                    clearScreen();
                }
                else // if wrong choice
                {
                    printf("\n\tWorng Entry. Try Again.\n\tPress Enter to go back...\n\t");
                    getchar();
                    getchar();
                    system("cls");
                    clearScreen();
                }
            } while (co == 0);
        }

        if (login == 2) // if admin has logged in
        {
            eo = 0;
            do
            {
                strcpy(userOnline.id, Log); // copy id to log
                printf("\n\n\t");
                for (i = 0; i < 120; i++)
                {
                    printf("*");
                }
                Date();                                                                    // reading current date
                printf("\n\tWelcome back admin.\n\n\tDate: %02d/%02d/%d\n", dd, mm, yyyy); // welcoming admin and showing all option of admin
                printf("\n\tEnter Respective Value\n");
                printf("\n\t%-20s\t%-20s\t%-20s\t%-20s\t%-20s\n\t%-20s\t%-20s\t%-20s\t%-20s\t%-20s\n\n", "1 :Add Book", "2 :Remove Book", "3 :Update Book", "4 :Search Book", "5 :View Catalog", "6 :View Users List", "7 :Remove User", "8 :Read Requests", "9:Change Password", "10:Logout");
                printf("\n\tYour Choice : ");
                scanf("%d", &b); // reading choice into b
                // if(b==1)
                // {
                //     SetDate(); //setting the current date
                //     printf("\n\tPress Enter to go next...\n\t");
                //     getchar();
                //     getchar();
                // }
                // else
                if (b == 1)
                {
                    system("cls");
                    clearScreen();
                    AddBook(); // going to add book function
                    printf("\n\tPress Enter to go next...\n\t");
                    getchar();
                    getchar();
                    system("cls");
                    clearScreen();
                }
                else if (b == 2)
                {
                    system("cls");
                    clearScreen();
                    RemoveBook(); // removing a book from catalog
                    Catalog();    // showing catalog again
                    printf("\n\tPress Enter to go next...\n\t");
                    getchar();
                    getchar();
                    system("cls");
                    clearScreen();
                }
                else if (b == 3)
                {
                    system("cls");
                    clearScreen();
                    UpdateBook(); // Updating a book
                    getchar();
                    system("cls");
                    clearScreen();
                }
                else if (b == 4) // Search
                {
                    system("cls");
                    clearScreen();
                    Search();
                    printf("\n\tPress Enter to go next...\n\t");
                    getchar();
                    system("cls");
                    clearScreen();
                }
                else if (b == 5)
                {
                    system("cls");
                    clearScreen();
                    Catalog(); // showing catalog
                    printf("\n\tPress Enter to go next...\n\t");
                    getchar();
                    getchar();
                    system("cls");
                    clearScreen();
                }
                else if (b == 6)
                {
                    system("cls");
                    clearScreen();
                    UsersList(); // Showing list of users
                    printf("\n\tPress Enter to go next...\n\t");
                    getchar();
                    getchar();
                    system("cls");
                    clearScreen();
                }
                else if (b == 7)
                {
                    system("cls");
                    clearScreen();
                    RemoveUser(); // removing a  user
                    printf("\n\tPress Enter to go next...\n\t");
                    getchar();
                    getchar();
                    system("cls");
                    clearScreen();
                }
                else if (b == 8)
                {
                    system("cls");
                    clearScreen();
                    ReadRequest(); // reading the requests
                    printf("\n\tPress Enter to go next...\n\t");
                    getchar();
                    getchar();
                    system("cls");
                    clearScreen();
                }
                else if (b == 9)
                {
                    system("cls");
                    clearScreen();
                    ChangePassword(userOnline); // changing password
                    printf("\n\tPress Enter to go next...\n\t");
                    getchar();
                    getchar();
                    system("cls");
                    clearScreen();
                }
                else if (b == 10) // logout
                {
                    getchar();
                    strcpy(Log, "");
                    login = 0;
                    eo = 1;
                    system("cls");
                    clearScreen();
                }
                else // if choice is not valid
                {
                    printf("\n\tWorng Entry. Try Again.\n\tPress Enter to go back...\n\t");
                    getchar();
                    getchar();
                    system("cls");
                    clearScreen();
                }
            } while (eo == 0);
        }
    } while (bo == 0);
}
