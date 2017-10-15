#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <sys/time.h>

// Number of nodes and edges
uint32_t numNodes;
uint32_t numEdges;

// Mapping from node id to array of neighbouring node ids
// First element of each array holds the number of neighbours
uint32_t** neighbours;

// Mapping from node id to bit vector of reaching nodes
uint64_t** reaching;
uint64_t** reachingNext;

// Number of chunks = ceiling(numNodes / 64)
uint32_t numChunks;

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


  // Create mapping from node id to bit vector of reaching nodes
  numChunks = (numNodes+63) / 64;
  reaching = (uint64_t**) calloc(numNodes, sizeof(uint64_t*));
  reachingNext = (uint64_t**) calloc(numNodes, sizeof(uint64_t*));
  for (int i = 0; i < numNodes; i++) {
    reaching[i] = (uint64_t*) calloc(numChunks, sizeof(uint64_t));
    reachingNext[i] = (uint64_t*) calloc(numChunks, sizeof(uint64_t));
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

  // Initialise reaching vector for each node
  for (uint64_t i = 0; i < numNodes; i++) {
    reaching[i][i/64] = 1ul << (i%64);
    reachingNext[i][i/64] = 1ul << (i%64);
  }

  // Distance increases on each iteration
  uint32_t dist = 1;

  int done = 0;
  while (! done) {
    // For each node
    for (int i = 0; i < numNodes; i++) {
      uint32_t numNeighbours = neighbours[i][0];
      // For each neighbour
      for (int j = 1; j <= numNeighbours; j++) {
        uint32_t n = neighbours[i][j];
        // For each chunk
        for (int k = 0; k < numChunks; k++)
          reachingNext[i][k] = reachingNext[i][k] | reaching[n][k];
      }
      // Update sums
      for (int k = 0; k < numChunks; k++) {
        uint64_t diff = reachingNext[i][k] & ~reaching[i][k];
        uint32_t n = __builtin_popcountll(diff);
        sum += n * dist;
      }
    }


    // For each node, update reaching vector
    done = 1;
    for (int i = 0; i < numNodes; i++) {
      uint32_t n = 0;
      for (int k = 0; k < numChunks; k++) {
        reaching[i][k] = reachingNext[i][k];
        n += __builtin_popcountll(reaching[i][k]);
      }
      if (n != numNodes) done = 0;
    }
    

    dist++;
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
