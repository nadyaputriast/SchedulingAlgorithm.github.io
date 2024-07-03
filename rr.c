#include <stdio.h>
#include <limits.h>

int main() 
{
    int n, time_quantum;
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

    printf("Enter time quantum: ");
    scanf("%d", &time_quantum);

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
        int executed = 0;
        for (int i = 0; i < n; i++) 
        {
            if (remaining_bt[i] > 0 && at[i] <= current_time && !is_completed[i]) 
            {
                executed = 1;
                if (rt[i] == -1)
                    rt[i] = current_time - at[i];

                if (remaining_bt[i] <= time_quantum)
                {
                    current_time += remaining_bt[i];
                    remaining_bt[i] = 0;
                    ct[i] = current_time;
                    completed++;
                    is_completed[i] = 1;

                    tat[i] = ct[i] - at[i];
                    wt[i] = tat[i] - bt[i];
                } 
                else 
                {
                    current_time += time_quantum;
                    remaining_bt[i] -= time_quantum;
                }
            }
        }

        if (!executed)
        {
            int next_at = INT_MAX;
            for (int j = 0; j < n; j++) 
                if (remaining_bt[j] > 0 && at[j] > current_time && at[j] < next_at) 
                    next_at = at[j];
            if (next_at != INT_MAX)
                current_time = next_at;
        }
    }

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (pid[j] > pid[j + 1])
            {
                int temp = pid[j];
                pid[j] = pid[j + 1];
                pid[j + 1] = temp;

                temp = at[j];
                at[j] = at[j + 1];
                at[j + 1] = temp;

                temp = bt[j];
                bt[j] = bt[j + 1];
                bt[j + 1] = temp;

                temp = ct[j];
                ct[j] = ct[j + 1];
                ct[j + 1] = temp;

                temp = tat[j];
                tat[j] = tat[j + 1];
                tat[j + 1] = temp;

                temp = wt[j];
                wt[j] = wt[j + 1];
                wt[j + 1] = temp;

                temp = rt[j];
                rt[j] = rt[j + 1];
                rt[j + 1] = temp;
            }
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