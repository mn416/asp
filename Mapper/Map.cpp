// Put nodes into groups of a given size such that no nodes in a group
// share any common neighbours.  This is a simple greedy algorithm.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "Set.h"
#include "Seq.h"

typedef unsigned Node;

struct Network {
  unsigned numNodes;
  unsigned numEdges;
  Node* src;
  Node* dst;
  uint64_t** neighbours;
  Set* setOfNeighbours;
  Seq<Node>* seqOfNeighbours;
};

unsigned get(FILE* fp)
{
  unsigned val;
  if (fscanf(fp, "%u", &val) <= 0) {
    fprintf(stderr, "Invalid file format\n");
    exit(EXIT_FAILURE);
  }
  return val;
}

void readNetwork(const char* filename, Network* net)
{
  FILE *fp = fopen(filename, "rt");
  if (fp == NULL) {
    fprintf(stderr, "Error opening file '%s'\n", filename);
    exit(EXIT_FAILURE);
  }
  // Read node and edges counts
  net->numNodes = get(fp);
  net->numEdges = get(fp);
  // Allocate edges
  net->src = new Node [net->numEdges];
  net->dst = new Node [net->numEdges];
  // Read edges
  for (unsigned i = 0; i < net->numEdges; i++) {
    net->src[i] = get(fp);
    net->dst[i] = get(fp);
  }
  fclose(fp);
  // Allocate neighbours
  unsigned numChunks = (net->numNodes + 63) / 64;
  net->setOfNeighbours = new Set [net->numNodes];
  for (unsigned i = 0; i < net->numNodes; i++)
    net->setOfNeighbours[i].alloc(net->numNodes);
  net->seqOfNeighbours = new SmallSeq<Node> [net->numNodes];
  for (unsigned i = 0; i < net->numEdges; i++) {
    Node from = net->src[i];
    Node to = net->dst[i];
    net->setOfNeighbours[from].insert(to);
    net->setOfNeighbours[to].insert(from);
    net->seqOfNeighbours[from].insert(to);
    net->seqOfNeighbours[to].insert(from);
  }
}

void group(Network* net, unsigned groupSize)
{
  Set set(net->numNodes);
  Set reach(net->numNodes);
  Set done(net->numNodes);

  for (unsigned i = 0; i < net->numNodes; i++)
    if (! done.member(i)) {
      done.insert(i);
      printf("%i ", i);
      reach.reset();
      unsigned from = i;
      for (unsigned g = 1; g < groupSize; g++) {
        Seq<Node>* seq = &net->seqOfNeighbours[from];
        for (unsigned j = 0; j < seq->numElems; j++)
          reach.plus(&net->setOfNeighbours[seq->elems[j]]);
        set.copy(&reach);
        set.inv();
        set.minus(&done);
        int n = set.get();
        if (n < 0) {
          break;
        }
        else {
          done.insert(n);
          from = n;
          printf("%d ", n);
        }
      }
      printf("\n");
    }
}

int main(int argc, char* argv[])
{
  if (argc != 2) {
    fprintf(stderr, "Usage: map [NETWORK]\n");
    exit(EXIT_FAILURE);
  }

  Network net;
  readNetwork(argv[1], &net);
  group(&net, 4);
}
