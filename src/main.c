#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    char data;
    struct node *next;
    struct node *prev;
} node;

typedef struct {
    int size;
    char sign;
    node *head;
    node *tail;
} BigNumber;

void init_big_number(BigNumber *bn) {
    bn->size = 0;
    bn->sign = 0;
    bn->head = bn->tail = NULL;
}

void change_sign(BigNumber *bn) {
    if (bn->sign)
        bn->sign = 0;
    else
        bn->sign = 1;
}

void remove_leading_zeros(BigNumber *bn) {
    node *temp = bn->head;
    node *follow;
    int count = 0;
    BigNumber new;
    init_big_number(&new);
//возможно, получится заменить follow на temp->prev
    while (temp && temp->data == 0){
        follow = temp;
        temp = temp->next;
        free(follow);
        count++;
    }

    if (temp == NULL){
        *bn = new;
        return;
    }
    temp->prev = NULL;
    bn->head = temp;
    bn->size -=count;
}

void destroy_big_number(BigNumber *bn){
    node *temp = bn->head;
    node *follow = NULL;

    BigNumber new;
    init_big_number(&new);
    *bn = new;
//возможно, получится заменить follow на temp->prev
    while (temp){
        follow = temp;
        temp = temp->next;
        free(follow);
    }
}

void print_big_number(BigNumber bn){
    node *temp = bn.head;
    if(bn.size == 0){
        printf( "0\n");
        return;
    }
    if(bn.sign){
        printf("-");
    }
    while(temp){
        if (temp->data < 10)
            printf("%c", temp->data + '0');
        else
            printf("%c", (temp->data - 10) + 'A');
        temp = temp->next;
    }
    printf("\n");
}

void insert_in_front(BigNumber *bn, int value){
    node *temp = malloc(sizeof(node));
    if(temp){
        temp->prev = temp->next = NULL;
        temp->data = value;
    }
    else return;//дописать ошибку выделения памяти
    if(bn->head == NULL){
        bn->head = temp;
        bn->tail = temp;
    }
    else{
        bn->head->prev = temp;
        temp->next = bn->head;
        bn->head = temp;
    }
    bn->size++;
}
void insert_in_end(BigNumber *bn, int value) {
    node* temp = malloc(sizeof(node));
    if(temp) {
        temp->next = temp->prev = NULL;
        temp->data = value;
    }
    else return;//дописать ошибку выделения памяти
    if (bn->tail == NULL)
        bn->head = bn->tail = temp;
    else {
        bn->tail->next = temp;
        temp->prev = bn->tail;
        bn->tail = temp;
    }
    bn->size++;
}

int length(BigNumber bn){
    return bn.size;
}

int main() {

    return 0;
};