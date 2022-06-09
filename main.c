#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


//MACRO TO DEFINE IMPORTANT LIMITS
#define MAX_Y  2010
#define MIN_Y  1990
#define MAX_USERNAME 30
#define MAX_PASSWORD  20
#define FILE_NAME  "StudentRecordSystem.bin"
#define MAX_FIRST_NAME 50
#define MAX_LAST_NAME 50
#define MAX_ADDRESS 300
#define FILE_HEADER_SIZE  sizeof(file_header)



//date
typedef struct
{
    int yyyy;
    int mm;
    int dd;
} date;


//file header
typedef struct
{
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
} file_header;



//student info structure
typedef struct
{
    unsigned int student_id;
    char first_name[MAX_FIRST_NAME];
    char last_name[MAX_LAST_NAME];
    char address[MAX_ADDRESS];
    date joining_date;
} student_info;




//top message
void center_message(const char* message)
{
    int len =0;
    int posn = 0;

    //calculates req. no. of spaces
    len = (78 - strlen(message))/2;
    printf("\t\t\t");
    for(posn =0 ; posn < len ; posn++)
    {
        printf(" ");
    }

    printf("%s",message);
}





//top message
void top_message(const char *message)
{
    system("cls");
    printf("\t\t\t###########################################################################");
    printf("\n\t\t\t############                                                   ############");
    printf("\n\t\t\t############        Student Record Management System           ############");
    printf("\n\t\t\t############                                                   ############");
    printf("\n\t\t\t###########################################################################");
    printf("\n\t\t\t---------------------------------------------------------------------------\n");
    center_message(message);
    printf("\n\t\t\t----------------------------------------------------------------------------");
}



//display message
void display_message()
{
    printf("\n\n\n\n\n");
    printf("\n\t\t\t  **-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**\n");
    printf("\n\t\t\t        =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
    printf("\n\t\t\t        =               Student Record              =");
    printf("\n\t\t\t        =                 Management                =");
    printf("\n\t\t\t        =                   System                  =");
    printf("\n\t\t\t        =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n\n");
    printf("\n\t\t\t  **-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**\n");
    printf("\n\n\n\t\t\t Press Any Key To Continue\n");
    getch();
}



//checking name
int val_name(const char *name)
{
    int v_n = 1;
    int len = 0;
    int index = 0;
    len = strlen(name);
    for(index =0; index <len ; ++index)
    {
        if(!(isalpha(name[index])) && (name[index] != '\n') && (name[index] != ' '))
        {
            v_n = 0;
            break;
        }
    }
    return v_n;
}


//leap year checking
int  leap(int year)
{
    return (((year % 4 == 0) &&
             (year % 100 != 0)) ||
            (year % 400 == 0));
}



// date validation. returns 1 if valid
int val_date(date *v_d)
{
    //y,m,d range check
    if (v_d->yyyy > MAX_Y ||
            v_d->yyyy < MIN_Y)
        return 0;

    if (v_d->mm < 1 || v_d->mm > 12)
        return 0;

    if (v_d->dd < 1 || v_d->dd > 31)
        return 0;

    //handles leap years
    if (v_d->mm == 2)
    {
        if (leap(v_d->yyyy))
            return (v_d->dd <= 29);
        else
            return (v_d->dd <= 28);
    }

    //handles 30 day months
    if (v_d->mm == 4 || v_d->mm == 6 ||
            v_d->mm == 9 || v_d->mm == 11)
        return (v_d->dd <= 30);
    return 1;
}





//add student
void add_student()
{
    student_info add_student_info = {0};
    FILE *fp = NULL;
    int status = 0;
    fp = fopen(FILE_NAME,"ab+");


    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }


    top_message("Add New Students");
    printf("\n\n\t\t\tENTER YOUR DETAILS BELOW:");
    printf("\n\t\t\t---------------------------------------------------------------------------\n");
    printf("\n\t\t\tStudent ID  = ");
    fflush(stdin);
    scanf("%u",&add_student_info.student_id);



    do
    {
        printf("\n\t\t\tFirst Name  = ");
        fflush(stdin);
        fgets(add_student_info.first_name,MAX_FIRST_NAME,stdin);
        status = val_name(add_student_info.first_name);
        if (!status)
        {
            printf("\n\t\t\tName contain invalid character. Please enter again.");
        }
    }
    while(!status);



    do
    {
        printf("\n\t\t\tLast Name  = ");
        fflush(stdin);
        fgets(add_student_info.last_name,MAX_LAST_NAME,stdin);
        status = val_name(add_student_info.last_name);
        if (!status)
        {
            printf("\n\t\t\tName contain invalid character. Please enter again.");
        }
    }
    while(!status);



    do
    {
        printf("\n\t\t\tStudent Address  = ");
        fflush(stdin);
        fgets(add_student_info.address,MAX_FIRST_NAME,stdin);
        status = val_name(add_student_info.address);
        if (!status)
        {
            printf("\n\t\t\tName contain invalid character. Please enter again.");
        }
    }
    while(!status);



    do
    {
        //get date year,month and day from user
        printf("\n\t\t\tEnter date in format (day/month/year): ");
        scanf("%d/%d/%d",&add_student_info.joining_date.dd,&add_student_info.joining_date.mm,&add_student_info.joining_date.yyyy);
        //check date validity
        status = val_date(&add_student_info.joining_date);
        if (!status)
        {
            printf("\n\t\t\tPlease enter a valid date.\n");
        }
    }


    while(!status);
    fwrite(&add_student_info,sizeof(add_student_info), 1, fp);
    fclose(fp);
}



//search for student
void student_search()
{
    int found = 0;
    int student_id =0;
    student_info add_student_info = {0};
    FILE *fp = NULL;
    fp = fopen(FILE_NAME,"rb");



    if(fp == NULL)
    {
        printf("\n\t\t\tFile is not opened\n");
        exit(1);
    }


    top_message("Search for Students");

    if (fseek(fp,FILE_HEADER_SIZE,SEEK_SET) != 0)
    {
        fclose(fp);
        printf("\n\t\t\tFacing issues while reading the file\n");
        exit(1);
    }


    printf("\n\n\t\t\tEnter Student ID No. to search:");
    fflush(stdin);
    scanf("%u",&student_id);


    while (fread (&add_student_info, sizeof(add_student_info), 1, fp))
    {
        if(add_student_info.student_id == student_id)
        {
            found = 1;
            break;
        }
    }


    if(found)
    {
        printf("\n\t\t\tStudent ID = %d\n",add_student_info.student_id);
        printf("\t\t\tFirst Name = %s",add_student_info.first_name);
        printf("\n\t\t\tLast Name = %s",add_student_info.last_name);
        printf("\n\t\t\tStudent Address = %s",add_student_info.address);
        printf("\t\t\tStudent Admission date(day/month/year) =  (%d/%d/%d)",add_student_info.joining_date.dd,
               add_student_info.joining_date.mm, add_student_info.joining_date.yyyy);
    }


    else
    {
        printf("\n\t\t\tNo Record");
    }


    fclose(fp);
    printf("\n\n\n\t\t\tPress any key to go to main menu.....");
    fflush(stdin);
    getchar();

}



// view students
void view_student()
{
    int found = 0;
    student_info add_student_info = {0};
    FILE *fp = NULL;
    unsigned int count_s = 1;
    top_message("View Student Details");
    fp = fopen(FILE_NAME,"rb");
    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }
    if (fseek(fp,FILE_HEADER_SIZE,SEEK_SET) != 0)
    {
        fclose(fp);
        printf("Facing issues while reading the file\n");
        exit(1);
    }
    while (fread (&add_student_info, sizeof(add_student_info), 1, fp))
    {
        printf("\n\t\t\tNo. Of Students = %d\n\n",count_s);
        printf("\t\t\tStudent ID = %u\n",add_student_info.student_id);
        printf("\t\t\tStudent First Name = %s",add_student_info.first_name);
        printf("\t\t\tStudent Last Name = %s",add_student_info.last_name);
        printf("\t\t\tStudent Address = %s",add_student_info.address);
        printf("\t\t\tStudent Admission Date(dd/mm/yyyy) =  (%d/%d/%d)\n\n",add_student_info.joining_date.dd,
               add_student_info.joining_date.mm, add_student_info.joining_date.yyyy);
        found = 1;
        ++count_s;
    }


    fclose(fp);


    if(!found)
    {
        printf("\n\t\t\tNo Records");
    }


    printf("\n\n\t\t\tPress Any Key To Go To Main Menu\n");
    fflush(stdin);
    getchar();
}


// delete student entry
void delete_student()
{
    int found = 0;
    int d_s = 0;
    file_header fileHeaderInfo = {0};
    student_info add_student_info = {0};
    FILE *fp = NULL;
    FILE *tmpFp = NULL;
    top_message("Delete Student Details");
    fp = fopen(FILE_NAME,"rb");


    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }


    tmpFp = fopen("tmp.bin","wb");


    if(tmpFp == NULL)
    {
        fclose(fp);
        printf("File is Not Opened\n");
        exit(1);
    }


    fread (&fileHeaderInfo,FILE_HEADER_SIZE, 1, fp);
    fwrite(&fileHeaderInfo,FILE_HEADER_SIZE, 1, tmpFp);
    printf("\n\t\t\tEnter Student ID No. For Delete:");
    scanf("%d",&d_s);


    while (fread (&add_student_info, sizeof(add_student_info), 1, fp))
    {
        if(add_student_info.student_id != d_s)
        {
            fwrite(&add_student_info,sizeof(add_student_info), 1, tmpFp);
        }
        else
        {
            found = 1;
        }
    }


    (found)? printf("\n\t\t\tRecord deleted successfully....."):printf("\n\t\t\tRecord not found");
    fclose(fp);
    fclose(tmpFp);
    remove(FILE_NAME);
    rename("tmp.bin",FILE_NAME);
}


//update function
void update(void)
{
    file_header fileHeaderInfo = {0};
    FILE *fp = NULL;
    unsigned char userName[MAX_USERNAME] = {0};
    unsigned char password[MAX_PASSWORD] = {0};
    top_message("Update Credentials");
    fp = fopen(FILE_NAME,"rb+");


    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }


    fread (&fileHeaderInfo,FILE_HEADER_SIZE, 1, fp);


    if (fseek(fp,0,SEEK_SET) != 0)
    {
        fclose(fp);
        printf("\n\t\t\tFacing Issues While Updating Password\n");
        exit(1);
    }


    printf("\n\n\t\t\tNew Username: ");
    fflush(stdin);
    fgets(userName,MAX_USERNAME,stdin);
    printf("\n\n\t\t\tNew Password: ");
    fflush(stdin);
    fgets(password,MAX_PASSWORD,stdin);
    strncpy(fileHeaderInfo.username,userName,sizeof(userName));
    strncpy(fileHeaderInfo.password,password,sizeof(password));
    fwrite(&fileHeaderInfo,FILE_HEADER_SIZE, 1, fp);
    fclose(fp);
    printf("\n\t\t\tYour Password Has Been Changed Successfully");
    printf("\n\t\t\ttLogin Again:");
    fflush(stdin);
    getchar();
    exit(1);
}




//Display menu
void menu()
{
    int choice = 0;
    do
    {
        top_message("Main Menu");
        printf("\n\n\n\t\t\t1.Add A Student");
        printf("\n\t\t\t2.Search For A Student");
        printf("\n\t\t\t3.View A Student");
        printf("\n\t\t\t4.Delete A Student");
        printf("\n\t\t\t5.Update Credentials");
        printf("\n\t\t\t0.Exit");
        printf("\n\n\n\t\t\tEnter choice => ");
        scanf("%d",&choice);
        switch(choice)
        {
        case 1:
            add_student();
            break;
        case 2:
            student_search();
            break;
        case 3:
            view_student();
            break;
        case 4:
            delete_student();
            break;
        case 5:
            update();
            break;
        case 0:
            printf("\n\n\n\t\t\t\tThank you\n\n\n\n\n");
            exit(1);
            break;
        default:
            printf("\n\n\n\t\t\tInvalid Input. Try again\n");
        }
    }
    while(choice!=0);
}


//login
void login()
{
    unsigned char userName[MAX_USERNAME] = {0};
    unsigned char password[MAX_PASSWORD] = {0};
    int L=0;
    file_header fileHeaderInfo = {0};
    FILE *fp = NULL;
    top_message("Login");
    fp = fopen(FILE_NAME,"rb");


    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }


    fread (&fileHeaderInfo,FILE_HEADER_SIZE, 1, fp);
    fclose(fp);


    do
    {
        printf("\n\n\n\t\t\t\tUsername:");
        fgets(userName,MAX_USERNAME,stdin);
        printf("\n\t\t\t\tPassword:");
        fgets(password,MAX_PASSWORD,stdin);
        if((!strcmp(userName,fileHeaderInfo.username)) && (!strcmp(password,fileHeaderInfo.password)))
        {
            menu();
        }
        else
        {
            printf("\t\t\t\tLogin Failed Enter Again Username & Password\n\n");
            L++;
        }
    }


    while(L<=3);


    if(L>3)
    {
        top_message("Login Failed");
        printf("\t\t\t\tSorry,Unknown User.");
        getch();
        system("cls");
    }
}


//checks if file exists
int file_exists(const char *path)
{
    // Try to open file
    FILE *fp = fopen(path, "rb");
    int status = 0;
    // If file does not exists
    if (fp != NULL)
    {
        status = 1;
        // File exists hence close file
        fclose(fp);
    }
    return status;
}
void init()
{
    FILE *fp = NULL;
    int status = 0;
    const char defaultUsername[] ="admin\n";
    const char defaultPassword[] ="admin\n";
    file_header fileHeaderInfo = {0};
    status = file_exists(FILE_NAME);
    if(!status)
    {
        //create the binary file
        fp = fopen(FILE_NAME,"wb");
        if(fp != NULL)
        {
            //Copy default password
            strncpy(fileHeaderInfo.password,defaultPassword,sizeof(defaultPassword));
            strncpy(fileHeaderInfo.username,defaultUsername,sizeof(defaultUsername));
            fwrite(&fileHeaderInfo,FILE_HEADER_SIZE, 1, fp);
            fclose(fp);
        }
    }
}
int main()
{
    init();
    display_message();
    login();
    return 0;
}