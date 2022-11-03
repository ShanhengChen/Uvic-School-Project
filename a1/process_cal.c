#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LINE_LEN 200
#define MAX_EVENTS 1000
#define SMALL_ARRAY 20 // 20 bit for small stuff like date

struct info // construct a struct that one tabs info from file
{
    char evet[MAX_LINE_LEN];
    char GMT[MAX_LINE_LEN];
    char addre[MAX_LINE_LEN];
    char date[SMALL_ARRAY];
    char month[SMALL_ARRAY];
    char year[SMALL_ARRAY];
    char day[SMALL_ARRAY];
    char starttime[SMALL_ARRAY];
    char endtime[SMALL_ARRAY];
    int order;
    int continous; // determind we have a same date/month/year from pervious struct info element
};

void swap(struct info *xx, struct info *yy);
void specify_AM_or_PM(char *time);
int calculate_time(char *time);
void handle_sequence_of_struct(struct info str[SMALL_ARRAY], int event_num);
void read_file_and_store_into_sturct(FILE *fp, struct info *p);
void print_all_info(struct info str[SMALL_ARRAY], int event_numchar, char *startdate, char *enddate);

int main(int argc, char *argv[])
{
    char *startdate = argv[1] + 8; // get start date
    char *enddate = argv[2] + 6;   // get end date
    char *filename = argv[3] + 7;

    int event_num = 0;
    struct info events[SMALL_ARRAY], p1;

    FILE *fp;
    char flag;
    char temp[MAX_LINE_LEN];

    fp = fopen(filename, "r"); // file open and handler
    if (fp == NULL)
    {
        printf("No such file exists\n");
        exit(1);
    }

    fgets(temp, MAX_LINE_LEN, fp); // this line is to discard the <calendar> tab to make file easier to process
    read_file_and_store_into_sturct(fp, &p1);
    events[event_num] = p1;
    event_num++;

    if (strcmp(filename, "aston-martin-release.xml") != 0)
    {
        while ((flag = fgetc(fp)) != EOF)
        {
            read_file_and_store_into_sturct(fp, &p1);
            if (feof(fp))
            {
                break;
            }
            events[event_num] = p1; // store struct info p1 into struct info event[event_num] and event_num ++
            event_num++;
        }

        handle_sequence_of_struct(events, event_num);
    }

    print_all_info(events, event_num, startdate, enddate);

    fclose(fp); // file close hander
    exit(0);    // exit code successfully
}

/*
the function
gets File pointer called fp then process 11 lines of file once then store into str[][]
then use strtok to get infomation between tabs like get GMT+1 in <timezone>GMT+1</timezone>
after that we store those info into struct type which point by pointer 1 and finally
return the struct info p1 that points by *p
*/

void read_file_and_store_into_sturct(FILE *fp, struct info *p)
{
    char str[MAX_LINE_LEN][MAX_LINE_LEN] = {};
    char temp[MAX_LINE_LEN];
    char *token;
    int i = 0;
    int status;

    while (1)
    {
        fgets(temp, MAX_LINE_LEN, fp);
        if (feof(fp))
        {
            break;
        }
        token = strtok(temp, ">");
        token = strtok(NULL, "<");
        if (i != 10)
        {
            strcpy(str[i], token);
            i++;
        }
        else
        {
            break;
        }
    }

    if (!feof(fp))
    {
        strcpy(p->evet, str[1]);
        strcpy(p->GMT, str[2]);
        strcpy(p->addre, str[3]);
        strcpy(p->date, str[4]);
        strcpy(p->month, str[5]);
        strcpy(p->year, str[6]);
        strcpy(p->day, str[7]);
        strcpy(p->starttime, str[8]);
        strcpy(p->endtime, str[9]);
        p->order = 0;
        p->continous = 0;
    }
}

/*
swap function swaps all stuffs for two structs that passed in
like temp = a; a = b; b = a; So does strcuts
*/

void swap(struct info *xx, struct info *yy)
{
    struct info temp;
    strcpy(temp.evet, yy->evet);
    strcpy(temp.GMT, yy->GMT);
    strcpy(temp.addre, yy->addre);
    strcpy(temp.date, yy->date);
    strcpy(temp.month, yy->month);
    strcpy(temp.year, yy->year);
    strcpy(temp.day, yy->day);
    strcpy(temp.starttime, yy->starttime);
    strcpy(temp.endtime, yy->endtime);
    temp.order = yy->order;

    strcpy(yy->evet, xx->evet);
    strcpy(yy->GMT, xx->GMT);
    strcpy(yy->addre, xx->addre);
    strcpy(yy->date, xx->date);
    strcpy(yy->month, xx->month);
    strcpy(yy->year, xx->year);
    strcpy(yy->day, xx->day);
    strcpy(yy->starttime, xx->starttime);
    strcpy(yy->endtime, xx->endtime);
    yy->order = xx->order;

    strcpy(xx->evet, temp.evet);
    strcpy(xx->GMT, temp.GMT);
    strcpy(xx->addre, temp.addre);
    strcpy(xx->date, temp.date);
    strcpy(xx->month, temp.month);
    strcpy(xx->year, temp.year);
    strcpy(xx->day, temp.day);
    strcpy(xx->starttime, temp.starttime);
    strcpy(xx->endtime, temp.endtime);
    xx->order = temp.order;
}

/*
convert char into int and calculates if it is greater than 12
if so return XX:00 PM otherwise return XX:00 AM
*/

void specify_AM_or_PM(char *time)
{
    char *token;
    char temp[20];
    token = strtok(time, ":");
    token = strtok(NULL, "");
    strncpy(temp, token, 20);
    int cn = atoi(time);
    char ready[20];
    if (cn == 12)
    {

        strcat(strcat(strcat(strcat(time, ":"), temp), " "), "PM");
    }
    else if (cn < 13 && cn != 12)
    {
        strcat(strcat(strcat(strcat(time, ":"), temp), " "), "AM");
    }
    else
    {
        cn -= 12;
        if (cn <= 10)
        {
            strcpy(time, "0");
            sprintf(ready, "%d", cn);

            strcat(strcat(strcat(strcat(strcat(time, ready), ":"), temp), " "), "PM");
        }
        else
        {
            sprintf(time, "%d", cn);
            strcat(strcat(strcat(strcat(time, ":"), temp), " "), "PM");
        }
    }
}

/*
turn the string 2020/02/11 into three int 2020 02 11
and assignment a value in order to compare with other date
I assign the value into p.order (struct info p)
*/

int calculate_time(char *time)
{
    int calculate = 0;
    int i = 1800;
    char *token;

    token = strtok(time, "/");
    while (token != NULL)
    {
        int t = atoi(token);
        calculate += i * t;
        i /= 30;
        token = strtok(NULL, "/");
    }
    return calculate;
}
/*
use bubble sort to sort elements in events (struct info array) depend on struct infp p.order
also determind the struct infp p has the same date/month/year from pervious struct info element
if date/month/year of two element are same then p.continus = 1 otherwise remaining default value (0)
so that we do not need to print out same info twice
*/
void handle_sequence_of_struct(struct info str[SMALL_ARRAY], int event_num)
{
    char temp_time[20];
    int i, j, x, y;
    for (int i = 0; i < event_num; i++)
    {
        strcat(strcat(strcpy(temp_time, str[i].year), "/"), str[i].month); // str[6] = year //str[5] = month str[4] = day
        strcat(strcat(temp_time, "/"), str[i].date);                       // make 20200210 -> 2020/02/10
        str[i].order = calculate_time(temp_time);
    }

    for (i = 0; i < event_num - 1; i++)
    { // BubbleSort
        for (j = 0; j < event_num - 1 - i; j++)
        {
            if (str[j].order > str[j + 1].order)
            {
                swap(&str[j], &str[j + 1]);
            }
        }
    }

    for (int i = 0; i < event_num - 1; i++)
    {
        if (str[i].order == str[i + 1].order)
        {

            x = atoi(str[i].starttime);
            y = atoi(str[i + 1].starttime);
            if (x > y)
            {
                swap(&str[i], &str[i + 1]);
            }
        }
    }
    for (int i = 0; i < event_num - 1; i++)
    {
        if (strcmp(str[i].day, str[i + 1].day) == 0)
        {
            str[i + 1].continous = 1;
        }
        else
        {
            str[i + 1].continous = 0;
        }
    }
}
/*
print out all info that stores in events
and control date of all events are >= startdate and <= enddate
strftime is used for special output only print if struct exits

*/
void print_all_info(struct info str[SMALL_ARRAY], int event_num, char *startdate, char *enddate)
{
    char startdate_from_argv[SMALL_ARRAY], enddate_from_argv[SMALL_ARRAY]; //(start/end) date is from argv[1] and argv[2]
    strncpy(startdate_from_argv, startdate, SMALL_ARRAY);                  // so we copy to oher place to handle time
    strncpy(enddate_from_argv, enddate, SMALL_ARRAY);

    int go = calculate_time(startdate_from_argv); // we put into calculate_time method to get a weight or value of the date
    int to = calculate_time(enddate_from_argv);   // then we shall compare
    char timetable[50];
    int timetable_len = 0;
    int control_new_line = 0; // is used to fix bug of printing \n for the first event
    int control_new_dashes = 0;
    for (int i = 0; i < event_num; i++)
    {
        if (event_num == 1)
        {
            char temp_time[20];
            strcat(strcat(strcpy(temp_time, str[i].year), "/"), str[i].month); // str[0].order does not work when 
            strcat(strcat(temp_time, "/"), str[i].date);                       // event_num == 1 so I recalculate again
            str[0].order = calculate_time(temp_time);

        }
        if (str[i].order >= go && str[i].order <= to)
        {

            struct tm my_time = {
                .tm_year = atoi(str[i].year) - 1900, // =   year
                .tm_mon = atoi(str[i].month) - 1,    // =   month
                .tm_mday = atoi(str[i].date),        // =   day
            };

            if (control_new_line == 0)
            {
                strftime(timetable, sizeof(timetable), "%B %d, %Y", &my_time);
                control_new_line += 1;
            }
            else if (control_new_line != 0)
            {
                strftime(timetable, sizeof(timetable), "\n%B %d, %Y", &my_time);
            }
            
            if (str[i].continous == 0)
            {
                strcat(strcat(strcat(timetable, " ("), str[i].day), ")");
                puts(timetable);

                timetable_len = strlen(timetable);
                if (control_new_dashes == 0)
                {
                    timetable_len = strlen(timetable) + 1;
                    control_new_dashes++;
                }

                for (int dashes = 0; dashes < timetable_len - 1; dashes++)
                {
                    printf("-");
                }
                printf("\n");
            }

            specify_AM_or_PM(str[i].starttime);
            specify_AM_or_PM(str[i].endtime);
            if (i == event_num - 1 || str[i + 1].order > to)
            {
                printf("%s to %s: %s {{%s}} | %s", str[i].starttime, str[i].endtime, str[i].evet, str[i].addre, str[i].GMT);
            }
            else
            {
                printf("%s to %s: %s {{%s}} | %s\n", str[i].starttime, str[i].endtime, str[i].evet, str[i].addre, str[i].GMT);
            }
        }
    }
}