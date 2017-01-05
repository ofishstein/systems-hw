#include <stdio.h>
#include <math.h>


// enum for hits and misses
#define MISS 0
#define HIT  1

#define MAX_CACHE_LINES 32
#define MAX_ROWS 256
#define MAX_SETS 32

typedef struct {
    int valid;
    int tag;
    int numCacheLines;
    int data[MAX_CACHE_LINES];
} Row;

typedef struct {
    int numRows;
    Row rows[MAX_ROWS];
} Set;

typedef struct {
    int s;      // size of cache
    int n;      // number of sets
    int l;      // bytes per line
    int mode;   // mode of cache
    Set sets[MAX_SETS];
} Cache;

Cache buildCache(int s, int l, int n, int mode)
{
    Cache cache = {s,n,l, mode};

    for (int x = 0; x < n; x++) {
        int rows = s / ( n * l );
        Set set = {rows};
        for (int z = 0; z < set.numRows; z++) {
            Row row = {0, 0, -1, l};
            set.rows[z] = row; 
        }
        cache.sets[x] = set;
    }

    return cache;
}

int visit(Set *set, int tag)
{
    int low_i = set->numRows - 1;

    for (int x = 0; x <= set->numRows; x++) {
        
        // get row
        Row *row = &set->rows[x];
        
        // If the tag is found and valid
        if (row->tag == tag && row->valid) {
            return HIT;
        }
        
        // If the tag is not found but there is an empty slot
        if (!row->valid) {
            row->valid = 1;
            row->tag = tag;
            return MISS;
        }
        
        // If the tag is not found and set is full, evict lowest version
        if (x == set->numRows - 1) {
            Row *rowl = &set->rows[low_i];
            rowl->tag = tag;
            return MISS;
        } 
    }
    
    return MISS;
}

void visitAddresses(Cache *cache, int len, int adrs[])
{
    if (cache->mode == 0) 
    {
        printf("Set Associative Cache: (%i, %i, %i) \n",cache->s,cache->l,cache->n);
    }
    else if (cache->mode == 1)
    {
        printf("Direct Mapped Cache: (%i, %i) \n", cache->s ,cache->l);
    } 
    else 
    {
        printf("Fully Associative Cache: (%i, %i) \n",cache->s,cache->l);
    }
    
    // loop through addresses and visit each
    for (int x = 0; x < len; x++) {
        
        // get address
        int adr = adrs[x];

        // compute tag, index, offset
        int offset, index, tag;
        tag = adr / (cache->s / cache->n);

        if (cache->mode == 2)
        {
            index = 0;
        }
        else
        {
            index = adr / (cache->l);
        }
        
        offset = adr % cache->l;
        
        // get set
        Set *set = &cache->sets[index];
        
        // test for hit
        int hit = visit(set, tag);
        
        // log to console
        printf("%i\t: %s\t- %d %d %d\n", adrs[x], hit ? "HIT" : "MISS", tag, index, offset);
    }
    
}

int main(int argc, char *argv[])
{

    static int len = 39;
    static int test_set[] = {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 
        76, 80, 0, 4, 8, 12, 16, 71, 3, 41, 81, 39, 38, 71, 15, 39, 11, 51, 57, 41};

    int s, l, n, mode;

    printf("Cache total size (in bytes)?: ");
    scanf("%i", &s);

    printf("Bytes per block?: ");
    scanf("%i", &l);

    printf("Mode (1 for Direct Mapped, 2 for Fully Associative, Else Set-Associative): ");
    scanf("%i", &mode);


    if (mode == 1)
    {
        n = 1;
    }
    else if (mode == 2)
    {
        n = s / l;
    }
    else 
    {
        mode = 0;
        printf("Number of sets: ");
        scanf("%i", &n);
    }
    
    Cache cache = buildCache(s, l, n, mode);
    
    visitAddresses(&cache, len, test_set);
    
    return 0;
}