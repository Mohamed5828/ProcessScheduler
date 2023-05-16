#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "headers.h"
#include "Queue.h"
#include "processesHelperFunctions.h" 
#include "HPF.h"

struct node 
{
    int data;
    int key;
    struct node *next;
};

struct node *head = NULL;
struct node *current = NULL;

//display the linked list
void printList(){
    struct node *p = head;
    printf("\n[ ");

    while(p != NULL){
        printf(" %d" , p->data);
        p = p->next;
    }
    printf ("]");
}

//insertion at the beginning
void insertAtBegin(int key,int data){
    struct node *link = (struct node*) malloc(sizeof(struct node));

    //create link
    link->key = key ; 
    link ->data = data;
    //point it the old node
    link -> next = head;
    //point first to the new first node
    head = link;
}

//insertion at the end
void insertatend(int key,int data){

   //create a link
   struct node *link = (struct node*) malloc(sizeof(struct node));
   link->key = key ; 
   link->data = data;
   struct node *linkedlist = head;

   // point it to old first node
   while(linkedlist->next != NULL)
      linkedlist = linkedlist->next;

   //point first to new first node
   linkedlist->next = link;
}

//delete first item 
struct node* deleteFirst(){

    //ref to the node to be deleted
    struct node *deletedNode = head;
    head = head->next ;
    return deletedNode;
}

boolean isEmpty(){
    return head == NULL ;
}

int linkedLength(){
    int length = 0;
    struct node *current;
    for(current = head ; current != NULL ; current = current ->next){
        length++;
    }
    return length;
    
}

//get the node by the key 

struct node* find(int key){
    struct node *current = head ;
    if(head == NULL) 
        return NULL;
    while (current->key != key){
        if(current->next == NULL){
            return NULL;
        }else{
            current = current ->next;
        }
    }
    return current;
}

//delete the node by the key
struct node* deleteByKey(int key){
    struct node *current = head ;
    struct node *prev = NULL ;
    if(head == NULL) 
        return NULL;
    while (current->key != key){
        if(current->next == NULL){
            return NULL;
        }else{
            prev = current;
            current = current ->next;
        }
    }
    //when the keys matches then 

    //if the key is the first --> drop the first node
    if(current == head){
        head = head->next;
    }else{
    //the key is anywhere else we drop it and connect the prev to the next
    prev->next = current ->next;
    }
    return current;
}

int main(){
    insertAtBegin(1,10);
   insertAtBegin(2,20);
   insertAtBegin(3,30);
   insertAtBegin(4,1);
   insertAtBegin(5,40);
   insertAtBegin(6,56); 

   printf("Original List: "); 
	
   //print list
   printList();

//    while(!isEmpty(linked)) 
//    {            
//       struct node *temp = deleteFirst();
//       printf("\nDeleted value:");
//       printf("(%d,%d) ",temp->key,temp->data);
//    }  
	
   printf("\nList after deleting all items: ");
   printList();
   
   
   insertAtBegin(1,10);
   insertAtBegin(2,20);
   insertAtBegin(3,30);
   insertAtBegin(4,1);
   insertAtBegin(5,40);
   insertAtBegin(6,56);
   printf("\nRestored List: ");
   printList();
   printf("\n");  

}