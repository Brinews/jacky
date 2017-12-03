#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int bool;
enum { false, true };
struct pageEntry {
    int frame;          // frame number
    bool referenced;    // referenced flag
    bool modified;      // modified flag
    bool present;       // present flag
    int tick;           // loaded time
    int counter;        // ref count
};

#define NUM_PAGES   8
#define NUM_FRAMES  4
#define SEED        2017

struct pageEntry pageTable[NUM_PAGES];

int count;      // page frames occupied
int nFaults;    // number of occupied frames
int nMemRef;    // page references
int nDiskRef;   // disk references
int page_frames[NUM_FRAMES];    // virtual-physical mapping
int ticker;

void rnd(int page, int modified) {
    nMemRef++;
    struct pageEntry *entry = &pageTable[page];
    int frame;
    entry->referenced = true;

    if (!entry->present) { // page fault
        nFaults++;

        if (count == NUM_FRAMES) { // find the evicted page
            int pickFrame = NUM_FRAMES * (double) rand()/(RAND_MAX+1.0);

            // get the page number of the evicted page
            int evPage = page_frames[pickFrame];

            struct pageEntry *evEntry = &pageTable[evPage];

            evEntry->present = false;

            // OS may not update the other fields of the page table
            // evEntry->refereneced = false;
            //
            // prepare the frame for referenced page
            frame = pickFrame;

            // one disk reference required to write the evited dirty page
            if (evEntry->modified)
                nDiskRef++;

        } else { // Available unoccupied page frame
            frame = count++; // FIFO allocate frame to referenced page
        }

        nDiskRef++;

        // update the page table entry of the referenced page
        entry->present = true;
        entry->modified = modified;
        entry->frame = frame;

        // map the virtual page onto the physical page frame
        page_frames[frame] = page;
    } else { // HIT
        // the dirty page is still dirty
        entry->modified = entry->modified || modified;
    }
}

void nru(int page, int modified) {
    nMemRef++;
    struct pageEntry *entry = &pageTable[page];
    int frame;
    entry->referenced = true;

    if (!entry->present) { // page fault
        nFaults++;

        if (count == NUM_FRAMES) { // find the evicted page
            //int pickFrame = NUM_FRAMES * (double) rand()/(RAND_MAX+1.0);
            int pickFrame = 0;
            int lowTicker = ticker+2;

            for (int i = 0; i < NUM_FRAMES; i++) {
                if (pageTable[page_frames[i]].tick < lowTicker) {
                    lowTicker = pageTable[page_frames[i]].tick;
                    pickFrame = i;
                } else if (pageTable[page_frames[i]].tick 
                        == lowTicker) {
                    if (pageTable[page_frames[i]].frame <
                            pageTable[page_frames[pickFrame]].frame) {
                        pickFrame = i;
                    }
                }
            }

            // get the page number of the evicted page
            int evPage = page_frames[pickFrame];

            struct pageEntry *evEntry = &pageTable[evPage];

            evEntry->present = false;

            // OS may not update the other fields of the page table
            // evEntry->refereneced = false;
            //
            // prepare the frame for referenced page
            frame = pickFrame;

            // one disk reference required to write the evited dirty page
            if (evEntry->modified)
                nDiskRef++;

        } else { // Available unoccupied page frame
            frame = count++; // FIFO allocate frame to referenced page
        }

        nDiskRef++;

        // update the page table entry of the referenced page
        entry->present = true;
        entry->modified = modified;
        entry->frame = frame;

        entry->tick = ticker;

        // map the virtual page onto the physical page frame
        page_frames[frame] = page;

    } else { // HIT
        // the dirty page is still dirty
        entry->modified = entry->modified || modified;
        entry->tick = ticker;
    }

    ticker++;
}

void lru(int page, int modified) {
    nMemRef++;
    struct pageEntry *entry = &pageTable[page];
    int frame;
    entry->referenced = true;

    if (!entry->present) { // page fault
        nFaults++;

        if (count == NUM_FRAMES) { // find the evicted page
            int pickFrame = -1;
            int lowTicker = ticker+2;

            for (int i = 0; i < NUM_FRAMES; i++) {
                if (pageTable[page_frames[i]].tick < lowTicker) {
                    if ((pickFrame == -1) || (pickFrame != -1
                                && pageTable[page_frames[i]].counter <
                                pageTable[page_frames[pickFrame]].counter))
                    lowTicker = pageTable[page_frames[i]].tick;
                    pickFrame = i;
                }
            }

            // get the page number of the evicted page
            int evPage = page_frames[pickFrame];

            struct pageEntry *evEntry = &pageTable[evPage];

            evEntry->present = false;

            // OS may not update the other fields of the page table
            // evEntry->refereneced = false;
            //
            // prepare the frame for referenced page
            frame = pickFrame;

            // one disk reference required to write the evited dirty page
            if (evEntry->modified)
                nDiskRef++;

        } else { // Available unoccupied page frame
            frame = count++; // FIFO allocate frame to referenced page
        }

        nDiskRef++;

        // update the page table entry of the referenced page
        entry->present = true;
        entry->modified = modified;
        entry->frame = frame;

        entry->tick = ticker;
        entry->counter = 1;

        // map the virtual page onto the physical page frame
        page_frames[frame] = page;

    } else { // HIT
        // the dirty page is still dirty
        entry->modified = entry->modified || modified;

        entry->tick = ticker;
        entry->counter++;
    }

    ticker++;
}

void sec(int page, int modified) {
    nMemRef++;
    struct pageEntry *entry = &pageTable[page];
    int frame;
    entry->referenced = true;

    if (!entry->present) { // page fault
        nFaults++;

        if (count == NUM_FRAMES) { // find the evicted page
            //int pickFrame = NUM_FRAMES * (double) rand()/(RAND_MAX+1.0);
            int pickFrame = -1;
            int lowTicker = -1;

            for (int i = 0; i < NUM_FRAMES; i++) {
                if (pageTable[page_frames[i]].tick > lowTicker && 
                    pageTable[page_frames[i]].referenced == false) {

                    lowTicker = pageTable[page_frames[i]].tick;
                    pickFrame = i;
                }
                pageTable[page_frames[i]].referenced = false;
            }

            if (pickFrame == -1) { // second chance
                for (int i = 0; i < NUM_FRAMES; i++) {
                    if (pageTable[page_frames[i]].tick > lowTicker && 
                            pageTable[page_frames[i]].referenced == false) {

                        lowTicker = pageTable[page_frames[i]].tick;
                        pickFrame = i;
                    }
                }
            }

            // get the page number of the evicted page
            int evPage = page_frames[pickFrame];

            struct pageEntry *evEntry = &pageTable[evPage];

            evEntry->present = false;

            // OS may not update the other fields of the page table
            // evEntry->refereneced = false;
            //
            // prepare the frame for referenced page
            frame = pickFrame;

            // one disk reference required to write the evited dirty page
            if (evEntry->modified)
                nDiskRef++;

        } else { // Available unoccupied page frame
            frame = count++; // FIFO allocate frame to referenced page
        }

        nDiskRef++;

        // update the page table entry of the referenced page
        entry->present = true;
        entry->modified = modified;
        entry->frame = frame;

        entry->tick = ticker;

        // map the virtual page onto the physical page frame
        page_frames[frame] = page;

    } else { // HIT
        // the dirty page is still dirty
        entry->modified = entry->modified || modified;
    }

    ticker++;
}

void fif(int page, int modified) {
    nMemRef++;
    struct pageEntry *entry = &pageTable[page];
    int frame;
    entry->referenced = true;

    if (!entry->present) { // page fault
        nFaults++;

        if (count == NUM_FRAMES) { // find the evicted page
            //int pickFrame = NUM_FRAMES * (double) rand()/(RAND_MAX+1.0);
            int pickFrame = 0;
            int lowTicker = ticker+2;

            for (int i = 0; i < NUM_FRAMES; i++) {
                if (pageTable[page_frames[i]].tick < lowTicker) {
                    lowTicker = pageTable[page_frames[i]].tick;
                    pickFrame = i;
                }
            }

            // get the page number of the evicted page
            int evPage = page_frames[pickFrame];

            struct pageEntry *evEntry = &pageTable[evPage];

            evEntry->present = false;

            // OS may not update the other fields of the page table
            // evEntry->refereneced = false;
            //
            // prepare the frame for referenced page
            frame = pickFrame;

            // one disk reference required to write the evited dirty page
            if (evEntry->modified)
                nDiskRef++;

        } else { // Available unoccupied page frame
            frame = count++; // FIFO allocate frame to referenced page
        }

        nDiskRef++;

        // update the page table entry of the referenced page
        entry->present = true;
        entry->modified = modified;
        entry->frame = frame;

        entry->tick = ticker;

        // map the virtual page onto the physical page frame
        page_frames[frame] = page;

    } else { // HIT
        // the dirty page is still dirty
        entry->modified = entry->modified || modified;
    }

    ticker++;
}




void initSetting()
{
    nMemRef = 0;
    nFaults = 0;
    nDiskRef = 0;
    count = 0;
    ticker = 0;

    memset(pageTable, 0, sizeof(struct pageEntry)*NUM_PAGES);
}

void rnd_simulate(char *fRefPages, char *fReport) {
    FILE *fpRefPages = fopen(fRefPages, "r");
    FILE *fpReport = fopen(fReport, "a+");
    int page, modified;

    initSetting();

    srand(SEED);
    while (fscanf(fpRefPages, "%d %d", &page, &modified) != EOF)
        rnd(page, modified);

    fprintf(fpReport, "RND M %d F %d D %d L %d %d %d %d \n", 
            nMemRef, nFaults, nDiskRef, page_frames[0], page_frames[1],
            page_frames[2], page_frames[3]);

    fclose(fpRefPages);
    fclose(fpReport);
}

void nru_simulate(char *fRefPages, char *fReport) {
    FILE *fpRefPages = fopen(fRefPages, "r");
    FILE *fpReport = fopen(fReport, "a+");
    int page, modified;

    initSetting();

    while (fscanf(fpRefPages, "%d %d", &page, &modified) != EOF)
        nru(page, modified);

    fprintf(fpReport, "NRU M %d F %d D %d L %d %d %d %d \n", 
            nMemRef, nFaults, nDiskRef, page_frames[0], page_frames[1],
            page_frames[2], page_frames[3]);

    fclose(fpRefPages);
    fclose(fpReport);
}

void lru_simulate(char *fRefPages, char *fReport) {
    FILE *fpRefPages = fopen(fRefPages, "r");
    FILE *fpReport = fopen(fReport, "a+");
    int page, modified;

    initSetting();

    srand(SEED);
    while (fscanf(fpRefPages, "%d %d", &page, &modified) != EOF)
        lru(page, modified);

    fprintf(fpReport, "LRU M %d F %d D %d L %d %d %d %d \n", 
            nMemRef, nFaults, nDiskRef, page_frames[0], page_frames[1],
            page_frames[2], page_frames[3]);

    fclose(fpRefPages);
    fclose(fpReport);
}

void sec_simulate(char *fRefPages, char *fReport) {
    FILE *fpRefPages = fopen(fRefPages, "r");
    FILE *fpReport = fopen(fReport, "a+");
    int page, modified;

    initSetting();

    srand(SEED);
    while (fscanf(fpRefPages, "%d %d", &page, &modified) != EOF)
        sec(page, modified);

    fprintf(fpReport, "SEC M %d F %d D %d L %d %d %d %d \n", 
            nMemRef, nFaults, nDiskRef, page_frames[0], page_frames[1],
            page_frames[2], page_frames[3]);

    fclose(fpRefPages);
    fclose(fpReport);
}

void clk_simulate(char *fRefPages, char *fReport) {
    FILE *fpRefPages = fopen(fRefPages, "r");
    FILE *fpReport = fopen(fReport, "a+");
    int page, modified;

    initSetting();

    srand(SEED);
    while (fscanf(fpRefPages, "%d %d", &page, &modified) != EOF)
        sec(page, modified);

    fprintf(fpReport, "CLK M %d F %d D %d L %d %d %d %d \n", 
            nMemRef, nFaults, nDiskRef, page_frames[0], page_frames[1],
            page_frames[2], page_frames[3]);

    fclose(fpRefPages);
    fclose(fpReport);
}

void fif_simulate(char *fRefPages, char *fReport) {
    FILE *fpRefPages = fopen(fRefPages, "r");
    FILE *fpReport = fopen(fReport, "a+");
    int page, modified;

    initSetting();

    while (fscanf(fpRefPages, "%d %d", &page, &modified) != EOF)
        fif(page, modified);

    fprintf(fpReport, "FIF M %d F %d D %d L %d %d %d %d \n", 
            nMemRef, nFaults, nDiskRef, page_frames[0], page_frames[1],
            page_frames[2], page_frames[3]);

    fclose(fpRefPages);
    fclose(fpReport);
}


int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "%s in.txt out.txt", argv[0]);
        return 0;
    }

    nru_simulate(argv[1], argv[2]);
    rnd_simulate(argv[1], argv[2]);
    fif_simulate(argv[1], argv[2]);
    sec_simulate(argv[1], argv[2]);
    clk_simulate(argv[1], argv[2]);
    lru_simulate(argv[1], argv[2]);

    return 0;
}
