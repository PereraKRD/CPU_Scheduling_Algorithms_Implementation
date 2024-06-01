#include <stdio.h>
#include <stdbool.h>

// Structure to represent a process
struct Process {
    int processID;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int waitingTime;
    int turnaroundTime;
};

// First Come First Serve (FCFS) scheduling algorithm
void fcfs(struct Process processes[], int n) {
    int currentTime = 0;

    for (int i = 0; i < n; i++) {
        if (currentTime < processes[i].arrivalTime) {
            currentTime = processes[i].arrivalTime;
        }
        processes[i].waitingTime = currentTime - processes[i].arrivalTime;
        processes[i].turnaroundTime = processes[i].waitingTime + processes[i].burstTime;
        currentTime += processes[i].burstTime;
    }
}

// Shortest Job First (SJF) scheduling algorithm
void sjf(struct Process processes[], int n) {
    int currentTime = 0;
    int completed = 0;
    bool isCompleted[n];

    for (int i = 0; i < n; i++) {
        isCompleted[i] = false;
    }

    while (completed != n) {
        int idx = -1;
        int minBurst = 50;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && !isCompleted[i]) {
                if (processes[i].burstTime < minBurst) {
                    minBurst = processes[i].burstTime;
                    idx = i;
                }
                if (processes[i].burstTime == minBurst) {
                    if (processes[i].arrivalTime < processes[idx].arrivalTime) {
                        idx = i;
                    }
                }
            }
        }

        if (idx != -1) {
            processes[idx].waitingTime = currentTime - processes[idx].arrivalTime;
            currentTime += processes[idx].burstTime;
            isCompleted[idx] = true;
            processes[idx].turnaroundTime = currentTime - processes[idx].arrivalTime;
            completed++;
        } else {
            currentTime++;
        }
    }
}

// Round Robin (RR) scheduling algorithm
void roundRobin(struct Process processes[], int n, int quantum) {
    int currentTime = 0;
    int completed = 0;
    bool isCompleted[n];
    int queue[100];
    int front = 0, rear = 0;

    for (int i = 0; i < n; i++) {
        isCompleted[i] = false;
        processes[i].remainingTime = processes[i].burstTime;
        processes[i].waitingTime = 0;
    }

    queue[rear++] = 0;  // Enqueue the first process

    while (completed != n) {
        int idx = queue[front++];

        if (processes[idx].remainingTime <= quantum) {
            currentTime += processes[idx].remainingTime;
            processes[idx].remainingTime = 0;
            isCompleted[idx] = true;
            completed++;
            processes[idx].turnaroundTime = currentTime - processes[idx].arrivalTime;
            processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;

            // Add newly arrived processes to the queue
            for (int i = 0; i < n; i++) {
                if (!isCompleted[i] && processes[i].arrivalTime <= currentTime && i != idx) {
                    bool inQueue = false;
                    for (int j = front; j < rear; j++) {
                        if (queue[j] == i) {
                            inQueue = true;
                            break;
                        }
                    }
                    if (!inQueue) {
                        queue[rear++] = i;
                    }
                }
            }
        } else {
            currentTime += quantum;
            processes[idx].remainingTime -= quantum;

            // Add newly arrived processes to the queue
            for (int i = 0; i < n; i++) {
                if (!isCompleted[i] && processes[i].arrivalTime <= currentTime && i != idx) {
                    bool inQueue = false;
                    for (int j = front; j < rear; j++) {
                        if (queue[j] == i) {
                            inQueue = true;
                            break;
                        }
                    }
                    if (!inQueue) {
                        queue[rear++] = i;
                    }
                }
            }
            queue[rear++] = idx;  // Re-enqueue the current process
        }

        // If the queue is empty, enqueue the next process
        if (front == rear) {
            for (int i = 0; i < n; i++) {
                if (!isCompleted[i]) {
                    queue[rear++] = i;
                    break;
                }
            }
        }
    }
}


int main() {
    struct Process processes[] = {
            {1, 0, 10, 0, 0, 0},
            {2, 6, 8, 0, 0, 0},
            {3, 7, 4, 0, 0, 0},
            {4, 9, 5, 0, 0, 0}
    };

    int n = sizeof(processes) / sizeof(processes[0]);
    int quantum = 4;

    // Run FCFS algorithm
    fcfs(processes, n);

    // Display FCFS results
    printf("First Come First Serve (FCFS):\n");
    printf("Process ID | Waiting Time | Turnaround Time\n");
    printf("-----------|--------------|----------------\n");
    for (int i = 0; i < n; i++) {
        printf("\t%d\t   |  \t%d\t\t  |\t\t %d\n", processes[i].processID, processes[i].waitingTime, processes[i].turnaroundTime);
    }

    // Calculate FCFS average waiting time
    int totalWaitingTimeFCFS = 0;
    for (int i = 0; i < n; i++) totalWaitingTimeFCFS += processes[i].waitingTime;
    float avgWaitingTimeFCFS = (float)totalWaitingTimeFCFS / n;

    // Run SJF algorithm
    sjf(processes, n);

    // Display SJF results
    printf("\nShortest Job First (SJF):\n");
    printf("Process ID | Waiting Time | Turnaround Time\n");
    printf("-----------|--------------|----------------\n");
    for (int i = 0; i < n; i++) {
        printf("\t%d\t   |  \t%d\t\t  |\t\t %d\n", processes[i].processID, processes[i].waitingTime, processes[i].turnaroundTime);
    }

    // Calculate and display SJF average waiting time
    int totalWaitingTimeSJF = 0;
    for (int i = 0; i < n; i++) totalWaitingTimeSJF += processes[i].waitingTime;
    float avgWaitingTimeSJF = (float)totalWaitingTimeSJF / n;

    // Run Round Robin algorithm
    roundRobin(processes, n, quantum);

    // Display Round Robin results
    printf("\nRound Robin (RR):\n");
    printf("Process ID | Waiting Time | Turnaround Time\n");
    printf("-----------|--------------|----------------\n");
    for (int i = 0; i < n; i++) {
        printf("\t%d\t   |  \t%d\t\t  |\t\t %d\n", processes[i].processID, processes[i].waitingTime, processes[i].turnaroundTime);
    }

    // Calculate and display Round Robin average waiting time
    int totalWaitingTimeRR = 0;
    for (int i = 0; i < n; i++) totalWaitingTimeRR += processes[i].waitingTime;
    float avgWaitingTimeRR = (float)totalWaitingTimeRR / n;

    printf("\nAvg Waiting Time for FCFS is : %.2f", avgWaitingTimeFCFS);
    printf("\nAvg Waiting Time for SJF is : %.2f", avgWaitingTimeSJF);
    printf("\nAvg Waiting Time for Round Robin (time quantum = 4) is : %.2f\n", avgWaitingTimeRR);

    return 0;
}
