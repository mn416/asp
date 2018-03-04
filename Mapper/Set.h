#ifndef _SET_H_
#define _SET_H_

// Bit-vector set implementation
struct Set {
  unsigned numElems;
  unsigned numChunks;
  uint64_t* chunks;

  void insert(unsigned n)
  {
    chunks[n/64] |= 1ul << (n%64);
  }

  void remove(unsigned n)
  {
    chunks[n/64] &= ~(1ul << (n%64));
  }

  bool member(unsigned n)
  {
    return (chunks[n/64] & (1ul << (n%64))) != 0ul;
  }

  void reset()
  {
    for (unsigned i = 0; i < numChunks; i++) chunks[i] = 0;
  }

  void plus(Set* set)
  {
    assert(set->numChunks == numChunks);
    for (unsigned i = 0; i < numChunks; i++) chunks[i] |= set->chunks[i];
  }

  void minus(Set* set)
  {
    assert(set->numChunks == numChunks);
    for (unsigned i = 0; i < numChunks; i++)
      chunks[i] = chunks[i] & ~set->chunks[i];
  }

  void inv()
  {
    for (unsigned i = 0; i < numChunks; i++) chunks[i] = ~chunks[i];
  }

  void copy(Set* set)
  {
    assert(set->numChunks == numChunks);
    for (unsigned i = 0; i < numChunks; i++) chunks[i] = set->chunks[i];
  }

  int get()
  {
    for (unsigned i = 0; i < numChunks; i++) {
      if (chunks[i]) {
        uint64_t x = chunks[i];
        int count = i*64;
        while ((x&1) == 0ul) {
          x >>= 1;
          count++;
        }
        return count >= numElems ? -1 : count;
      }
    }
    return -1;
  }

  void alloc(unsigned n)
  {
    numElems = n;
    numChunks = (n + 63) / 64;
    chunks = new uint64_t [numChunks];
    reset();
  }

  Set()
  {
    numElems = 0;
    numChunks = 0;
    chunks = NULL;
  }

  Set(unsigned n)
  {
    alloc(n);
  } 

  ~Set()
  {
    if (chunks) delete [] chunks;
  }
};

#endif
