#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <sys/time.h>
#include <string.h>

// Use vectors called "chunks"
#define BitsPerChunk 256
typedef int vec __attribute__ ((vector_size (BitsPerChunk/8)));
typedef union {
  vec bits;
  uint64_t elem[BitsPerChunk/64];
} Chunk;

// Number of nodes and edges
uint32_t numNodes;
uint32_t numEdges;

// Mapping from node id to array of neighbouring node ids
// First element of each array holds the number of neighbours
uint32_t** neighbours;

// Mapping from node id to bit vector of reaching nodes
Chunk** reaching;
Chunk** reachingNext;

// Number of chunks = ceiling(numNodes / BitsPerChunk)
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
  numChunks = (numNodes+BitsPerChunk-1) / BitsPerChunk;
  reaching = (Chunk**) calloc(numNodes, sizeof(Chunk*));
  reachingNext = (Chunk**) calloc(numNodes, sizeof(Chunk*));
  for (int i = 0; i < numNodes; i++) {
    reaching[i] =
      (Chunk*) aligned_alloc(BitsPerChunk, numChunks * sizeof(Chunk));
    reachingNext[i] =
      (Chunk*) aligned_alloc(BitsPerChunk, numChunks * sizeof(Chunk));
    memset(reaching[i], 0, numChunks * sizeof(Chunk));
    memset(reachingNext[i], 0, numChunks * sizeof(Chunk));
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
    uint64_t* reaching64 = (uint64_t*) reaching[i];
    uint64_t* reachingNext64 = (uint64_t*) reachingNext[i];
    reaching64[i/64] = 1ul << (i%64);
    reachingNext64[i/64] = 1ul << (i%64);
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
          reachingNext[i][k].bits |= reaching[n][k].bits;
      }
      // Update sums
      for (int k = 0; k < numChunks; k++) {
        Chunk diff;
        diff.bits = reachingNext[i][k].bits & ~reaching[i][k].bits;
        uint32_t n = 0;
        for (int c = 0; c < BitsPerChunk/64; c++)
          n += __builtin_popcountll(diff.elem[c]);
        sum += n * dist;
      }
    }


    // For each node, update reaching vector
    done = 1;
    for (int i = 0; i < numNodes; i++) {
      uint32_t n = 0;
      for (int k = 0; k < numChunks; k++) {
        reaching[i][k].bits = reachingNext[i][k].bits;
        for (int c = 0; c < BitsPerChunk/64; c++)
          n += __builtin_popcountll(reaching[i][k].elem[c]);
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
