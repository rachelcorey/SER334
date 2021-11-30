//
// Created by kirry on 11/30/21.
//

#include <PageTable.h>


struct page_table_entry {
    unsigned int bit;
};
struct page_table {
    struct page_table_entry *entries;
    int page_count;
    int frame_count;
    enum replacement_algorithm algorithm;
    int verbose;
};


/**
 * Creates a new page table object. Returns a pointer to created page table.
 *
 * @param page_count Number of pages.
 * @param frame_count Numbers of frames.
 * @param algorithm Page replacement algorithm
 * @param verbose Enables showing verbose table contents.
 * @return A page table object.
 */
struct page_table* page_table_create(int page_count, int frame_count, enum replacement_algorithm algorithm, int verbose) {
    struct page_table* pT = malloc(sizeof (struct page_table));
    pT->entries = malloc(sizeof(struct page_table_entry) * page_count);
    pT->page_count = page_count;
    pT->frame_count = frame_count;
    pT->algorithm = algorithm;
    pT->verbose = verbose;
    return pT;
}

/**
 * Destorys an existing page table object. Sets outside variable to NULL.
 *
 * @param pt A page table object.
 */
void page_table_destroy(struct page_table** pt) {

}

/**
 * Simulates an instruction accessing a particular page in the page table.
 *
 * @param pt A page table object.
 * @param page The page being accessed.
 */
void page_table_access_page(struct page_table *pt, int page) {

}

/**
 * Displays page table replacement algorithm, number of page faults, and the
 * current contents of the page table.
 *
 * @param pt A page table object.
 */
void page_table_display(struct page_table* pt) {

}

/**
 * Displays the current contents of the page table.
 *
 * @param pt A page table object.
 */
void page_table_display_contents(struct page_table *pt) {

}


