//definition of the Process
typedef struct Process{
    //inputs
    int id;
    int arrivalTime;
    int priority;
    int runningTime;

    int pid;

    bool isRunning;
    bool isCreated; // true if process has been created, false if it is new

    int enterTime;
    int waitingTime;
    int quitTime;
    int executionTime;
    int remainingTime;

    int memeorySize;
    void* memoryPTR;

} Process;

// Definition of PQueue-node
typedef struct PQueueNode {
    Process *data;
    int priority;
    struct PQueueNode* next;
} PQueueNode;

typedef struct PQueue {
    PQueueNode* head;
} PQueue;

// PQueueNode *head;
//definition of Queue-node
typedef struct QueueNode {
    Process *data;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue {
    QueueNode *front;
    QueueNode *rear;
} Queue;

void printQueue(Queue *queue) {
    QueueNode *current = queue->front;
    while (current != NULL) {
        printf("QUEUEUEEEUEUEUEUEUEUEUE\n\n%d ", current->data->id);
        current = current->next;
    }
    printf("\n");
}

// Function to create a new priority queue
PQueue* create_priority_queue() {
    PQueue* q = (PQueue*) malloc(sizeof(PQueue));
    if (q == NULL) {
        // handle error if malloc fails to allocate memory
        return NULL;
    }
    q->head = NULL;
    return q;
}
// Function to create a new queue
Queue *create_queue() {
    Queue *q = (Queue *)malloc(sizeof(Queue));
    if (q == NULL) {
        // handle error if malloc fails to allocate memory
        return NULL;
    }
    q->front = q->rear = NULL;
    return q;
}

//Function to Create a new queue Node 

 QueueNode *create_queueNode (Process *processData){
    QueueNode *process = (QueueNode*) malloc(sizeof(QueueNode));
       if (process == NULL) {
        // handle error if malloc fails to allocate memory
        return NULL;
    }
    process->data = processData;
    process->next = NULL;
    return process;
 }

//Function to Create a new piority queue Node 

 PQueueNode *create_p_queueNode (Process *processData){
    PQueueNode *process = (PQueueNode*) malloc(sizeof(PQueueNode));
       if (process == NULL) {
        // handle error if malloc fails to allocate memory
        return NULL;
    }
    process->data = processData;
    process->priority = processData->priority;
    // process->priority = P where p is int recived;
    process ->next = NULL;
    return process;
 }

//returns the head value
Process* p_queue_peek(PQueue *q){
    return q->head->data;
}
Process* queue_peek(Queue *q){
    return q->front->data;
}

//removes the highest priority
void pop_p_queue(PQueue *q){
    PQueueNode *process = q->head;
    q->head = q->head->next;
    free(process);
}

//insert according to process priority
void insert_ordered(PQueue* q, Process* process) {
    PQueueNode* node_to_insert = create_p_queueNode(process);
   
    if (q->head == NULL || q->head->priority < node_to_insert->priority) {
        // insert at the head of the queue
        node_to_insert->next = q->head;
        q->head = node_to_insert;
    } else {
        PQueueNode* curr = q->head;
        while (curr->next != NULL && curr->next->priority >= node_to_insert->priority) {
            curr = curr->next;
        }
        // insert after the current node
        node_to_insert->next = curr->next;
        curr->next = node_to_insert;
    }
}
//insert according to process remaining time
void insert_time_order(PQueue* q, Process* process) {
    PQueueNode* node_to_insert = create_p_queueNode(process);
   
    if (q->head == NULL || q->head->data->remainingTime > node_to_insert->data->remainingTime) {
        // insert at the head of the queue
        node_to_insert->next = q->head;
        q->head = node_to_insert;
    } else {
        PQueueNode* curr = q->head;
        while (curr->next != NULL && curr->next->data->remainingTime >= node_to_insert->data->remainingTime) {
            curr = curr->next;
        }
        // insert after the current node
        node_to_insert->next = curr->next;
        curr->next = node_to_insert;
    }
}


void enQueue(Queue *q , Process *process){
    QueueNode* temp = create_queueNode(process);
    if(q->rear == NULL){
        q->front = q->rear = temp;
        return;
    }
    
    q->rear->next = temp;
    q->rear = temp;
}

void deQueue(Queue *q){
    if (q->front == NULL){
        return ;
    }
    QueueNode *temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL)
        q->rear = NULL;

    free(temp);

}

//empty
bool queue_isEmpty(struct Queue* q)
{
    return q->front == NULL;
}
bool p_queue_isEmpty(struct PQueue* q)
{
    return q->head == NULL;
}

// //delete the node by the key
// void* deleteByKey(PQueue q,int key){
//     PQueueNode *current = q->head ;
//     PQueueNode *prev = NULL ;
//     if(head == NULL) 
//         return NULL;
//     while (current->data->memoryPTR != key){
//         if(current->next == NULL){
//             return NULL;
//         }else{
//             prev = current;
//             current = current ->next;
//         }
//     }
//     //when the keys matches then 

//     //if the key is the first --> drop the first node
//     if(current == head){
//         head = head->next;
//     }else{
//     //the key is anywhere else we drop it and connect the prev to the next
//     prev->next = current ->next;
//     }
//     return current;
// }