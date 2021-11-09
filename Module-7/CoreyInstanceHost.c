#include <malloc.h>
#include <math.h>
#include "CoreyInstanceHost.h"
#include "CoreyLoadBalancer.h"


struct host {
    int instances;
    pthread_t *thread;
};


pthread_t *create_instance(host *h) {
    return malloc(sizeof(pthread_t));
}

/**
* Initializes the host environment.
*/
host *host_create() {
    host* host = malloc(sizeof(host));
    host->instances = 0;
    host->thread = create_instance(host);
    return host;
}

/**
* Shuts down the host environment. Ensures any outstanding batches have
* completed.
*/
void host_destroy(host** host) {
    free(host);
    host = NULL;
}

void * instance_init(void *batch) {
    struct job_node *cur = batch;
    do {
        int result = pow(cur->data, 2);
        cur->data_result = &result;
        cur = cur->next;
    }
    while (cur->next != NULL);
    return NULL;
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
    pthread_create(&h->thread[h->instances], NULL, &instance_init, &batch);
}



