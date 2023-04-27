/********************************************************************************* 
* Karkar Elio, Ekarkar 
* 2023 Winter CSE101 PA#1 
* List.c 
* List ADT 
*********************************************************************************/ 
#include<stdio.h>
#include <stdlib.h>
#include<assert.h>
#include<stdbool.h>
#include "List.h"


typedef struct NodeObj {
    int data;
    struct NodeObj* next;
    struct NodeObj* previous;
} NodeObj;

typedef NodeObj* Node;

typedef struct LisObj{
    Node front;
    Node cursor;
    Node back;
    int index;
    int length;
} LisObj;

Node newNode(ListElement data) {
    Node N = malloc(sizeof(NodeObj));
    assert(N != NULL);
    N->data = data;
    N->next = NULL;
    N->previous = NULL;
    return(N);
}

void freeNode(Node* pN) {
    if (pN != NULL && *pN != NULL) {
        free(*pN);
        *pN = NULL;
    }
}

List newList() {
    List L;
    L = malloc(sizeof(LisObj));
    assert(L != NULL);
    L->front = NULL;
    L->back = NULL;
    L->cursor=NULL;
    L->length = 0;
    L->index = -1;
    return (L);
}
bool isEmpty(List L){
    if(L==NULL){
        printf("List error: Using isNULL() on NULL list");
        exit(EXIT_FAILURE);
    }
    return(L->length == 0);
}
void freeList(List* pL) {
    if (pL != NULL && *pL != NULL) {
        free(*pL);
        *pL = NULL;
    }
}

int length(List L) {
    if (L == NULL) {
        printf("List Error: the list you are using is NULL");
        exit(EXIT_FAILURE);
    }
    return(L->length);
}
int front(List L) {
    if (L == NULL) {
        printf("List Error: the list you are using is NULL");
        exit(EXIT_FAILURE);
    }
    return(L->front->data);
}
int back(List L) {
    if (L == NULL) {
        printf("List Error: the list you are using is NULL");
        exit(EXIT_FAILURE);
    }
    return(L->back->data);
}
bool equals(List A, List B) {
    if (A == NULL || B == NULL) {
        printf("List Error: calling equals() on NULL List");
        exit(EXIT_FAILURE);
    }
    bool eq;
    Node N, M;
    eq = (A->length == B->length);
    N = A->front;
    M = A->front;
    while (eq && N != NULL) {
        eq = (N->data == M->data);
        N = N->next;
        M = M->next;
    }
    return eq;
}
void clear(List L) {
    L->back = NULL;
    L->front = NULL;
    L->cursor =NULL;
    L->index = -1;
    L->length = 0;
}
int get(List L){
    if(L== NULL){
        printf("List Error: calling get() on NULL list");
        exit(EXIT_FAILURE);
    }
    return(L->cursor->data);
}
int index(List L) {
    if(L==NULL){
        printf("List Error: calling index() on NULL index");
        exit(EXIT_FAILURE);
    }
    return(L->index);
}

void set(List L, int x){
    if(L->length <= 0){
        printf("List Error: calling set() on NULL list");
        exit(EXIT_FAILURE);
    }
    if(L->index <=0){
        printf("List Error: calling set() on NULL index");
        exit(EXIT_FAILURE);
    }
    L ->cursor->data = x;
}
void moveFront(List L){
    if(L==NULL){
        printf("List error: calling moveFront() on NULL list");
        exit(EXIT_FAILURE);
    }
    L->cursor = L -> front;
    L->index = 0;
}
void moveBack(List L){
    if(L==NULL){
        printf("List error: calling moveBack() on NULL list");
        exit(EXIT_FAILURE);
    }
    L -> cursor = L->back;
    L->index = L->length - 1;
}
void movePrev(List L){
    if(L==NULL){
        printf("List error: calling movePrev() on NULL list");
        exit(EXIT_FAILURE);
    }
    if(L->cursor == L->front){
        L ->cursor = NULL;
        L -> index = -1;
    }else{
    if(L->cursor != NULL ){
        L-> cursor = L->cursor->previous;
        L-> index--;
    }
    }
}
void moveNext(List L){
    if(L==NULL){
        printf("List error: calling moveNext() on NULL list");
        exit(EXIT_FAILURE);
    }
    if(L->cursor == L->back){
        L ->cursor = NULL;
        L -> index = -1;
    }else{
    if(L->cursor!=NULL){
        L-> cursor = L->cursor->next;
        L-> index++;
    }
    }
}
void prepend(List L, int x){
    if(L==NULL){
        printf("List error: calling prepend() on NULL list");
        exit(EXIT_FAILURE);
    }
    if(L->length == 0){
        L->front = L->back = newNode(x);
    }
    else{
        Node temp;
        temp = L->front;
        L->front = newNode(x);
        temp -> previous = L ->front;
        L->front->next = temp;
        if(L->length == 1){
            L ->back = temp;
        }
        if(L->cursor){
            L -> index++;
        }
    }
    L->length++;
}

void append(List L, int x){
    if(L==NULL){
        printf("List error: calling append() on NULL list");
        exit(EXIT_FAILURE);
    }
    if(L->length == 0){
        L->front = L->back = newNode(x);
    }
    else{
        Node temp;
        temp = L->back;
        L->back = newNode(x);
        temp -> next = L -> back;
        L->back->previous = temp;
        if(L->length == 1){
            L->front = temp;
        }
        
    }
    L->length++;
}
void insertBefore(List L, int x){
    if(L==NULL){
        printf("List error: calling insertBefore() on NULL list reference");
        exit(EXIT_FAILURE);
    }
    if(L->length == 0){
        L->front = L->back = newNode(x);
    }
    if(L->cursor == L->front){
        prepend(L,x);
    }else{
    Node PreviousValue = L -> cursor ->previous;
    Node temp = newNode(x);
    PreviousValue->next = temp;
    temp -> previous = PreviousValue;
    temp -> next = L->cursor;
    L -> cursor -> previous = temp;
    L->length++;
    L->index++;
    }
}

void insertAfter(List L, int x){
    if(L==NULL){
        printf("List error: calling insertAfter() on empty list reference");
        exit(EXIT_FAILURE);
    }
    if(L-> length == 0){
        L->front=L->back=newNode(x);
    }else 
    if(L->cursor == L->back){
        append(L,x);
    }else{
    Node nextValue = L->cursor->next;
    Node temp = newNode(x);
    nextValue ->previous = temp;
    L->cursor->next = temp;
    temp ->next = nextValue;
    temp ->previous = L->cursor;
    L->length++;
    }
}

void deleteFront(List L){
    Node temp;
    if(L==NULL){
        printf("List error: calling deleteFront() on empty list");
        exit(EXIT_FAILURE);
    }
    if(L->length == 1){
        temp = L->front;
        freeNode(&temp);
        L->length--;
    }
        
        if(L->cursor == L->front){
        temp = L->front;
        L->front = temp ->next;
        freeNode(&temp);
        L->index = -1;
        L->cursor = NULL;
        L->length--;
        }else{
        temp = L->front;
        L->front = temp ->next;
        freeNode(&temp);
        L->front->previous = NULL;
        L->index--;
        L->length--;
        }
}

void deleteBack(List L){
    Node temp;
    if(L==NULL){
        printf("List error: calling deleteBack() on empty list");
        exit(EXIT_FAILURE);
    }
    if(L->length == 1){
        temp = L->back;
        freeNode(&temp);
        L->length--;
    }else{
    if(L->cursor == L->back){
        temp = L->back;
        L->back = temp ->previous;
        freeNode(&temp);
        L-> index = -1;
        L-> cursor = NULL;
        L->length--;
    
    }else{
        temp = L->back;
        L->back = temp ->previous;
        freeNode(&temp);
        L->back->next=NULL;
        L->length--;
    }
}
}

void delete(List L){
    if(L==NULL){
        printf("List error: calling delete() on empty list");
        exit(EXIT_FAILURE);
    }
    if(L->length == 0){
        printf("List error: calling delete() on NULL list reference");
        exit(EXIT_FAILURE);
    }
    if(L->length == 1){
        Node temp = L->cursor;
        freeNode(&temp);
        L->length--;
        L->cursor = NULL;
    }else{
    if(L->cursor == L->front){
        deleteFront(L);
    }else{
    if(L->back == L->cursor){
        deleteBack(L);
    }else{
        Node before, after, temp;
        before = L->cursor->previous;
        after = L->cursor->next;
        before -> next = after;
        after -> previous = before;
        temp = L -> cursor;
        freeNode(&temp);
        L->length--;
        L->cursor = NULL;
    }
}
}
}
void printList(FILE* out, List L){
    if(L==NULL){
        printf("List error: calling printList() on NULL list");
        exit(EXIT_FAILURE);
    }
    if(L->length <=0){
        printf("List error: calling printlist() on empty list");
        exit(EXIT_FAILURE);
    }
    moveFront(L);
    while(L->cursor!=NULL){
    fprintf(out, "%d ", L->cursor->data);
    moveNext(L);
}
}
List copyList(List L){
    if(L==NULL){
        printf("List error: using copyList on NULL list reference");
        exit(EXIT_FAILURE);
    }
    List copied = newList();
    copied -> front = L->front;
    copied -> back = L->back;
    copied -> length = L->length;
    Node current = NULL;
    moveFront(copied);
    current = L->front;
    while(current!=NULL){
        copied->cursor = L;
        current = current -> next;
    }
    copied -> cursor = NULL;
    copied -> index = -1;
    return(copied);
}