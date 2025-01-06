#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include<ctype.h>
// Structure to represent a node in the priority queue
typedef struct
{
    char name[50];
    int cost;
    char path[100][50]; // To store the path
    int pathLength;           // Length of the path
} Node;

// Priority Queue for UCS
typedef struct
{
    Node nodes[100];
    int size;
} PriorityQueue;

// Graph structure
typedef struct
{
    char source[50];
    char destination[50];
    int cost;
} Edge;

// Function to insert into the priority queue
void push(PriorityQueue *pq, char *name, int cost, char path[100][50], int pathLength)
{
    Node newNode;
    strcpy(newNode.name, name);
    newNode.cost = cost;
    newNode.pathLength = pathLength;
    for (int i = 0; i < pathLength; i++)
    {
        strcpy(newNode.path[i], path[i]);
    }
    strcpy(newNode.path[pathLength], name);
    newNode.pathLength++;

    pq->nodes[pq->size++] = newNode;

    // Sort the queue based on cost
    for (int i = pq->size - 1; i > 0; i--) {
        if (pq->nodes[i].cost < pq->nodes[i - 1].cost)
        {
            Node temp = pq->nodes[i];
            pq->nodes[i] = pq->nodes[i - 1];
            pq->nodes[i - 1] = temp;
        }
    }
}

// Function to remove the minimum-cost node
Node pop(PriorityQueue *pq)
{
    Node min = pq->nodes[0];
    for (int i = 1; i < pq->size; i++) {
        pq->nodes[i - 1] = pq->nodes[i];
    }
    pq->size--;
    return min;
}

// UCS Algorithm
void uniformCostSearch(Edge graph[], int edges, char *start, char *goal) {
    PriorityQueue pq;
    pq.size = 0;

    // Initialize the priority queue with the start node
    char initialPath[100][50] = {""};
    push(&pq, start, 0, initialPath, 0);

    // Track visited nodes
    char visited[100][50];
    int visitedCount = 0;

    while (pq.size > 0) {
        Node current = pop(&pq);

        // Check if the node is already visited
        int alreadyVisited = 0;
        for (int i = 0; i < visitedCount; i++) {
            if (strcasecmp(visited[i], current.name) == 0) {
                alreadyVisited = 1;
                break;
            }
        }
        if (alreadyVisited) continue;

        // Mark the current node as visited
        strcpy(visited[visitedCount++], current.name);

        // If goal is reached
        if (strcasecmp(current.name, goal) == 0)
         {
            printf("Optimal route cost from %s to %s: %d\n", start, goal, current.cost);
            printf("Path: ");
            for (int i = 0; i < current.pathLength; i++)
            {
                printf("%s", current.path[i]);
                if (i < current.pathLength - 1) printf(" -> ");
            }
            printf("\n");
            return;
        }

        // Add neighbors to the queue
        for (int i = 0; i < edges; i++)
        {
            if (strcasecmp(graph[i].source, current.name) == 0) {
                push(&pq, graph[i].destination, current.cost + graph[i].cost, current.path, current.pathLength);
            }
        }
    }

    printf("No route found from %s to %s\n", start, goal);
}

// Read graph from CSV file
int readGraph(char *filename, Edge graph[])
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return 0;
    }

    char line[100];
    int count = 0;

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%[^,],%d", graph[count].source, graph[count].destination, &graph[count].cost);
        count++;
    }

    fclose(file);
    return count;
}
void toUpperCaseString(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = toupper(str[i]); // Convert each character to uppercase
    }
}
int main()
{
    Edge graph[100];
    int edges = readGraph("graph.csv", graph);

    if (edges == 0) {
        printf("No graph data available.\n");
        return 1;
    }

    char start[50], goal[50];
    printf("Enter source: ");
    scanf("%s", start);
    printf("Enter destination: ");
    scanf("%s", goal);
    toUpperCaseString(start);
    toUpperCaseString(goal);

    uniformCostSearch(graph, edges, start, goal);

    return 0;
}
