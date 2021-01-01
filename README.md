# LIBRARY-MANAGEMENT-SYSTEM-IN-C
This is a simple program in C for library management system. The system has admin and users.
Admin can add, update or remove the Books and its details. also view and manage the users. 
Users are a student or faculty. users can borrow and return a book, also message or request a book to admin. 
Other than these there are also some features like changing passwords, viewing catalog, Search book, etc.


#
    ADMIN MANUAL
#
    Admin Registration

For registering as admin you will have to know two main things,
  1. During registration, when the code asks who are you? for 'F' or 'S', admin should give 'A'.
  2. The Developer Password.
  STEPS TO REGISTER:
Step 1: When you are in Home page Press 3 (register option).
  Step 2: Enter your name.
  Step 3: Enter 'A'.
  Step 4: Enter Developer Password.
  Step 5: Give your ID or Roll number. Press enter.
  Step 6: Set a new password. Press enter.
  Step 7: Make a note of your User ID and Password shown on the screen.
  Step 8: Press enter to go back to Home Page.

    Admin Login

Step 1: When in Home Page Press 2 (Login option).
Step 2: Give your User ID. Press enter. (In case the user does not exists, you will have 3 chances
        to enter correct ID,after that you will be exited automatically.)
Step 3: Give your Password. Press enter.(In case the Password does not match, you will have 3 chances
        to enter correct Password,after that you will be exited automatically.)
(If the ID and password match, You will be entering into your admin page.)

    Admin Page

Admin page has Details and Options that admin has access.
It has current Date printed.
Options..
1. Set Date (to Change the current date)
2. Add Book (to add a new book to the catalog)
3. Remove Book (to remove or delete an existing book from catalog)
4. Update Book (to update an existing book, field wise like name,author,id,number of copies.)
5. Search Book (to search existing book, free search by book name or author)
6. View Catalog (to view all the books in the catalog)
7. View Users List (to view all the registered users and books they borrowed)
8. Remove User (to remove an existing user)
9. Read Requests (to read the requests or messages sent by users)
10. Change Password (to change admin password)
11. Logout (to logout of admin account)

    Set Date



    USER MANUAL

    User Registration

Step 1. When you are in Home Page Press 3 (Register option).
Step 2. Enter your Name and press enter.
Step 3. Enter F (case sensitive) if you are a Faculty or
              S (case sensitive) if you are a Student. 
        (if you enter any other character, you will have 3 chances to enter a correct one)
Step 4. Give your ID or Roll Number and press enter.
        (In case the ID already have been registered, you will have 3 chances to enter unique one)
Step 5. Set a new Password and press enter.
Step 6. Make a note of Your User ID and Password.

    User Login

Step 1: When in Home Page Press 2 (Login option).
Step 2: Give your User ID. Press enter. (In case the user does not exists, you will have 3 chances
        to enter correct ID,after that you will be exited automatically.)
Step 3: Give your Password. Press enter.(In case the Password does not match, you will have 3 chances
        to enter correct Password,after that you will be exited automatically.)
        (If the ID and password match, You will be entering into your User page.)

    User Page

It shows all the details of user like, the user name, number of books he have, the table of books he have
with due dates and then the current date. Then it has all the options that user has access of.
Options...
1. Borrow Book (to borrow a book form catalog)
2. Return Book (to return the book which user have)
3. View Catalog (to view all books in the catalog)
4. Search (to search a book, free search by book name and author)
5. Change Password (to change the password of user)
6. Request Book (to send a request or any message to admin)
7. Logout (to logout of the user account)

    Borrow Book

This function first shows all the books which are in the catalog.
Then it asks for the book ID to borrow that book.
Press enter to go back to user page.

