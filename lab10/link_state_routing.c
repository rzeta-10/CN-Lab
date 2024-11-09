// CS22B1093 ROHAN G

#include <stdio.h>
#include <stdbool.h>

#define MAX 4
#define INF 100

// no link -> INF == 100

void print_path(int dist[], int prev[], int src) {
    printf("Shortest paths from node %c:\n", 'A' + src);
    for (int i = 0; i < MAX; i++) {
        if (i != src) {
            printf("To %c: Cost = %d, Path = %c", 'A' + i, dist[i], 'A' + src);
            int j = i;
            while (prev[j] != -1 && prev[j] != src) {
                printf(" -> %c", 'A' + prev[j]);
                j = prev[j];
            }
            printf(" -> %c\n", 'A' + i);
        }
    }
}

void dijkstra(int graph[MAX][MAX], int src) {
    int dist[MAX];     
    bool visited[MAX]; 
    int prev[MAX];     

    for (int i = 0; i < MAX; i++) {
        dist[i] = INF;
        visited[i] = false;
        prev[i] = -1;
    }
    dist[src] = 0;

    for (int count = 0; count < MAX - 1; count++) {
        int min = INF, u = -1;

        for (int v = 0; v < MAX; v++) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                u = v;
            }
        }

        visited[u] = true;

        for (int v = 0; v < MAX; v++) {
            if (!visited[v] && graph[u][v] && dist[u] != INF &&
                dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                prev[v] = u;
            }
        }
    }

    print_path(dist, prev, src);
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

    for (int i = 0; i < MAX; i++) {
        printf("\n--- Shortest paths from node %c ---\n", 'A' + i);
        dijkstra(graph, i);
    }

    return 0;
}