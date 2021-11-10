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
    pthread_t thread;
    int complete;
};


pthread_t create_instance(host *h) {
    return (pthread_t) malloc(sizeof(pthread_t) * sizeof(h));
}

/**
* Initializes the host environment.
*/
host *host_create() {
    host *host = (struct host*) malloc(sizeof(struct host) + 1);
    host->instances = 0;
    host->thread = create_instance(host);
    host->complete = 0;
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
    pthread_t id = cur->id;
//    printf("the result from user %d for data %d is: %d, stored at: %p\n", cur->user_id, cur->data, *(cur->data_result), cur->data_result);
    do {
        *(cur->data_result) = cur->data * cur->data;
//        printf("the result from user %d for data %d is: %d, stored at: %p\n", cur->user_id, cur->data, *(cur->data_result), cur->data_result);
        cur = cur->next;
    }
    while (cur->next != NULL);
    *(cur->data_result) = cur->data * cur->data;
//    cur->data_result = (int) pow(cur->data, 2);
//    printf("the result from user %d for data %d is: %d, stored at: %p\n", cur->user_id, cur->data, *(cur->data_result), cur->data_result);
    cur->bal->host->complete = 1;
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
    ++h->instances;
    batch->id = h->thread;
    pthread_create(&h->thread, NULL, &instance_process_data, &batch[0]);
    while (!h->complete);
    pthread_mutex_unlock(&batch->bal->protecc);
}



