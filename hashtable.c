#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 101   // Prime number for better distribution

typedef struct IncidentInfo {
    char type[32];
    int max_severity;
    int total_count;
    char last_location[32];
    struct IncidentInfo* next; // For collision chaining
} IncidentInfo;

IncidentInfo* hashTable[TABLE_SIZE];

// -- Hash function (from before) --
unsigned int hfun(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return (unsigned int)hash;
}

unsigned int getIndex(const char *key) {
    return hfun(key) % TABLE_SIZE;
}

// -- Insert/Update into hash table --
void insertOrUpdateIncident(const char* type, int severity, const char* location) {
    unsigned int idx = getIndex(type);
    IncidentInfo* node = hashTable[idx];
    while (node) {
        if (strcmp(node->type, type) == 0) {
            // Update this incident type
            if (severity > node->max_severity)
                node->max_severity = severity;
            node->total_count += 1;
            strcpy(node->last_location, location);
            return;
        }
        node = node->next;
    }
    // Not found: create new node
    node = (IncidentInfo*)malloc(sizeof(IncidentInfo));
    strcpy(node->type, type);
    node->max_severity = severity;
    node->total_count = 1;
    strcpy(node->last_location, location);
    node->next = hashTable[idx];
    hashTable[idx] = node;
}

// -- Search in hash table --
IncidentInfo* searchIncident(const char* type) {
    unsigned int idx = getIndex(type);
    IncidentInfo* node = hashTable[idx];
    while (node) {
        if (strcmp(node->type, type) == 0)
            return node;
        node = node->next;
    }
    return NULL;
}

// -- CSV Loader --
void loadIncidentsCSV(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("File not found: %s\n", filename);
        exit(1);
    }
    char line[512];
    fgets(line, sizeof(line), fp); // Skip header

    while (fgets(line, sizeof(line), fp)) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;

        // Example: 101,fire,8,Zone-A,2025-09-15 15:30,ongoing
        char *token;
        int col = 0;
        char incident_type[32], location[32];
        int severity = 0;

        token = strtok(line, ","); // incident_id (unused)
        token = strtok(NULL, ","); // type
        strcpy(incident_type, token);

        token = strtok(NULL, ","); // severity
        severity = atoi(token);

        token = strtok(NULL, ","); // location
        strcpy(location, token);

        // Update hash table by type
        insertOrUpdateIncident(incident_type, severity, location);

        // Rest of line (timestamp, status) ignored for hash table
    }
    fclose(fp);
}

// -- Print all hash table entries --
void printAllIncidents() {
    printf("IncidentType\tTotalCount\tMaxSeverity\tLastLocation\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        IncidentInfo* node = hashTable[i];
        while (node) {
            printf("%s\t\t%d\t\t%d\t\t%s\n",
                node->type, node->total_count, node->max_severity, node->last_location);
            node = node->next;
        }
    }
}

int main() {
    // Initialize hash table to NULL
    for (int i = 0; i < TABLE_SIZE; i++) hashTable[i] = NULL;

    // Load CSV and build hash table
    loadIncidentsCSV("incidents.csv");

    // Print everything in the hash table
    printf("All Incident Types (Aggregated):\n");
    printAllIncidents();

    // Demo: lookup for a particular type
    char query[32];
    printf("\nEnter incident type to search (e.g., Fire): ");
    scanf("%s", query);

    IncidentInfo* info = searchIncident(query);
    if (info) {
        printf("Found: %s - Count: %d, Max Severity: %d, Last Location: %s\n",
            info->type, info->total_count, info->max_severity, info->last_location);
    } else {
        printf("No incident of type '%s' found!\n", query);
    }

    return 0;
}
