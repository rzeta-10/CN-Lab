#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 4

int topoMatrix[MAX][MAX];

struct node {
    int id;
    int cost[MAX];
};

void bellmanFord(int source, int updated) {
    int distance[MAX];
    for(int i = 0; i<MAX; i++) {
        distance[i] = topoMatrix[source][i];
    }

    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            if(topoMatrix[j][i] != 0) {
                if(distance[i] > distance[j] + topoMatrix[j][i]) {
                    distance[i] = distance[j] + topoMatrix[j][i];
                }
            }
        }
    }

    if(updated == 0) {
        printf("\nDistance from node %d to all other nodes : \n", source);
        for(int i = 0; i<MAX; i++) {
            printf("%d ", distance[i]);
        }
        printf("\n");
    } else {
        printf("\nUpdated Distance from node %d to all other nodes : \n", source);
        for(int i = 0; i<MAX; i++) {
            printf("%d ", distance[i]);
        }
        printf("\n");
    }

    for(int i = 0; i<MAX; i++) {
        if(distance[i] != topoMatrix[source][i]) {
            bellmanFord(i, 1);
        }
    }

    return;
}

int main() {
    int i, j;
    printf("Enter the Topology matrix : \n");
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            scanf("%d", &topoMatrix[i][j]);
        }
    }

    printf("\nInput Topology Matrix : \n");
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            printf("%d ", topoMatrix[i][j]);
        }
        printf("\n");
    }

    for(int i = 0; i<MAX; i++) {
        bellmanFord(i, 0);
    }
}