#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int powTwo(int a)
{
    if (a == 1) return 0;
    if (a <= 0) return -1;
    int i = 2, j = 1;
    while (i < a)
    {
        i *= 2;
		j++;
    }
    if (i == a) return j;
	return -1;
}

unsigned long power(int a, int b)
{
	if (b <= 0)
	{
		return 1;
	}
	unsigned long result = a;
	int i = 1;
	while (i < b && i <= 48)
	{
		result *= a;
		i++;
	}
	return result;
}

int main(int argc, char** argv)
{
    if (argc != 6)
    {
        printf("error\n");
        return 0;
    }

    int cacheSize = atoi(argv[1]);
    if (powTwo(cacheSize) == -1)
    {
        printf("error\n");
        return 0;
    }

    char* assoc = argv[2];
    if (strcmp(assoc, "direct") != 0 && strcmp(assoc, "assoc") != 0 && (strlen(assoc) <= 6 || assoc[0] != 'a' || assoc[1] != 's' || assoc[2] != 's' || assoc[3] != 'o' || assoc[4] != 'c' || assoc[5] != ':'))
    {
        printf("error\n");
        return 0;
    }
    

    char* policy = argv[3];
    if (strcmp(policy, "lru") != 0)
    {
        printf("error\n");
        return 0;
    }

    int blockSize = atoi(argv[4]);
    if (powTwo(blockSize) == -1 || blockSize > cacheSize)
    {
        printf("error\n");
        return 0;
    }

    FILE* srcFile = fopen(argv[5], "r");
    if (srcFile == NULL)
    {
        printf("error\n");
        return 0;
    }

    

    int S = 1, E = 1, B = blockSize;
    if (strcmp(assoc, "direct") == 0)
    {
        E = 1;
        S = cacheSize / blockSize;
    } else if (strcmp(assoc, "assoc") == 0)
    {
        E = cacheSize / blockSize;
        S = 1;
    } else if (sscanf(assoc, "assoc:%d", &E) == 1)
    {
        S = (cacheSize / B) / E;
        if (powTwo(E) == -1)
        {
        	printf("error\n");
        	free(srcFile);
        	return 0;
        }
    } else
    {
        printf("error\n");
        free(srcFile);
		return 0;
    }
    
    char mode;
	int lineNum = 1;
    unsigned long int instruction, slot = 0;
    int i = fscanf(srcFile, "\n%lx: %c %lx", &instruction, &mode, &slot);
    if (i != 3)
    {
        printf("error\n");
        free(srcFile);
        return 0;
    }
	//E is lines per set, B is the block size in bytes, S is the number of sets
	int b = powTwo(B), s = powTwo(S);
	int t = (B * 8) - b - s;

    int pmr = 0, pmw = 0, phits = 0, pmisses = 0, nmr = 0, nmw = 0, nhits = 0, nmisses = 0;

	//Allocate memory for both caches, prefetch and without
	unsigned long** precache = (unsigned long** ) malloc(sizeof(unsigned long* ) * S); 
	unsigned long** noncache = (unsigned long** ) malloc(sizeof(unsigned long* ) * S);
	//Allocate memory for each set
	int x = 0;
	for (x = 0; x < S; x++)
	{
		precache[x] = (unsigned long* ) calloc(E, sizeof(long int));
		noncache[x] = (unsigned long* ) calloc(E, sizeof(long int));
	}
	//Could use short ints attached to each other if this becomesa problem
	
	

	//Now we need to implement a data structure for valid bits
	unsigned short** prevalid = (unsigned short** ) malloc(sizeof(unsigned short* ) * S);
	unsigned short** nonvalid = (unsigned short** ) malloc(sizeof(unsigned short* ) * S);
	for (x = 0; x < S; x++)
	{
		prevalid[x] = (unsigned short* ) calloc(E, sizeof(unsigned short));
		nonvalid[x] = (unsigned short* ) calloc(E, sizeof(unsigned short));
	}
	
	//Now we need to implement a data structure for lru
	//lru is going to be for each individual set, it's going to just be an E size array of integers, dynamically allocated
	//The front will be the least recently used, things will be added to the back
	//Accessing the lru will be O(1)
	unsigned long** prelru = (unsigned long** ) malloc(sizeof(unsigned long* ) * S);
	unsigned long** nonlru = (unsigned long** ) malloc(sizeof(unsigned long* ) * S);
	for (x = 0; x < S; x++)
	{
		nonlru[x] = calloc(E, sizeof(unsigned long));
		prelru[x] = calloc(E, sizeof(unsigned long));
	}
	
	//Now input will be read from file
	

	unsigned long tag, setindex, blockoffset;
	
	
    while (i == 3)
    {
        if (mode != 'R' && mode != 'W') //Input checking
        {
            printf("error\n");
            free(srcFile);
			for (x = 0; x < S; x++)
			{
				free(prelru[x]);
				free(precache[x]);
				free(prevalid[x]);
				free(noncache[x]);
				free(nonlru[x]);
				free(nonvalid[x]);
			}
			free(prelru);
			free(precache);
			free(prevalid);
			free(noncache);
			free(nonlru);
			free(nonvalid);
            return 0;
        }
        
        //save the tag, block offset, and set index
        blockoffset = (slot) & (power(2, b) - 1);
	setindex = (slot >> b) & (power(2, s) - 1);
	tag = ((slot >> b) >> s) & (power(2, t) - 1);
		
	//If it is a hit, hit will become 1, 0 otherwise
	int hit;
		
		
        //Without Prefetch
		//Search through cache
		hit = 0;
		
		
		for (x = 0; x < E; x++)
		{
			if (*(nonvalid[setindex] + x) == 1)
			{
				if ((((*(noncache[setindex] + x) >> b) >> s) & (power(2, t) - 1)) == tag)
				{
					hit = 1;
					break;
				}
			} else
			{
				//cold miss, empty slot
				break;
			}
		}
		//if not found, miss
		//if miss, replace the back of the lru queue with it
		//if write miss, add one memory read
		//if hit, update lru queue, do nothing else
		
		if (!hit)
		{
			if (*(nonvalid[setindex]) == 0)
			{
				*(nonlru[setindex]) = tag;
				*(nonvalid[setindex]) = 1;
				*(noncache[setindex]) = ((((tag) << s) | setindex) << b) | blockoffset;
			} else if (E == 1)
			{
				*(nonlru[setindex]) = tag;
				*(noncache[setindex]) = ((((tag) << s) | setindex) << b) | blockoffset;
			} else
			{
				//replacement
				int full = 1;
				for (x = 0; x < E; x++)
				{
					if (*(nonvalid[setindex] + x) == 0)
					{
						full = 0;
						break;
					}
				}
				if (full)
				{
					//take out the lru, then move everything back, and then insert at the end
					unsigned long lru = *(nonlru[setindex]);
					for (x = 1; x < E; x++)
					{
						*(nonlru[setindex] + x - 1) = *(nonlru[setindex] + x);
					}
					*(nonlru[setindex] + E - 1) = tag;
					//find the lru in the set and replace it
					for (x = 0; x < E; x++)
					{
						if ((((*(noncache[setindex] + x) >> b) >> s) & (power(2, t) - 1)) == lru)
						{
							*(noncache[setindex] + x) = ((((tag) << s) | setindex) << b) | blockoffset;
						}
					}
				} else
				{
					//find the next open slot (x) in the cache and put it in there
					//put it in the next slot in the lru
					*(nonlru[setindex] + x) = tag;
					*(nonvalid[setindex] + x) = 1;
					*(noncache[setindex] + x) = ((((tag) << s) | setindex) << b) | blockoffset;
				}
				
			}
			if (mode == 'W') 
			{
				nmw++;
				nmr++;
			}
			if (mode == 'R') 
			{
				nmr++;
			}
			nmisses++;
		} else
		{
			//if (setindex == 1) printf("hit %lu\n", tag);
			if (E > 1)
			{
				//remove the tag from the lru and insert it at the back
				for (x = 0; x < E; x++)
				{
					if (*(nonvalid[setindex] + x) && *(nonlru[setindex] + x) == tag)
					{
						while (x + 1 < E && *(nonvalid[setindex] + x + 1))
						{
							*(nonlru[setindex] + x) = *(nonlru[setindex] + x + 1);
							x++;
						}
						*(nonlru[setindex] + x) = tag;
						break;
					}
				}
			}
			if (mode == 'W')
			{
				nmw++;
			}
			//if E is 1 then do nothing, it already is the lru and mru
			
			nhits++;
		}
		
		
		
        //With prefetch
		hit = 0;
		
		
		for (x = 0; x < E; x++)
		{
			if (*(prevalid[setindex] + x) == 1)
			{
				if ((((*(precache[setindex] + x) >> b) >> s) & (power(2, t) - 1)) == tag)
				{
					hit = 1;
					break;
				}
			} else
			{
				//cold miss, empty slot
				
				break;
			}
		}
		//if not found, miss
		//if miss, replace the back of the lru queue with it
		//if write miss, add one memory read
		//if hit, update lru queue, do nothing else
		
		if (!hit)
		{
			//if (setindex == 1) printf("miss %lu\n", tag);
			if (*(prevalid[setindex]) == 0)
			{
				*(prelru[setindex]) = tag;
				*(prevalid[setindex]) = 1;
				*(precache[setindex]) = ((((tag) << s) | setindex) << b) | blockoffset;
			} else if (E == 1)
			{
				*(prelru[setindex]) = tag;
				*(precache[setindex]) = ((((tag) << s) | setindex) << b) | blockoffset;
			} else
			{
				//replacement
				int full = 1;
				for (x = 0; x < E; x++)
				{
					if (*(prevalid[setindex] + x) == 0)
					{
						full = 0;
						break;
					}
				}
				if (full)
				{
					//take out the lru, then move everything back, and then insert at the end
					unsigned long lru = *(prelru[setindex]);
					for (x = 1; x < E; x++)
					{
						*(prelru[setindex] + x - 1) = *(prelru[setindex] + x);
					}
					*(prelru[setindex] + E - 1) = tag;
					//find the lru in the set and replace it
					for (x = 0; x < E; x++)
					{
						if ((((*(precache[setindex] + x) >> b) >> s) & (power(2, t) - 1)) == lru)
						{
							*(precache[setindex] + x) = ((((tag) << s) | setindex) << b) | blockoffset;
						}
					}
				} else
				{
					//find the next open slot (x) in the cache and put it in there
					//put it in the next slot in the lru
					*(prelru[setindex] + x) = tag;
					*(prevalid[setindex] + x) = 1;
					*(precache[setindex] + x) = ((((tag) << s) | setindex) << b) | blockoffset;
				}
			}
			
			//PREFETCH---------------------------------------------------------------------------------
			int incache = 0;
			unsigned long newblock = slot + B;
			unsigned long newblockoffset = (newblock) & (power(2, b) - 1);
			unsigned long newsetindex = (newblock >> b) & (power(2, s) - 1);
			unsigned long newtag = ((newblock >> b) >> s) & (power(2, t) - 1);
			for (x = 0; x < E; x++)				{
				if (*(prevalid[newsetindex] + x) == 1)
				{
					if ((((*(precache[newsetindex] + x) >> b) >> s) & (power(2, t) - 1)) == newtag)
						{
						incache = 1;
						break;
					}
				} else
				{
					//cold miss, empty slot
					
					break;
				}
			}
			if (!incache)
			{
				pmr++;
				if (*(prevalid[newsetindex]) == 0)
				{
					*(prelru[newsetindex]) = newtag;
					*(prevalid[newsetindex]) = 1;
					*(precache[newsetindex]) = ((((newtag) << s) | newsetindex) << b) | blockoffset;
				} else if (E == 1)
				{
					*(prelru[newsetindex]) = newtag;
					*(precache[newsetindex]) = ((((newtag) << s) | newsetindex) << b) | blockoffset;
				} else
				{
					//replacement
					int full = 1;
					for (x = 0; x < E; x++)
					{
						if (*(prevalid[newsetindex] + x) == 0)
						{
							full = 0;
							break;
						}
					}
					if (full)
					{
						//take out the lru, then move everything back, and then insert at the end
						unsigned long lru = *(prelru[newsetindex]);
						for (x = 1; x < E; x++)
						{
							*(prelru[newsetindex] + x - 1) = *(prelru[newsetindex] + x);
						}
						*(prelru[newsetindex] + E - 1) = newtag;
						//find the lru in the set and replace it
						for (x = 0; x < E; x++)
						{
							if ((((*(precache[newsetindex] + x) >> b) >> s) & (power(2, t) - 1)) == lru)
							{
								*(precache[newsetindex] + x) = ((((newtag) << s) | newsetindex) << b) | newblockoffset;
							}
						}
					} else
					{
						//find the next open slot (x) in the cache and put it in there
						//put it in the next slot in the lru
						*(prelru[newsetindex] + x) = newtag;
						*(prevalid[newsetindex] + x) = 1;
						*(precache[newsetindex] + x) = ((((newtag) << s) | newsetindex) << b) | newblockoffset;
					}
				}
			}
			//----------------------------------------------------------------------------------------
			
			if (mode == 'W') 
			{
				pmw++;
				pmr++;
			}
			if (mode == 'R') 
			{
				pmr++;
			}
			pmisses++;
		} else
		{
			if (E > 1)
			{
				//remove the tag from the lru and insert it at the back
				for (x = 0; x < E; x++)
				{
					if (*(prevalid[setindex] + x) && *(prelru[setindex] + x) == tag)
					{
						while (x + 1 < E && *(prevalid[setindex] + x + 1))
						{
							*(prelru[setindex] + x) = *(prelru[setindex] + x + 1);
							x++;
						}
						*(prelru[setindex] + x) = tag;
						break;
					}
				}
			}
			if (mode == 'W')
			{
				pmw++;
			}
			
			phits++;
		}
        i = fscanf(srcFile, "\n%lx: %c %lx", &instruction, &mode, &slot);
        lineNum++;
    }

    char* line = (char* ) malloc(sizeof(char) * 10);
    fscanf(srcFile, "%s", line);
    if (!strcmp(line, "#eof")){
        printf("no-prefetch\n");
        printf("Memory reads: %d\n", nmr);
        printf("Memory writes: %d\n", nmw);
        printf("Cache hits: %d\n", nhits);
        printf("Cache misses: %d\n", nmisses);
        printf("with-prefetch\n");
        printf("Memory reads: %d\n", pmr);
        printf("Memory writes: %d\n", pmw);
        printf("Cache hits: %d\n", phits);
        printf("Cache misses: %d\n", pmisses);
    } else
    {
        printf("error\n");
    }

	
	free(srcFile);
	free(line);
	for (x = 0; x < S; x++)
	{
		free(prelru[x]);
		free(precache[x]);
		free(prevalid[x]);
		free(noncache[x]);
		free(nonlru[x]);
		free(nonvalid[x]);
	}
	free(prelru);
	free(precache);
	free(prevalid);
	free(noncache);
	free(nonlru);
	free(nonvalid);
    return 0;
}
