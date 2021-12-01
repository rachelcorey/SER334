/**
 * Program to simulate contents of page table with respect to a reference string
 * and a page replacement algorithm.
 *
 * Completion time: 4 hours
 *
 * @author Acuna, Rachel Corey
 * @version 1.0
 */
#include <stdio.h>
#include <string.h>
#include "CoreyDataLoader.c"
#include "PageTable.c"

int main(int argc, char* argv[]) {
    char* filename = argv[1];

    struct test_scenario* data = load_test_data(filename);
    struct page_table* pt_fifo = page_table_create(data->page_count, data->frame_count, FIFO, 0);
    struct page_table* pt_lru = page_table_create(data->page_count, data->frame_count, LRU, 0);
    struct page_table* pt_mfu = page_table_create(data->page_count, data->frame_count, MFU, 0);

    //simulate page requests: fifo
    for (int i = 0; i < data->refstr_len; i++) {
        page_table_access_page(pt_fifo, data->refstr[i]);
        //page_table_display_contents(pt_fifo);
    }
    page_table_display(pt_fifo);

    //simulate page requests: lru
    for (int i = 0; i < data->refstr_len; i++) {
        page_table_access_page(pt_lru, data->refstr[i]);
        //page_table_display_contents(pt_lru);
    }
    page_table_display(pt_lru);

    //simulate page requests: mfu
    for (int i = 0; i < data->refstr_len; i++) {
        page_table_access_page(pt_mfu, data->refstr[i]);
        //page_table_display_contents(pt_mfu);
    }
    page_table_display(pt_mfu);

    page_table_destroy(&pt_fifo);
    page_table_destroy(&pt_lru);
    page_table_destroy(&pt_mfu);
    free(data);
}