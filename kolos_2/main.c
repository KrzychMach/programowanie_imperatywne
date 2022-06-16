#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct  {
    double *results;
    int len;
} Data;


typedef struct tagQueueElement {
    Data data;
    struct tagQueueElement *next;
} QueueElement;


typedef struct tagQueue {
    QueueElement *head;
    QueueElement *tail;
} Queue;


int free_queue(Queue *pqueue, double **table) {
    int counter = 0;
    QueueElement *element = pqueue->head;
    while (element != NULL) {
        for (int i = 0; i < element->data.len; ++i) {
            memcpy(table[i], element->data.results, element->data.len * sizeof(double));
//            table[counter][i] = element->data.results[i];
        }
        QueueElement *next_element = element->next;
        free(element->data.results);
        free(element);
        element = next_element;
        ++counter;
    }
    return counter;
}


void push_copy(Queue *pqueue, const Data*pdata) {
    QueueElement *element = malloc(sizeof(QueueElement));
    element->data = *pdata;
    element->next = NULL;
    pqueue->tail->next = element;
    pqueue->tail = element;
}


int pop(Queue *pqueue,  Data*pdata) {
    if (pqueue->head == NULL) return -1;

    QueueElement *element = pqueue->head;
    pqueue->head = element->next;
    if (pqueue->head == NULL) return 0;

    *pdata = element->data;
    free(element);
    return 1;
}


int get_total_count(const Queue *pqueue) {
    int counter = 0;
    QueueElement *element = pqueue->head;
    while (element != NULL) {
        counter += element->data.len;
        element = element->next;
    }
    return counter;
}
