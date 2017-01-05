#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// A line (or row) representation that stores the tag 
// and whether it's valid
struct Line {
  int tag;
  int valid;
};

// Constructor for a Line
// defaults tag and valid to 0
struct Line newLine() {
  struct Line line;
  line.tag = 0;
  line.valid = 0;
  return line;
}

// A Set representation that stores
// its index number,
// the number of lines within the set,
// the current eviction line,
// and an array of Lines containing all the Lines within that Set.
struct Set {
  int index;
  int numLines;
  int evictionLine;
  struct Line* lineArray;
};

// Constructor for a Set
// requires index and number of lines
// start evicition line at 0
// generates required Lines and stores them in an array
struct Set newSet(int index, int numLines) {
  struct Set s;
  s.index = index;
  s.numLines = numLines;
  s.evictionLine = 0;

  // Allocate memory based off give number of Lines within Wet
  s.lineArray = malloc(numLines * 8);
  
  // Create as many Lines as necessary
  for (int i = 0; i < numLines; i++) {
    s.lineArray[i] = newLine();
  }
  
  return s;

}

// A Cache representation that stores:
// the Cache size (ex. 128-byte or 64-byte),
// the size of the blocks (ex. 8-byte or 16-byte),
// the numer of Lines per Set (associativity)
// and an array of all Sets within the Cache
struct Cache{
  int size;
  int blockSize;
  int numLines;
  struct Set* setArray;
};

// Constructor for Cache
// requires Cache size, block size, and number of lines per set
// generates require Sets and stores them in an array
struct Cache newCache(int size, int blockSize, int numLines) {
  struct Cache c;
  c.size = size;
  c.blockSize = blockSize;
  c.numLines = numLines;
  
  // Allocate memory based off give number of Lines within Wet
  c.setArray = malloc(setArraySize(c) * 16);
  
    // Create as many Sets as necessary
  for (int i = 0; i < setArraySize(c); i++) {
    c.setArray[i] = newSet(i, numLines);
  }

  return c;
}

// A Address representation that stores a tag 
// and index
struct Address{
  int tag;
  int index;
};

// Constructor for Address
// Calculates values for tag and index based off given Cache
// and an initial address (int)
struct Address newAddress(struct Cache cache, int address ) {
  struct Address a;
  a.tag = address / (cache.size / cache.numLines);
  a.index = address / cache.blockSize % setArraySize(cache);
  return a;
}

// Calculates how large the Set array of a Cache should be
int setArraySize(struct Cache cache) {
  return cache.size / (cache.numLines * cache.blockSize);
}

// Prints the given Cache
void printCache(struct Cache cache) {
  printf("\n Final State of Cache: \n");

  for (int i = 0; i < setArraySize(cache); i++) {
    struct Set set = cache.setArray[i];
    
    for (int i = 0; i < set.numLines; i++) {
        printf("Index %d: ", set.index);
        printf("\t Tag: %d \t Valid: %d \n", set.lineArray[i].tag, set.lineArray[i].valid);
    }
  }
}

//Prints for each address whether 
void hitOrMiss(struct Cache cache, struct Address address ) {
  
  //loop over Set array
  for (int i = 0; i < setArraySize(cache); i++) {
    
    // if current Set array index = given Address index
    if (cache.setArray[i].index == address.index) {

      // loop over the Line array within that specific Set
      for (int n = 0; n < cache.setArray[i].numLines; n++) {
        
        // check if tags match and Line is valid
        if (cache.setArray[i].lineArray[n].tag == address.tag &&
            cache.setArray[i].lineArray[n].valid) {
          printf("HIT\n");
          return;
        }
        // if line isn't valid
        if (!cache.setArray[i].lineArray[n].valid) {
          cache.setArray[i].lineArray[n].tag = address.tag;
          cache.setArray[i].lineArray[n].valid = 1;
          printf("MISS\n");
          return;
        }
      }

      // else, everything was valid but no tags matched
      cache.setArray[i].lineArray[cache.setArray[i].evictionLine].tag = address.tag;
      printf("MISS Evicting Line %d\n", cache.setArray[i].evictionLine);

      // increment eviction line
      cache.setArray[i].evictionLine = (cache.setArray[i].evictionLine + 1) % cache.setArray[i].numLines;
    }
  }
}

int main(int argc, char *argv[]) {

  //test data, vertical for readibility
  int addressArray[] = {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 
        76, 80, 0, 4, 8, 12, 16, 71, 3, 41, 81, 39, 38, 71, 15, 39, 11, 51, 57, 41};

  /* For each Test:
   *
   * Create a new Cache with the given tests parameters
   *
   * Load each address array item into the Cache
   *
   * Print each given Address's tag and index within the Cache
   *
   * Print whether the given Address was a hit or a miss
   *
   * Print the final state of the Cache
  */

  printf("\n TEST RUN 1 \n Cache Size: \t \t 128-byte \n Cache Block Size: \t 8-byte \n Lines Per Set: \t 16 (direct-mapped) \n");
  struct Cache test1 = newCache(128, 8, 16);
  for (int i = 0; i < (39); i++) {
      struct Address a = newAddress(test1, addressArray[i]);
      printf("%d :", addressArray[i]);
      hitOrMiss(test1, a);
  }

  printf("\n TEST RUN 2 \n Cache Size: \t \t 64-byte \n Cache Block Size: \t 8-byte \n Lines Per Set: \t 2 (2-way associative) \n");
  struct Cache test2 = newCache(64, 8, 2);
  for (int i = 0; i < 39; i++) {
      struct Address a = newAddress(test2, addressArray[i]);
      printf("Address: %d \t Tag: %d \t Index: %d \t", addressArray[i], a.tag, a.index);
      hitOrMiss(test2, a);
  }
  printCache(test2);

  printf("\n TEST RUN 3 \n Cache Size: \t \t 128-byte \n Cache Block Size: \t 16-byte \n Lines Per Set: \t 8 (direct-mapped) \n");
  struct Cache test3 = newCache(128, 16, 8);
  for (int i = 0; i < 39; i++) {
      struct Address a = newAddress(test3, addressArray[i]);
      printf("Address: %d \t Tag: %d \t Index: %d \t", addressArray[i], a.tag, a.index);
      hitOrMiss(test3, a);
  }
  printCache(test3);

  printf("\n TEST RUN 4 \n Cache Size: \t \t 64-byte \n Cache Block Size: \t 8-byte \n Lines Per Set: \t 1 (fully associative) \n");
  struct Cache test4 = newCache(64, 8, 1);
  for (int i = 0; i < 39; i++) {
      struct Address a = newAddress(test4, addressArray[i]);
      printf("Address: %d \t Tag: %d \t Index: %d \t", addressArray[i], a.tag, a.index);
      hitOrMiss(test4, a);
  }
  printCache(test4);

  exit;
}