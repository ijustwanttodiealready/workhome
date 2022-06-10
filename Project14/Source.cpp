#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <locale.h>


#define true 1
#define false 0
#define VERTICES MAX



FILE* MYFILE;
const int DEBUG = false;
const int LINES = 2;
const int SWITCH_N = 11;
const int HOSTS_N = 12;
const int VERTICES = SWITCH_N + HOSTS_N;
int G[VERTICES][VERTICES], visited[VERTICES], n;

int open_file(char* filename)
{
    if (fopen_s(&MYFILE, filename, "r")) {
        puts("An error occured. File not found.");
        puts("current directory");
        system("pwd");
        return 0;
    }

    return 1;
}

int numPlaces(int n) {
    if (n < 0) return numPlaces((n == INT_MIN) ? INT_MAX : -n);
    if (n < 10) return 1;
    return 1 + numPlaces(n / 10);
}

char* strshift(char* string)
{
    if (!string) return 0;
    char* result = NULL;
    char pattern[] = "[%u, %u], ";
    char* temp;
    char match[200];
    char pin = 0;
    int digit = 0;
    int shift = 1;

    if (!string) return NULL;

    if (*string != '[' || !(string + shift)) return NULL;

    digit = atoi(string + shift);
    shift += numPlaces(digit) + 2;


    digit = atoi(string + shift);
    shift += numPlaces(digit) + 1;

    if (string[shift - 1] == '\0') {
        //printf("%c", check);
    }
    else {
        shift += 2;
    }

    int actual_length = strlen(string) - shift;

    if (actual_length == 0) {
        return NULL;
    }
    result = (char*)malloc(sizeof(char) * actual_length);
    if (!result) return NULL;
    for (int i = 0; i < actual_length; i++) {
        result[i] = string[i + shift];
    }
    result[actual_length + 1] = '\0';

    return result;
}
void printSolution(int path[]);
/* A utility function to check if the vertex v can be added at
   index 'pos' in the Hamiltonian Cycle constructed so far (stored
   in 'path[]') */
int isSafe(int v, int graph[VERTICES][VERTICES], int path[], int pos)
{
    /* Check if this vertex is an adjacent vertex of the previously
       added vertex. */
    if (graph[path[pos - 1]][v] == 0)
        return false;

    /* Check if the vertex has already been included.
      This step can be optimized by creating an array of size VERTICES */
    for (int i = 0; i < pos; i++)
        if (path[i] == v)
            return false;

    return true;
}

/* A recursive utility function to solve hamiltonian cycle problem */
int hamCycleUtil(int graph[VERTICES][VERTICES], int path[], int pos)
{
    /* base case: If all vertices are included in Hamiltonian Cycle */
    if (pos == VERTICES)
    {
        // And if there is an edge from the last included vertex to the
        // first vertex
        if (graph[path[pos - 1]][path[0]] == 1)
            return true;
        else
            return false;
    }
    // Try different vertices as a next candidate in Hamiltonian Cycle.
    // We don't try for 0 as we included 0 as starting point in hamCycle()
    for (int v = 1; v < VERTICES; v++)
    {
        /* Check if this vertex can be added to Hamiltonian Cycle */
        if (isSafe(v, graph, path, pos))
        {
            path[pos] = v;

            /* recur to construct rest of the path */
            if (hamCycleUtil(graph, path, pos + 1) == true)
                return true;

            /* If adding vertex v doesn't lead to a solution,
               then remove it */
            path[pos] = -1;
        }
    }

    /* If no vertex can be added to Hamiltonian Cycle constructed so far,
       then return false */
    return false;
}

/* This function solves the Hamiltonian Cycle problem using Backtracking.
  It mainly uses hamCycleUtil() to solve the problem. It returns false
  if there is no Hamiltonian Cycle possible, otherwise return true and
  prints the path. Please note that there may be more than one solutions,
  this function prints one of the feasible solutions. */
int hamCycle(int graph[VERTICES][VERTICES])
{
    int* path = new int[VERTICES];
    for (int i = 0; i < VERTICES; i++)
        path[i] = -1;

    /* Let us put vertex 0 as the first vertex in the path. If there is
       a Hamiltonian Cycle, then the path can be started from any point
       of the cycle as the graph is undirected */
    path[0] = 0;
    if (hamCycleUtil(graph, path, 1) == false)
    {
        printf("\nSolution does not exist");
        return false;
    }

    printSolution(path);
    return true;
}

/* A utility function to print solution */
void printSolution(int path[])
{
    printf("Solution Exists:"
        " Following is one Hamiltonian Cycle \n");
    for (int i = 0; i < VERTICES; i++)
        printf(" %d ", path[i]);
    // Let us print the first vertex again to show the complete cycle
    printf(" %d ", path[0]);
    printf("\n");
}


int main()
{
    char filename[] = "C://Users//Kirill//Desktop//exam-task-master//network1.txt"; // название файла
    char fileString[200];
    char remainder[200]; // остаток от строки
    char v_arr[LINES][200]; // массив с обоими строками
    char format[] = "[%u,";
    int i = 0;
    int graph[VERTICES][VERTICES];
    size_t str_size; // полезный размер строчки
    setlocale(LC_ALL, "rus");
    if (!open_file(filename)) return 0;

    //    remainder = malloc(sizeof(char) * 200);

    i = 0;
    while (fgets(fileString, 200, MYFILE)) {
        str_size = strlen(fileString) - 3;

        // отформатировал строку
        strncpy_s(remainder, fileString + 1, str_size);
        remainder[str_size] = '\0';
        char h = remainder[strlen(remainder)];
        if (DEBUG) {
            printf("remainder \n%s\n", remainder);
        }
        str_size = strlen(remainder);
        strcpy_s(v_arr[i], remainder);
        i++;
        remainder[0] = '\0';
    }
    if (DEBUG) {
        i = 0;
        while (i < LINES) {
            printf("v_arr[%d] %s\n", i, v_arr[i]);
            i++;
        }
    }
    // указал формат
    int v_from = 0, v_to = 0;
    for (i = 0; i < VERTICES; i++) {
        for (int j = 0; j < VERTICES; j++) {
            graph[i][j] = 0;
        }
    }
    char* temp = NULL;
    int j = 12;
    for (i = 0; *v_arr[0] != 0 && i < VERTICES; i++) {

        sscanf(v_arr[0], format, &v_from);
        sscanf(v_arr[1], format, &v_to);

        v_from = v_from == 0 ? j++ : v_from;

        graph[v_from - 1][v_to - 1] = graph[v_to - 1][v_from - 1] = 1;

        printf("Вершины - %d - %d\n", v_from, v_to);

        if ((temp = strshift(v_arr[0])) != NULL) {
            strcpy_s(v_arr[0], temp);
        }

        if ((temp = strshift(v_arr[1])) != NULL) {
            strcpy_s(v_arr[1], temp);
        }

    }
    puts("матрица смежности\n");
    printf("     ");
    for (int i = 0; i < VERTICES; i++) {

        printf("%d ", i + 1);
        if (i < 9) printf(" ", i);
    }
    putchar('\n');
    for (int i = 0; i < VERTICES * 1.6; i++) {
        printf("--");
    }
    putchar('\n');
    for (i = 0; i < VERTICES; i++) {
        if (i < 9) printf(" ", i);
        printf("%d| ", i + 1);
        for (int j = 0; j < VERTICES; j++) {

            printf(" %d ", graph[i][j]);
        }
        printf("\n");
    }
    puts("");

    hamCycle(graph);

    return 0;
}

