#include <malloc.h>
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include "CoreyLoadBalancer.h"
#include "CoreyInstanceHost.h"

struct balancer {
    struct host *host;
    struct job_node *first;
    pthread_mutex_t *protecc;
    int requests;
    int batchSize;
};

/**
 * Initializes the load balancer. Takes batch size as parameter.
 */
balancer* balancer_create(int batch_size) {
    balancer* bal = malloc(sizeof(struct balancer));
    bal->host = host_create();
    bal->first = NULL;
    pthread_mutex_init(bal->protecc, NULL);
    bal->batchSize = batch_size;
    bal->requests = 0;
    return 0;
}

/**
 * Shuts down the load balancer. Ensures any outstanding batches have
 * completed.
 */
void balancer_destroy(balancer** lb) {
    free(lb);
    lb = NULL;
}

/**
 * Adds a job to the load balancer. If enough jobs have been added to fill a
 * batch, will request a new instance from InstanceHost. When job is complete,
 * *data_return will be updated with the result.
 *
 * @param user_id the id of the user making the request.
 * @param data the data the user wants to process.
 * @param data_return a pointer to a location to store the result of processing.
 */
void balancer_add_job(balancer* lb, int user_id, int data, int* data_return) {
    pthread_mutex_lock(lb->protecc);
    ++lb->requests;
    printf("LoadBalancer: Received new job from user #%d to process data=%d and store it at %p.\n",
           user_id, data, data_return);

    struct job_node *j = malloc(sizeof(struct job_node));
    j->data = data;
    j->data_result = data_return;
    j->next = lb->first;
    lb->first = j;

    if (lb->requests == lb->batchSize) {
        printf("Received batch and spinning up new instance.\n");
        host_request_instance(lb->host, lb->first);
    }

    pthread_mutex_unlock(lb->protecc);
}
