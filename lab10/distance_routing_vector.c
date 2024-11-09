// CS22B1093 ROHAN G

#include <stdio.h>
#include <stdbool.h>

#define MAX 4
#define INF 100

// no link -> INF == 100

void print_table(int dist[MAX][MAX]) {
    printf("\nRouting tables:\n");
    printf("   ");
    for (int i = 0; i<MAX; i++) printf("  %c  ", 'A' + i);
    printf("\n");
    for (int i = 0; i < MAX; i++) {
        printf("%c: ", 'A' + i);
        for (int j = 0; j < MAX; j++) {
            if (dist[i][j] == INF) {
                printf(" INF ");
            } else {
                printf("  %d  ", dist[i][j]);
            }
        }
        printf("\n");
    }
}

void distance_vector_routing(int graph[MAX][MAX]) {
    int dist[MAX][MAX];
    bool updated;

    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            dist[i][j] = graph[i][j];
        }
    }

    int step = 1;
    do {
        updated = false;
        printf("\n--- Step %d ---", step++);
        
        for (int src = 0; src < MAX; src++) {
            for (int dest = 0; dest < MAX; dest++) {
                for (int via = 0; via < MAX; via++) {
                    if (graph[src][via] != INF && dist[via][dest] != INF) {
                        int new_distance = graph[src][via] + dist[via][dest];
                        if (new_distance < dist[src][dest]) {
                            dist[src][dest] = new_distance;
                            updated = true;
                        }
                    }
                }
            }
        }

        print_table(dist);

    } while (updated);
}

int main() {
    int i, j;
    
    int graph[MAX][MAX];

    printf("Enter the Topology matrix : \n");
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            scanf("%d", &graph[i][j]);
        }
    }

    printf("\nInput Topology Matrix : \n");
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            printf("%d ", graph[i][j]);
        }
        printf("\n");
    }
    
    // printf("Initial topology matrix:\n");
    print_table(graph);

    distance_vector_routing(graph);

    printf("\n--- Convergence achieved ---\n");

    return 0;
}