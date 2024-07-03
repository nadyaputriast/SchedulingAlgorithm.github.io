#include <stdio.h>
#include <limits.h>

int main() 
{
    int n;
    int pid[100], at[100], bt[100], ct[100], tat[100], wt[100], rt[100], remaining_bt[100];
    int current_time = 0, completed = 0;
    int is_completed[100] = {0}; 
    float total_wt = 0, total_tat = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter arrival time (AT) and burst time (BT) for each process:\n");
    for (int i = 0; i < n; i++) 
    {
        pid[i] = i + 1;
        printf("Process %d: ", i + 1);
        scanf("%d %d", &at[i], &bt[i]);
        remaining_bt[i] = bt[i];
        rt[i] = -1;
    }

    for (int i = 0; i < n - 1; i++) 
    {
        for (int j = 0; j < n - i - 1; j++) 
        {
            if (at[j] > at[j + 1]) 
            {
                int temp = at[j];
                at[j] = at[j + 1];
                at[j + 1] = temp;

                temp = bt[j];
                bt[j] = bt[j + 1];
                bt[j + 1] = temp;

                temp = pid[j];
                pid[j] = pid[j + 1];
                pid[j + 1] = temp;

                temp = remaining_bt[j];
                remaining_bt[j] = remaining_bt[j + 1];
                remaining_bt[j + 1] = temp;
            }
        }
    }

    current_time = at[0];

    while (completed != n) 
    {
        int shortest = -1;
        int min_remaining_bt = INT_MAX;

        for (int i = 0; i < n; i++) 
        {
            if (at[i] <= current_time && remaining_bt[i] > 0 && remaining_bt[i] < min_remaining_bt) 
            {
                min_remaining_bt = remaining_bt[i];
                shortest = i;
            }
        }

        if (shortest != -1) 
        {
            if (rt[shortest] == -1)
                rt[shortest] = current_time - at[shortest];

            current_time++;
            remaining_bt[shortest]--;

            if (remaining_bt[shortest] == 0) 
            {
                ct[shortest] = current_time;
                tat[shortest] = ct[shortest] - at[shortest];
                wt[shortest] = tat[shortest] - bt[shortest];
                is_completed[shortest] = 1;
                completed++;
            }
        } 
        else 
        {
            current_time++;
        }
    }

    for (int i = 0; i < n; i++) 
    {
        total_wt += wt[i];
        total_tat += tat[i];
    }

    float avg_wt = total_wt / n;
    float avg_tat = total_tat / n;

    printf("PID\tAT\tBT\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; i++)
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", pid[i], at[i], bt[i], ct[i], tat[i], wt[i], rt[i]);

    printf("Average Waiting Time: %.2f\n", avg_wt);
    printf("Average Turnaround Time: %.2f\n", avg_tat);

    return 0;
}