#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>

// Number of nodes and edges
uint32_t numNodes;
uint32_t numEdges;

// Mapping from node id to array of neighbouring node ids
// First element of each array holds the number of neighbours
uint32_t** neighbours;

void readGraph(const char* filename)
{
  // Read edges
  FILE* fp = fopen(filename, "rt");
  if (fp == NULL) {
    fprintf(stderr, "Can't open '%s'\n", filename);
    exit(EXIT_FAILURE);
  }

  // Create mapping from node id to number of neighbours
  int ret = fscanf(fp, "%d %d", &numNodes, &numEdges);
  uint32_t* count = (uint32_t*) calloc(numNodes, sizeof(uint32_t));
  for (int i = 0; i < numEdges; i++) {
    uint32_t src, dst;
    ret = fscanf(fp, "%d %d", &src, &dst);
    count[src]++;
    count[dst]++;
  }

  // Create mapping from node id to neighbours
  neighbours = (uint32_t**) calloc(numNodes, sizeof(uint32_t*));
  rewind(fp);
  ret = fscanf(fp, "%d %d", &numNodes, &numEdges);
  for (int i = 0; i < numNodes; i++) {
    neighbours[i] = (uint32_t*) calloc(count[i]+1, sizeof(uint32_t));
    neighbours[i][0] = count[i];
  }
  for (int i = 0; i < numEdges; i++) {
    uint32_t src, dst;
    ret = fscanf(fp, "%d %d", &src, &dst);
    neighbours[src][count[src]--] = dst;
    neighbours[dst][count[dst]--] = src;
  }

  // Release
  free(count);
  fclose(fp);
}

// Compute sum of all shortest paths
uint64_t ssp()
{
  // Sum of distances
  uint64_t sum = 0;

  // Visited array
  uint8_t *visited = (uint8_t*) calloc(numNodes, sizeof(uint8_t));

  // Fringe array and size
  uint32_t *fringe = (uint32_t*) calloc(numNodes, sizeof(uint32_t));
  uint32_t fringeSize;

  // New fringe array and size
  uint32_t *newFringe = (uint32_t*) calloc(numNodes, sizeof(uint32_t));
  uint32_t newFringeSize;

  // Apply BFS from each vertex in turn
  for (uint32_t src = 0; src < numNodes; src++) {
    newFringeSize = 0;
    fringeSize = 1;
    fringe[0] = src;
    memset(visited, 0, numNodes);
    visited[src] = 1;

    uint32_t d = 1;
    while (fringeSize > 0) {
      // For each node in fringe
      for (uint32_t i = 0; i < fringeSize; i++) {
        uint32_t v = fringe[i];
        // For each neighbour
        uint32_t numNeighbours = neighbours[v][0];
        for (uint32_t j = 1; j <= numNeighbours; j++) {
          uint32_t n = neighbours[v][j];
          if (! visited[n]) {
            visited[n] = 1;
            newFringe[newFringeSize++] = n;
            sum += d;
          }
        }
      }
      // Increment distance and move to new fringe
      d++;
      uint32_t* p = fringe; fringe = newFringe; newFringe = p;
      fringeSize = newFringeSize; newFringeSize = 0;
    }
  }

  return sum;
}

int main(int argc, char**argv)
{
  if (argc != 2) {
    printf("Specify edges file\n");
    exit(EXIT_FAILURE);
  }
  readGraph(argv[1]);

  struct timeval start, finish, diff;

  gettimeofday(&start, NULL);
  uint64_t sum = ssp();
  gettimeofday(&finish, NULL);

  printf("Sum of shortest paths = %lu\n", sum);
 
  timersub(&finish, &start, &diff);
  double duration = (double) diff.tv_sec + (double) diff.tv_usec / 1000000.0;
  printf("Time = %lf\n", duration);

  return 0;
}
