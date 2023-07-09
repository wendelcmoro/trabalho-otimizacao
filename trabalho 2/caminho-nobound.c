#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int res = 0;

int nodes = 0;

// get the first max edge cost for current node
int firstMax(int **graph, int i, int n) {
    int max = 0;

    for (int j = 0; j < n; j++) {
        if (graph[i][j] > max && j != i) {
            max = graph[i][j];
        }
    }

    return max;
}

// get the second max edge cost for current node
int secondMax(int **graph, int i, int n) {
    int first = 0;
    int second = 0;

    for (int j = 0; j < n; j++) {
        if (i != j) {
            if (graph[i][j] >= first) {
                second = first;
                first = graph[i][j];
            }
            else if (graph[i][j] >= second && graph[i][j] != first) {
                second = graph[i][j];
            }
        }
    }
    return second;
}

void copyToFinal(int *currPath, int *finalPath, int n) {
    for (int i = 0; i < n; i++) {
        finalPath[i] = -1;
    }
    for (int i = 0; i < n; i++) {
        finalPath[i] = currPath[i];
    }
    //finalPath[n] = currPath[0];
}

void branchAndBound(int **graph, int currentBound, int currWeight, int level, int *currentPath, int n, int *finalPath, int *completed) {
    
    //printf ("aa\n");
    // base case is when we have reached level N which
    // means we have covered all the nodes once
    if (level == n) {
        // check if there is an edge from last vertex in
        // path back to the first vertex
        //if (graph[currentPath[level-1]][0] != 0) {
            // curr_res has the total weight of the
            // solution we got
            // int currRes;
            if (graph[currentPath[level-1]][currentPath[0]] != 0) {
                int currRes = currWeight + graph[currentPath[level-1]][currentPath[0]];
            // }
  
                // Update final result and final path if
                // current result is better.
                if (currRes > res) {
                    //if (res == currRes && finalPath[n+1] == -1) {
                        copyToFinal(currentPath, finalPath, n);
                        res = currRes;
                        // finalPath[n+1] = 0;
                    //}
                }
            }
        return;
    }
    else if (graph[currentPath[level-1]][currentPath[0]] != 0 && level > 2) {
        int currRes = currWeight + graph[currentPath[level-1]][currentPath[0]];
        
        if (currRes > res) {
            copyToFinal(currentPath, finalPath, n);
            res = currRes;
        }
    }

    // for any other level iterate for all vertices to
    // build the search space tree recursively
    for (int i = 0; i < n; i++) {
        
        // Consider next vertex if it is not same (diagonal
        // entry in adjacency matrix and not visited
        // already)
        if (graph[currentPath[level-1]][i] != 0 && completed[i] == 0) {
            //currWeight += graph[currentPath[level-1]][i];
  
            // different computation of curr_bound for
            // level 2 from the other levels
            //if (level == 1) {
            completed[i] = 1;
            currentPath[level] = i;
            // } else {
            //   currentBound += ((secondMax(graph, currentPath[level], n) + firstMax(graph, i, n))/2);
            // }

            //printf("%d %d\n", firstMax(graph, i, n), secondMax(graph, i, n));
  
            // curr_bound + curr_weight is the actual upper bound
            // for the node that we have arrived on
            // If current lower bound > final_res, we need to explore
            
            // the node further
            currWeight += graph[currentPath[level-1]][i];
            nodes++;

            // call TSPRec for the next level
            branchAndBound(graph, currentBound, currWeight, level+1, currentPath, n, finalPath, completed);

            currentPath[level] = -1;
  
            // Else we have to prune the node by resetting
            // all changes to curr_weight and curr_bound
            currWeight -= graph[currentPath[level-1]][i];
  
            // Also reset the visited array
            for (int k = 0; k < n; k++) {
                completed[k] = 0;
            }

            for (int j = 0; j <= level - 1; j++) {
                completed[currentPath[j]] = 1;
            }

        }        
    }
}

void tsp(int **graph, int *completed, int n, int *finalPath) {
    int *currentPath = malloc(n * sizeof(int));

    int currentBound = 0;

    // set currentPath array to -1
    // also compute the initial bound
    for (int i = 0; i < n; i++) {
        currentPath[i] = -1;
        //currentBound += firstMax(graph, i, n) + secondMax(graph, i, n);

        //printf("%d %d\n", firstMax(graph, i, n), secondMax(graph, i, n));
    }

    // rounding the upperbound
    //currentBound = round(currentBound/2);

    currentPath[0] = 0;

    nodes++;

    branchAndBound(graph, currentBound, 0, 1, currentPath, n, finalPath, completed);
}

int main () {
    int n;
    if (!scanf( "%d", &n)) {
        return 1;
    }

    int *graph[n];
    for (int i = 0; i < n; i++) {
         graph[i] = (int *)malloc(n * sizeof(int));
    }

    int *completed = malloc(n * sizeof(int));
    int *finalPath = malloc(n * sizeof(int));
    for (int i = 0; i < 0; i++) {
        completed[i] = 0;
        finalPath[i] = -1;
    }
    finalPath[0] = 0;

    completed[0] = 1;
    
    // read the 2d array
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j > i) {
                if (!scanf( "%d", &graph[i][j])) {
                    return 1;
                }
            }
            else if (i != j) {
                graph[i][j] = graph[j][i];
            }
            else {
                graph[i][j] = 0;
            }
        }
    }

    clock_t begin = clock();

    tsp(graph, completed, n, finalPath);

    clock_t end = clock();

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Execution time: %f\n", time_spent);
    printf("Number of nodes: %d\n", nodes);

    printf ("%d\n", res);
    for (int i = 0; i < n; i++) {
        if (finalPath[i] != -1) {
            printf ("%d ", finalPath[i] + 1);
        }
    }
    printf("1\n");

    return 0;
}