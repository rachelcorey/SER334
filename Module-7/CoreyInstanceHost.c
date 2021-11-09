#include <malloc.h>
#include <stdio.h>
#include <linux/unistd.h>
#include <math.h>
#include <unistd.h>
#include <asm/unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include "CoreyInstanceHost.h"
#include "CoreyLoadBalancer.h"


struct host {
    int instances;
    pthread_t *thread;
};


pthread_t *create_instance(host *h) {
    return malloc(sizeof(pthread_t) * sizeof(h));
}

/**
* Initializes the host environment.
*/
host *host_create() {
    host *host = (struct host*) malloc(sizeof(struct host) + 1);
    host->instances = 0;
    host->thread = create_instance(host);
    return host;
}

/**
* Shuts down the host environment. Ensures any outstanding batches have
* completed.
*/
void host_destroy(host** host) {

//    do {
//        int result = pow(cur->data, 2);
//        cur->data_result = &result;
//        cur = cur->next;
//    }
//    while (cur->next != NULL);
    free(*host);
    *host = NULL;
}

void * instance_process_data(void *batch) {
    struct job_node *cur = batch;
    printf("data: %d\n", cur->data);
    void *ptr = cur;
    void *nxt = cur->next;
    pthread_t id = syscall(__NR_gettid);
    do {
        if (nxt == NULL) {
            printf("cur.next is null\n");
        }
        int result = pow(cur->data, 2);
        printf("result: %d\n", result);
        if (ptr == NULL) {
            printf("cur is null\n");
        }
        if (cur->data_result == NULL) {
            printf("data result is null\n");
        }
        cur->data_result = &result;
        cur = cur->next;
    }
    while (cur->next != NULL);
//    host_destroy(&cur->host);
    pthread_exit(&id);
}

/**
* Creates a new server instance (i.e., thread) to handle processing the items
* contained in a batch (i.e., a listed list of job_node). InstanceHost will
* maintain a list of active instances, and if the host is requested to
* shutdown, ensures that all jobs are completed.
*
* @param job_batch_list A list containing the jobs in a batch to process.
*/
void host_request_instance(host* h, struct job_node* batch) {
    pthread_create(&h->thread[h->instances], NULL, &instance_process_data, &batch[0].host);
    host_destroy(&batch[0].host);
}



