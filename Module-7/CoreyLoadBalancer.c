#include <malloc.h>
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <memory.h>
#include "CoreyLoadBalancer.h"
#include "CoreyInstanceHost.h"

struct balancer {
    struct host *host;
    struct job_node *first;
    pthread_mutex_t protecc;
    int requests;
    int batchSize;
    int requestsCompleted;
};

/**
 * Initializes the load balancer. Takes batch size as parameter.
 */
balancer* balancer_create(int batch_size) {
    balancer* balance = malloc(sizeof(struct balancer) + 1);
    balance->requestsCompleted = 0;
    balance->host = host_create();
    balance->first = NULL;
    pthread_mutex_init(&balance->protecc, NULL);
    balance->batchSize = batch_size;
    balance->requests = 0;
    return balance;
}

/**
 * Shuts down the load balancer. Ensures any outstanding batches have
 * completed.
 */
void balancer_destroy(balancer** lb) {
    pthread_mutex_t m = (*lb)->protecc;
    pthread_mutex_destroy(&m);
    free(*lb);
    *lb = NULL;
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
    pthread_mutex_lock(&lb->protecc);
    ++lb->requests;
    printf("LoadBalancer: Received new job from user #%d to process data=%d and store it at %p.\n",
           user_id, data, data_return);

    struct job_node *j = malloc(sizeof(struct job_node));
    j->user_id = user_id;
    j->bal = lb;
    j->data = data;
    j->data_result = data_return;
//    printf("from balancer_add_job: address of data result for user %d and data %d is: %p\n", j->user_id, j->data, j->data_result);
    j->next = lb->first;
    lb->first = j;

    if (lb->requests == lb->batchSize) {
        printf("Received batch and spinning up new instance.\n");
        host_request_instance(lb->host, lb->first);
        lb->requests = 0;
        lb->requestsCompleted += lb->batchSize;
    }

    pthread_mutex_unlock(&lb->protecc);
}
