#include <stdio.h>
#include <limits.h>

int main() 
{
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    int pid[n], at[n], bt[n], ct[n], tat[n], wt[n], rt[n];
    int remaining_bt[n];

    printf("Masukkan waktu kedatangan (arrival time) dan lama proses (burst time):\n");
    for (int i = 0; i < n; i++) 
    {
        pid[i] = i + 1;
        printf("Process %d: ", i + 1);
        scanf("%d %d", &at[i], &bt[i]);
        remaining_bt[i] = bt[i];
    }

    int complete = 0, currentTime = 0, minm = INT_MAX;
    int shortest = 0, finish_time;
    int check = 0;

    while (complete != n) 
    {
        for (int j = 0; j < n; j++) 
        {
            if ((at[j] <= currentTime) && (remaining_bt[j] < minm) && remaining_bt[j] > 0) {
                minm = remaining_bt[j];
                shortest = j;
                check = 1;
            }
        }

        if (!check) 
        {
            currentTime++;
            continue;
        }

        if (remaining_bt[shortest] == bt[shortest]) 
            rt[shortest] = currentTime - at[shortest];

        remaining_bt[shortest]--;
        minm = remaining_bt[shortest];
        if (minm == 0)
            minm = INT_MAX;

        if (remaining_bt[shortest] == 0) 
        {
            complete++;
            check = 0;

            finish_time = currentTime + 1;
            ct[shortest] = finish_time;
            tat[shortest] = ct[shortest] - at[shortest];
            wt[shortest] = tat[shortest] - bt[shortest];

            if (wt[shortest] < 0)
                wt[shortest] = 0;
        }
        currentTime++;
    }

    printf("PID\tAT\tBT\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; i++) 
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", pid[i], at[i], bt[i], ct[i], tat[i], wt[i], rt[i]);
    
    return 0;
}