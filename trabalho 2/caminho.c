#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int res = 0;

int nodes = 0;

int *auxCompleted;

// update the bound function for the current level
int bound(int **graph, int i, int n, int *completed, int level) {
    for (int l = 0; l < n; l++) {
        auxCompleted[l] = 0;
    }

    int max;
    int line = i;

    int count = 0;
    int bound = 0;
    while (count < n - level) {
        count++;
        max = 0;

        for (int j = 0; j < n; j++) {
            if (graph[line][j] > max && line != j && (completed[j] == 0 || auxCompleted[j] == 0)) {
                auxCompleted[j] = 1;   
                max = graph[line][j];
                line = j;
            }
        }

        bound += max;
    }

    return bound;
}

// copy the partial result array to the finalResult array
void copyToFinal(int *currPath, int *finalPath, int n) {
    for (int i = 0; i < n; i++) {
        finalPath[i] = -1;
    }
    for (int i = 0; i < n; i++) {
        finalPath[i] = currPath[i];
    }
}

void branchAndBound(int **graph, int currWeight, int level, int *currentPath, int n, int *finalPath, int *completed) {
    
    // base case is when we have reached level N which
    // means we have covered all the nodes once
    if (level == n) {
        // check if there is an edge from last vertex in
        // path back to the first vertex if we arrives on the end of the path
        if (graph[currentPath[level-1]][currentPath[0]] != 0) {
            int currRes = currWeight + graph[currentPath[level-1]][currentPath[0]];

            // Update final result and final path if
            // current result is better.
            if (currRes > res) {
                copyToFinal(currentPath, finalPath, n);
                res = currRes;
            }
        }
        return;
    }
    // if we have not reached level N, then check if we can go back to the
    // first vertex, if so then save the path so far and update final_res
    else if (graph[currentPath[level-1]][currentPath[0]] != 0 && level > 2) {
        int currRes = currWeight + graph[currentPath[level-1]][currentPath[0]];
        
        if (currRes > res) {
            copyToFinal(currentPath, finalPath, n);
            res = currRes;
        }
    }
    
    // for any other level iterate for all vertices to
    // build the search space tree recursively
    int currentBound;
    
    for (int i = 0; i < n; i++) {
        
        // Consider next vertex if it is not same (diagonal
        // entry in adjacency matrix and not visited already)
        if (graph[currentPath[level-1]][i] != 0 && completed[i] == 0) {
            // currentBound is current upperBound
            currWeight += graph[currentPath[level-1]][i];
            currentBound = currWeight + bound(graph, i, n, completed, level);

            completed[i] = 1;
            currentPath[level] = i;

            // for the node that we have arrived on
            // If current bound > res, we need to explore            
            // the node further
            if (currentBound > res) {                
                nodes++;
  
                // call the function for the next node
                branchAndBound(graph, currWeight, level+1, currentPath, n, finalPath, completed);

                currentPath[level] = -1;
            }
  
            // Else we have to prune the node by resetting
            // all changes to currWeight and the chosen Set
            currWeight -= graph[currentPath[level-1]][i];
            completed[i] = 0;
        }        
    }
}

void maxPath(int **graph, int *completed, int n, int *finalPath) {
    int *currentPath = malloc(n * sizeof(int));

    // set currentPath array to -1
    for (int i = 0; i < n; i++) {
        currentPath[i] = -1;
    }

    currentPath[0] = 0;

    nodes++;

    branchAndBound(graph, 0, 1, currentPath, n, finalPath, completed);
}

int main () {
    // reading the size of 2d array
    int n;
    if (!scanf( "%d", &n)) {
        return 1;
    }

    // malloc all the needed structures
    // to be used
    int *graph[n];
    for (int i = 0; i < n; i++) {
         graph[i] = (int *)malloc(n * sizeof(int));
    }

    int *completed = malloc(n * sizeof(int));
    auxCompleted = malloc(n * sizeof(int));
    int *finalPath = malloc(n * sizeof(int));
    for (int i = 0; i < 0; i++) {
        auxCompleted[i] = 0;
        completed[i] = 0;
        finalPath[i] = -1;
    }
    finalPath[0] = 0;
    completed[0] = 1;
    
    // read the 2d array graph
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

    // call the maxPath function
    clock_t begin = clock();

    maxPath(graph, completed, n, finalPath);

    clock_t end = clock();

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;


    // printing output
    printf("Execution time: %f seconds\n", time_spent);
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