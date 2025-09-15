#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INCIDENTS 100

typedef struct {
    int incident_id;
    char type[20];
    int severity;
    char location[20];
    char timestamp[25];
    char status[15];
} Incident;

// Function to load the CSV file
int loadIncidentsCSV(const char *filename, Incident incidents[], int max_rows) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("File not found: %s\n", filename);
        return 0;
    }
    char line[200];
    int i = 0;

    // Skip the header row
    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp) && i < max_rows) {
        line[strcspn(line, "\n")] = 0; // Remove newline

        char *token = strtok(line, ",");
        if (!token) continue;

        incidents[i].incident_id = atoi(token);

        token = strtok(NULL, ",");
        strcpy(incidents[i].type, token);

        token = strtok(NULL, ",");
        incidents[i].severity = atoi(token);

        token = strtok(NULL, ",");
        strcpy(incidents[i].location, token);

        token = strtok(NULL, ",");
        strcpy(incidents[i].timestamp, token);

        token = strtok(NULL, ",");
        strcpy(incidents[i].status, token);

        i++;
    }

    fclose(fp);
    return i;
}

// Merge function
void merge(Incident arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    Incident *L = (Incident *)malloc(n1 * sizeof(Incident));
    Incident *R = (Incident *)malloc(n2 * sizeof(Incident));

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        // Sort by severity, descending order
        if (L[i].severity >= R[j].severity) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    // Copy remaining elements (if any)
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L);
    free(R);
}

// Merge Sort
void mergeSort(Incident arr[], int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Print function
void printIncidents(Incident incidents[], int count) {
    printf("incident_id,type,severity,location,timestamp,status\n");
    for (int i = 0; i < count; i++) {
        printf("%d,%s,%d,%s,%s,%s\n",
            incidents[i].incident_id,
            incidents[i].type,
            incidents[i].severity,
            incidents[i].location,
            incidents[i].timestamp,
            incidents[i].status
        );
    }
}

int main() {
    Incident incidents[MAX_INCIDENTS];
    int count = loadIncidentsCSV("incidents.csv", incidents, MAX_INCIDENTS);

    if (count == 0) {
        printf("No incidents to sort.\n");
        return 1;
    }

    printf("Before sorting:\n");
    printIncidents(incidents, count);

    mergeSort(incidents, 0, count - 1);

    printf("\nAfter sorting by severity (descending):\n");
    printIncidents(incidents, count);

    return 0;
}
