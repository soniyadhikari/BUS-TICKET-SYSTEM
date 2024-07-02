
    //   ~   BUS TICKET BOOKING SYSTEM  ~  //
#include <stdio.h>
#include <string.h>
#include<conio.h>     // getch() is used
#include <Windows.h>  // for colour

struct User {
        char username[20];
        char password[20];
    };
struct Bus {
        char bus_id[10];
        int capacity;
        int available_seats;
    };
struct Booking {
     	char user_name[20];
        char bus_id[10];
        int num_seats;
    };

void setConsoleColor(int color) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
    }

void pressEnterToContinue() {
	    setConsoleColor(6);
        printf("\n\n\n\t\t\t\t\t\tPress Enter to continue...");
        getchar();
        while (getchar() != '\n');
    }

void registerUser() {
	    system("cls");
        struct User user;
        setConsoleColor(3);
        printf("\t\t\t\t\t\t\t\t~~~REGISTRATION~~~\n");
        printf("----------------------------------------------------------------------------------------------------------------------------------------------------\n\n");
        setConsoleColor(7);
        printf("\n\n\n\t\t\t\t\t\tEnter Username: ");
        scanf("%s", user.username);
   	    printf("\n\t\t\t\t\t\tEnter Password: ");
   	    scanf("%s", user.password);
        FILE *userFile = fopen("users.txt", "ab");
        if (userFile == NULL) {
            printf("Error opening file.\n");
            return;
        }
        fwrite(&user, sizeof(struct User), 1, userFile);
        fclose(userFile);
        setConsoleColor(12);
        printf("\n\n\n\t\t\t\t\t\tUser registered successfully!\n");
    }

int loginUser() {
	    system("cls");
	    setConsoleColor(3);
        printf("\t\t\t\t\t\t\t\t~~~LOGIN~~~\n");
        printf("----------------------------------------------------------------------------------------------------------------------------------------------------\n\n");
        setConsoleColor(7);
        char username[20];
        char password[20];
        printf("\n\n\n\t\t\t\t\t\tEnter Username: ");
        scanf("%s", username);
        printf("\n\t\t\t\t\t\tEnter Password: ");
        scanf("%s", password);
        FILE *userFile = fopen("users.txt", "rb");
        if (userFile == NULL) {
            printf("\n\n\n\n\t\t\t\t\t\tError opening file.\n");
            return -1;
        }
   	    struct User user;
        int userIndex = 0;
        while (fread(&user, sizeof(struct User), 1, userFile)) {
            if (strcmp(user.username, username) == 0 && strcmp(user.password, password) == 0) {
                fclose(userFile);
                setConsoleColor(12);
                printf("\n\n\t\t\t\t\t\tUser logged in successfully!\n");
                pressEnterToContinue() ;
                return userIndex;
            }
            userIndex++;
        }
        fclose(userFile);
        printf("\n\n\t\t\t\t\t\tUser login failed.\n");
        return -1;
    }

void displayBuses() {
        system("cls");
        struct Bus buses[] = {
            { "BUS001", 52, 52 },
            { "BUS002", 40, 40 },
            { "BUS003", 32, 32 },
            { "BUS004", 60, 60 }
        };
        FILE *busFile = fopen("buses.txt", "wb");
        if (busFile == NULL) {
            printf("\n\n\n\n\t\t\t\t\t\tError opening file.\n");
            return ;
        }
        fwrite(buses, sizeof(struct Bus), sizeof(buses) / sizeof(struct Bus), busFile);
        setConsoleColor(3);
        printf("\t\t\t\t\t\t\t   ||AVAILABLE BUSES||\n");
        printf("----------------------------------------------------------------------------------------------------------------------------------------------------\n\n\n\n\n");
        setConsoleColor(7);
        for (int i = 0; i < 4; i++) {
            printf("\t\t\t\t\t\t\tBus ID: %s\n", buses[i].bus_id);
            printf("\t\t\t\t\t\t\tCapacity: %d\n", buses[i].capacity);
            printf("\t\t\t\t\t\t\t-----------------------------\n");
        }setConsoleColor(2);  
		printf("\n\n\n\n\n\t\t\t\t\t\tBUS001::Pokhara-->Kathmandu\tDeparture time::6 am\n\t\t\t\t\t\tBUS002::Pokhara-->Kathmandu\tDeparture time::2 pm\n\t\t\t\t\t\tBUS003::Pokhara-->Chitwan\tDeparture time::6 am\n\t\t\t\t\t\tBUS004::Pokhara-->Chitwan\tDeparture time::2 pm\n\n\n");  
        fclose(busFile);
        return ;
    }

void bookSeat() {
        system("cls");
   	    setConsoleColor(3);
        printf("\t\t\t\t\t\t\t\t~~~~BOOKING~~~~\n\n");
        printf("----------------------------------------------------------------------------------------------------------------------------------------------------\n\n\n");
        setConsoleColor(7);
        char user[20];
        printf("\t\t~~USER INFORMATION~~\n");
        printf("\t\tEnter your name: ");
        scanf("%s", user);
        char busId[10];
        printf("\n\n\t\t~~VERIFICATION OF BUS~~");
        printf("\n\t\tEnter Bus ID: ");
        scanf("%s", busId);

        FILE *busFile = fopen("buses.txt", "rb+");
        if (busFile == NULL) {
            printf("Error opening bus data file.\n");
            return;
        }         
	   
        struct Bus bus;
        int busFound = 0;
        while (fread(&bus, sizeof(struct Bus), 1, busFile)) {
            if (strcmp(bus.bus_id, busId) == 0) {
                busFound = 1;
                printf("\n\n\t\t~~BOOKING SEATS~~");
                int seatsToBook;
                printf("\n\t\tEnter the number of seats to book: ");
                scanf("%d", &seatsToBook);

                if (seatsToBook <= 0 || seatsToBook > bus.available_seats) {
                    printf("\t\tInvalid number of seats or not enough available seats.\n");
                    fclose(busFile);
                    return;
                }

                // Load existing bookings for this bus
                FILE *bookingFile = fopen("bookings.txt", "rb+");


                struct Booking existingBooking;
                int existingSeats[bus.capacity];
                memset(existingSeats, 0, sizeof(existingSeats));

                while (fread(&existingBooking, sizeof(struct Booking), 1, bookingFile)) {
                    if (strcmp(existingBooking.bus_id, busId) == 0) {
                        // Mark existing seats as taken
                        existingSeats[existingBooking.num_seats - 1] = 1;
                    }
                }

                fclose(bookingFile);
 
                printf("\n\n\t\tAVAILABLE SEAT NUMBERS: \n"); 
				printf("\t\t");              
                for (int i = 1; i <= bus.capacity; i++) {
                    if(i<= bus.capacity && !existingSeats[i-1]){
				        if (i%2!=0)  setConsoleColor(11);
				        else  setConsoleColor(2);
						printf("%d ", i);
                    }
                }
                setConsoleColor(11); printf("\n\n\t\tWindow seats --> Sky blue\t\t   SEATS ARE ARRANGED AS::");
                setConsoleColor(2);  printf("\n\t\tAisle seats --> Green");
                setConsoleColor(7);
                printf("    \t\t||  1    2       4    3  ||\n\t\t\t\t\t\t\t||  5    6       8    7  ||\n\t\t\t\t\t\t\t||  9    10      12   11 ||");             
            
                // Let the user choose their seat
                int selectedSeats[seatsToBook];

                for (int i = 0; i < seatsToBook; i++) {
                    printf("\n\n\t\tChoose seat number %d: ", i + 1);
                    scanf("%d", &selectedSeats[i]);

                    if (selectedSeats[i] <= 0 || selectedSeats[i] > bus.capacity || existingSeats[selectedSeats[i] - 1]) {
                        printf("\n\n\t\tInvalid seat selection.\n");
                        fclose(busFile);
                        return;
                    }
                    bus.available_seats--;
                    existingSeats[selectedSeats[i] - 1] = 1;
                }

                fseek(busFile, -sizeof(struct Bus), SEEK_CUR);
                fwrite(&bus, sizeof(struct Bus), 1, busFile);
            
                bookingFile = fopen("bookings.txt", "ab");
                if (bookingFile == NULL) {
                    printf("\n\n\t\tError opening booking data file.\n");
                    fclose(busFile);
                    return;
                }

                for (int i = 0; i < seatsToBook; i++) {
                    struct Booking booking;
                    strcpy(booking.user_name, user);
                    strcpy(booking.bus_id, bus.bus_id);
                    booking.num_seats = selectedSeats[i];
                    fwrite(&booking, sizeof(struct Booking), 1, bookingFile);
                }

                fclose(bookingFile);
                printf("\n\n\n\t\tBooking(s) successfully completed!\n\n\n\n");
                break;
            }
        }
        fclose(busFile);
        if (!busFound) {
            printf("\n\n\t\tBus not found.\n");
        }
    }
void cancelBooking() {
        system("cls");
        char username[20];
        setConsoleColor(3);
   	    printf("\t\t\t\t\t\t\t\t~~~~BOOKING CANCELLATION~~~~\n\n");
   	    printf("----------------------------------------------------------------------------------------------------------------------------------------------------\n\n\n");
        setConsoleColor(7);
        char user[20];
        printf("\t\t\t\tEnter your name: ");
        scanf("%s", username);
        FILE *bookingFile = fopen("bookings.txt", "rb+");
        if (bookingFile == NULL) {
            printf("\n\t\t\t\tError opening booking data file.\n");
            return;
        }

        struct Booking booking;
        int bookingFound = 0;
        int seatsToCancel[50]; // Assuming a maximum of 50 seats can be booked by a user
        int numSeatsToCancel = 0;

        while (fread(&booking, sizeof(struct Booking), 1, bookingFile)) {
            if (strcmp(booking.user_name, username) == 0) {
                bookingFound = 1;
                printf("\n\n\t\t\t\t~~Booking Details~~\n");
                printf("\t\t\t\tBus ID: %s\n", booking.bus_id);
                printf("\t\t\t\tSeat Number: %d\n", booking.num_seats);
                char confirm;
                printf("\n\t\t\t\tDo you want to cancel this booking? (y/n): ");
                scanf(" %c", &confirm); // Notice the space before %c to consume the newline character
            
			    if (confirm == 'y' || confirm == 'Y') {                
                    seatsToCancel[numSeatsToCancel] = booking.num_seats;
                    numSeatsToCancel++;
                    printf("\n\t\t\t\tBooking will be canceled.\n");
                }   
		   	  else {
                    printf("\n\t\t\t\tBooking was not canceled.\n");
                }
            }
        }

        rewind(bookingFile);

        if (numSeatsToCancel > 0) {
            FILE *tempFile = fopen("temp.txt", "wb+");
            if (tempFile == NULL) {
                printf("Error creating temporary file.\n");
                fclose(bookingFile);
                return;
            }

            while (fread(&booking, sizeof(struct Booking), 1, bookingFile)) {
                int deleteBooking = 0;
                for (int i = 0; i < numSeatsToCancel; i++) {
                    if (seatsToCancel[i] == booking.num_seats && strcmp(booking.user_name, username) == 0) {
                        deleteBooking = 1;
                        break;
                    }
                }
                if (!deleteBooking) {
                    fwrite(&booking, sizeof(struct Booking), 1, tempFile);
                }
            }

            fclose(bookingFile);
            fclose(tempFile);

            if (remove("bookings.txt") != 0) {
                printf("\n\n\t\t\t\tError deleting the original booking file.\n");
                return;
            } 

            if (rename("temp.txt", "bookings.txt") != 0) {
                printf("Error renaming the temporary file.\n");
                return;
            }

            printf("\n\n\t\t\t\t~~~All selected bookings canceled successfully!~~~\n");
        }
	    else {
            printf("\n\n\t\t\t\tNo bookings were canceled.\n");
            fclose(bookingFile);
        }

        if (!bookingFound) {
            printf("\n\n\t\t\t\tBooking not found for user %s.\n", username);
        }
    }
void displayUser() {
        system("cls");
        setConsoleColor(3);
	    printf("\t\t\t\t\t\t\t\t~~~~DISPLAY BOOKINGS~~~~\n");
        printf("----------------------------------------------------------------------------------------------------------------------------------------------------");
        setConsoleColor(7);
        printf("\n\n\n\n\n\n\t\t\t\t\t\t~~ This feature is only for Admin ~~\n");
        printf("\t\t\t\t\t       -------------------------------------\n");
        printf("\n\n\t\t\t\t\t\tAre you Admin (y/n)? ");
        char isAdmin;
        scanf(" %c", &isAdmin); // Notice the space before %c to consume the newline character
        if (isAdmin == 'y' || isAdmin == 'Y') {
            printf("\n\t\t\t\t\t\tAdmin password: ");
            char pass[20];
            scanf("%s", pass);
            if (strcmp(pass, "password12") == 0) {
                FILE* fp = fopen("bookings.txt", "rb"); // Open in binary mode
                    if (fp == NULL) {
                        printf("\n\n\n\t\t\t\t\t\tNo bookings are done yet.\n");
                        return;
                    }
			    struct Booking book;
                int bookingsFound = 0;
                while (fread(&book, sizeof(struct Booking), 1, fp) == 1) {
                    if (!bookingsFound) {
                        printf("\n\n\t\t\t\t\t\t%-15s%-30s%-10s", "USERNAME", "BUS_ID", "SEAT_NUM");
                        printf("\n\t\t\t\t\t      ------------   ----------                     -----------\n\n");
                    }
                    printf("\t\t\t\t\t\t%-15s%-30s%-10d\n", book.user_name, book.bus_id, book.num_seats);
                    bookingsFound = 1;
                }
                fclose(fp);
                if (!bookingsFound) {
                    printf("\n\n\n\t\t\t\t\t\tNo bookings are done yet.\n");
                }
            }  
	        else {
	        	setConsoleColor(4);
                printf("\n\n\n\t\t\t\t\t\t~~Wrong Password~~\n");}            
        } 
    }

void user_info() {
      	system("cls");
     	setConsoleColor(3);
    	printf("\t\t\t\t\t\t\t\t~~~~USER INFORMATION~~~~\n");
	    printf("----------------------------------------------------------------------------------------------------------------------------------------------------"); 
        setConsoleColor(7);
    	printf("\n\n\n\n\n\n\t\t\t\t\t\t~~This  feature is only for Admin~~\n");
    	printf("\t\t\t\t\t       -------------------------------------\n");
        printf("\n\n\t\t\t\t\t\tAre you Admin y/n?");
        getchar();
        if(getchar()=='y'|| getchar()=='Y'){
       	    printf("\n\t\t\t\t\t\tAdmin password :");
	        char pass[20];
	        scanf("%s",pass);
	        if(strcmp(pass,"password12")==0){
		        struct User user;
	            FILE *fp = fopen("users.txt", "r");
                printf("\n\n\t\t\t\t\t\t%-15s%-30s", "USERNAME", "Password");
                printf("\n\t\t\t\t\t      ------------   ------------\n\n");
    
                while (fread(&user, sizeof(struct User), 1, fp) == 1) {
                    printf("\t\t\t\t\t\t%-15s%-30s\n", user.username, user.password);
                } 
                fclose(fp);
            }
            else{			
                setConsoleColor(4);
                printf("\n\n\n\t\t\t\t\t\t~~Wrong Password~~");}
     	}
    }

void userMenu( ) {
        int choice;
        while (1) {
            system("cls");
            setConsoleColor(3);
            printf("\t\t\t\t\t\t\t\t~~~~USER MENU~~~~\n\n");
            printf("----------------------------------------------------------------------------------------------------------------------------------------------------\n\n\n");
            setConsoleColor(7);
	        printf("\n\n\n\n\n\n\t\t\t\t\t\t\t|-------|------------------------|\n");
            printf("\t\t\t\t\t\t\t|  S.N  |       User Menu        |\n");
            printf("\t\t\t\t\t\t\t|-------|------------------------|\n");
            printf("\t\t\t\t\t\t\t|   1.  | Display Available Buses|\n");
            printf("\t\t\t\t\t\t\t|   2.  |      Book Seat         |\n");
            printf("\t\t\t\t\t\t\t|   3.  |    Cancel Booking      |\n");
            printf("\t\t\t\t\t\t\t|   4.  |    Display Booking     |\n");
            printf("\t\t\t\t\t\t\t|   5.  |      User Info         |\n");
            printf("\t\t\t\t\t\t\t|   6.  |       Log Out          |\n");
            printf("\t\t\t\t\t\t\t|-------|------------------------|\n");
            printf("\n\n\n\n\t\t\t\t\t\tEnter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    displayBuses();
                    break;
                case 2:
                    bookSeat();
                    break;
                case 3:
                    cancelBooking();
                    break;
                case 4:
                 	displayUser();
				    break;	
			    case 5:
            	    user_info();
			     	break; 
		        case 6:
			        system("cls");
			        setConsoleColor(2);
			        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t~~~Logging Out~~~\n\n\n\n\n\n\n\n\n\n\n");
			        printf("\n\n\n\n\n\n\t\t\t\t\t\t\t   ~~Thank you for visiting~~");
			        setConsoleColor(7);
                    return;
                default: {
   			        printf("Invalid choice.\n");
                    system("cls");}
            }
           pressEnterToContinue();
     	}
    }

int main() {
        int choice;
        int loggedInIndex = -1; // -1 means no user logged in
        while (1) {
            system("cls");
            setConsoleColor(3); 
            printf("\t\t\t\t\t\t\t~~~~BUS TICKET BOOKING SYSTEM~~~~\n");
            printf("----------------------------------------------------------------------------------------------------------------------------------------------------\n\n\n"); 
	        printf("\n\n\n\n\n\t\t\t\t\t  **-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**\n");
            setConsoleColor(5);
            printf("\n\t\t\t\t\t        =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
            printf("\n\t\t\t\t\t        =                 WELCOME                   =");
            printf("\n\t\t\t\t\t        =                   TO                      =");
            printf("\n\t\t\t\t\t        =                   BUS                     =");
            printf("\n\t\t\t\t\t        =                 TICKET                    =");
            printf("\n\t\t\t\t\t        =                 BOOKING                   =");
            printf("\n\t\t\t\t\t        =                  SYSTEM                   =");
            printf("\n\t\t\t\t\t        =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
            setConsoleColor(3);
            printf("\n\n\t\t\t\t\t  **-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**\n");
            setConsoleColor(7);
	        printf("\n\n\n\n\n\n\n\t\t\t\t\t Enter any key to continue.....");
            getch();
            system("cls");
            setConsoleColor(3);
         	printf("\n\n\n\n\n\n\n\t\t\t\t\t\t---------------------LOGIN--------------------\n\n\n\n");
            setConsoleColor(7);
	    	printf("\n\n\t\t\t\t\t\t1. Register\n");
            printf("\t\t\t\t\t\t2. Login\n");
            printf("\t\t\t\t\t\t3. Exit\n");
	        printf("\n\n\n\t\t\t\t\tEnter your choice: ");
            scanf("%d", &choice);
       
	        switch (choice) {
                case 1:
                    registerUser();
                    break;
                case 2:
                    loggedInIndex = loginUser();
                    if (loggedInIndex != -1) 
                        userMenu( );
                    break;
                case 3:
                	setConsoleColor(4);
                    printf("\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t~~~Exiting~~~\n");
                exit(0);
                default:{                  
			       	printf("Invalid choice.\n");
                    system("cls");
			        }
            }
            pressEnterToContinue();
        }
        return 0;
    }
    