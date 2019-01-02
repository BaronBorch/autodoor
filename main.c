#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <Windows.h>
#include <time.h>

void get_action_times(int action_time[], struct tm *time);

int main()
{
    int times[4];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    get_action_times(times, &tm);
    printf("open time = %d:%d\n", times[0], times[1]);
    printf("close time = %d:%d\n", times[2], times[3]);
    while(1)
    {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        printf("in while open time = %d:%d\n", times[0], times[1]);
        printf("in while current time = %d:%d\n", tm.tm_hour, tm.tm_min);
        if(tm.tm_hour == 00 && tm.tm_min == 01)
        {
            get_action_times(times, &tm);
            printf("new day times update\n");
        }
        if(tm.tm_hour == times[0] && tm.tm_min == times[1])
        {
            printf("Open the gate!!!\n");
        }
        if(tm.tm_hour == times[2] && tm.tm_min == times[3])
        {
            printf("close the gate!!!\n");
        }
        Sleep(60000);
    }

    return 0;
}

void get_action_times(int action_time[], struct tm *time)
{
    FILE *wschody = NULL;
    char text[50], check_date[50], year_char[5], month_char[3], day_char[3], hour_char[3], min_char[3];
    int count = 0, loop = 0;

    wschody = fopen("wschody.txt","r");
    if(wschody == NULL)
    {
        printf("openining file failed\n");
        exit(1);
    }
    printf("file wschody.txt opened sucessfully!\n");

    while((fgets(text, sizeof(text), wschody) != NULL) || count > 3)
    {
        text[strlen(text) - 1] = '\0';
        switch(count)
        {
            case 0:
            {
                strcpy(check_date, "Month ");
                sprintf(month_char, "%d", time->tm_mon + 1);
                strcat(check_date, month_char);
                strcat(check_date, " ");
                sprintf(year_char, "%d", time->tm_year + 1900);
                strcat(check_date, year_char);
                printf("check date is %s\n", check_date);
                if(strcmp(text, check_date) == 0)
                    count ++;
            }
            break;
            case 1:
            {
                sprintf(day_char, "%d", time->tm_mday);
                strcpy(check_date, day_char);
                printf("check date is %s\n", check_date);
                if(strcmp(text, check_date) == 0)
                    count ++;
            }
            break;
            case 2:
            {
                if(loop == 1)
                {
                    action_time[0] = atoi(text);
                    action_time[1] = atoi(&text[3]);
                }
                if(loop == 3)
                {
                    action_time[2] = atoi(text);
                    action_time[3] = atoi(&text[3]);
                    action_time[1] = action_time[1] - 30;
                    if(action_time[1] < 0)
                    {
                        action_time[1] = 60 + action_time[1];
                        action_time[0] = action_time[0] - 1;
                    }
                    action_time[3] = action_time[3] + 30;
                    if(action_time[3] > 60)
                    {
                        action_time[3] = action_time[3] - 60;
                        action_time[2] = action_time[2] + 1;
                    }
                    count ++;
                }
                loop ++;
            }
        }
    }
    fclose(wschody);
}

