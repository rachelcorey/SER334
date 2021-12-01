
#include <PageTable.h>


struct page_table_entry {
    unsigned int bit;
    int frame;
};
struct page_table {
    struct page_table_entry *pages;
    int page_count;
    int frames_occupied;
    int frame_count;
    enum replacement_algorithm algorithm;
    int verbose;
    int page_faults;
};


/**
 * Creates a new page table object. Returns a pointer to created page table.
 *
 * @param page_count Number of pages.
 * @param frame_count Numbers of pages.
 * @param algorithm Page replacement algorithm
 * @param verbose Enables showing verbose table contents.
 * @return A page table object.
 */
struct page_table* page_table_create(int page_count, int frame_count, enum replacement_algorithm algorithm, int verbose) {
    struct page_table* pT = malloc(sizeof (struct page_table));
    pT->pages = malloc(sizeof(struct page_table_entry) * page_count);

    pT->frames_occupied = 0;
    pT->page_count = page_count;
    pT->frame_count = frame_count;
    pT->algorithm = algorithm;
    pT->verbose = verbose;
    pT->page_faults = 0;
    return pT;
}

/**
 * Destorys an existing page table object. Sets outside variable to NULL.
 *
 * @param pt A page table object.
 */
void page_table_destroy(struct page_table** pt) {
    free(pt[0]->pages);
    free(pt[0]);
}

int pageExists(struct page_table *pt, int page) {
    int e = 0;
    if (pt->pages[page].bit == 101) {
        e = 1;
    }
    return e;
}

void accFIFO(struct page_table *pt, int page) {

    if (!pageExists(pt, page)) {
        for (int i = 0; i < pt->page_count; ++i) {
            pt->pages[i].frame -= 1;
            if (pt->pages[i].frame == -1) {
                pt->pages[i].bit = 100;
                pt->pages[i].frame = i;
            }
        }
        pt->pages[page].frame = pt->frame_count - 1;
        pt->pages[page].bit = 101;
        ++pt->page_faults;
    }


}


void accLRU(struct page_table *pt, int page) {

    if (!pageExists(pt, page)) {

        ++pt->page_faults;
    }

}


void accMFU(struct page_table *pt, int page) {

    if (!pageExists(pt, page)) {

        ++pt->page_faults;
    }
}

/**
 * Simulates an instruction accessing a particular page in the page table.
 *
 * @param pt A page table object.
 * @param page The page being accessed.
 */
void page_table_access_page(struct page_table *pt, int page) {
    if (pt->frames_occupied == pt->frame_count) {
        switch (pt->algorithm) {
            case (FIFO):
                accFIFO(pt, page);
                break;
            case (LRU):
                accLRU(pt, page);
                break;
            case (MFU):
                accMFU(pt, page);
                break;
            default:
                break;
        }
    } else {
        pt->pages[page].frame = pt->frames_occupied;
        pt->pages[page].bit = 101;
        ++pt->frames_occupied;
        ++pt->page_faults;
    }
}

void print_enum(int val) {

    switch (val) {
        case 0:
            printf("FIFO\n");
            break;
        case 1:
            printf("LRU\n");
            break;
        case 2:
            printf("MRU\n");
            break;
    }

}

/**
 * Displays page table replacement algorithm, number of page faults, and the
 * current contents of the page table.
 *
 * @param pt A page table object.
 */
void page_table_display(struct page_table* pt) {
    printf("====Page Table====\n");
    printf("Mode: ");
    print_enum(pt->algorithm);
    printf("Page Faults: %d\n", pt->page_faults);
    page_table_display_contents(pt);
}

char byDigit(int x, int n)
{
    while (n--) {
        x /= 10;
    }
    char c = (x % 10) + '0';
    return c;
}

/**
 * Displays the current contents of the page table.
 *
 * @param pt A page table object.
 */
void page_table_display_contents(struct page_table *pt) {
    printf("page    frame  |  dirty    valid  |\n");

    for (int i = 0; i < pt->page_count; ++i) {
        int bit = pt->pages[i].bit;
        printf("   %d", i);
        printf("        %d  |", pt->pages[i].frame);
        char dirty = byDigit(bit, 1);
        printf("      %c", dirty);
        char valid = byDigit(bit, 0);
        printf("        %c  |", valid);
        printf("\n");
    }
}


