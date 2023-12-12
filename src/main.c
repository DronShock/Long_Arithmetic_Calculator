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

BigNumber subtraction(BigNumber bn1, BigNumber bn2);

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
    while (temp && temp->data == 0) {
        follow = temp;
        temp = temp->next;
        free(follow);
        count++;
    }

    if (temp == NULL) {
        *bn = new;
        return;
    }
    temp->prev = NULL;
    bn->head = temp;
    bn->size -= count;
}

void destroy_big_number(BigNumber *bn) {
    node *temp = bn->head;
    node *follow = NULL;

    BigNumber new;
    init_big_number(&new);
    *bn = new;
//возможно, получится заменить follow на temp->prev
    while (temp) {
        follow = temp;
        temp = temp->next;
        free(follow);
    }
}

void print_big_number(BigNumber bn) {
    node *temp = bn.head;
    if (bn.size == 0) {
        printf("0\n");
        return;
    }
    if (bn.sign) {
        printf("-");
    }
    while (temp) {
        if (temp->data < 10)
            printf("%c", temp->data + '0');
        else
            printf("%c", (temp->data - 10) + 'A');
        temp = temp->next;
    }
    printf("\n");
}

void insert_in_front(BigNumber *bn, int value) {
    node *temp = malloc(sizeof(node));
    if (temp) {
        temp->prev = temp->next = NULL;
        temp->data = value;
    } else return;//дописать ошибку выделения памяти
    if (bn->head == NULL) {
        bn->head = temp;
        bn->tail = temp;
    } else {
        bn->head->prev = temp;
        temp->next = bn->head;
        bn->head = temp;
    }
    bn->size++;
}

void insert_in_end(BigNumber *bn, int value) {
    node *temp = malloc(sizeof(node));
    if (temp) {
        temp->next = temp->prev = NULL;
        temp->data = value;
    } else return;//дописать ошибку выделения памяти
    if (bn->tail == NULL)
        bn->head = bn->tail = temp;
    else {
        bn->tail->next = temp;
        temp->prev = bn->tail;
        bn->tail = temp;
    }
    bn->size++;
}

int length(BigNumber bn) {
    return bn.size;
}

/* Сравнивает два числа и возвращает:
 * 0 если числа равны
 * 1 если больше первое число
 * 2 если больше второе число
 */
int compare(BigNumber bn1, BigNumber bn2) {
    remove_leading_zeros(&bn1);
    remove_leading_zeros(&bn2);

    BigNumber a = bn1;
    BigNumber b = bn2;

    int flag = 0;

    if (a.sign != b.sign) {
        if (b.sign)
            return 1;
        else
            return 2;
    } else if (a.sign)
        flag = 1;
    if (a.size != b.size) {
        if (flag)
            return (a.size > b.size) + 1;
        else
            return (a.size < b.size) + 1;
    } else {
        while (a.head != NULL && b.head != NULL) {
            if (a.head->data > b.head->data) {
                if (flag)
                    return 2;
                else
                    return 1;
            } else if (a.head->data < b.head->data) {
                if (flag)
                    return 1;
                else
                    return 2;
            } else {
                a.head = a.head->next;
                b.head = b.head->next;
            }
        }
        return 0;
    }
}

BigNumber addition(BigNumber bn1, BigNumber bn2) {
    remove_leading_zeros(&bn1);
    remove_leading_zeros(&bn2);

    BigNumber a = bn1;
    BigNumber b = bn2;

    BigNumber result;

    int macro, micro = 0;
    init_big_number(&result);

    if (a.sign != b.sign) {
        if (b.sign) {
            change_sign(&b);
            return subtraction(a, b);
        }
    } else
        result.sign = a.sign;

    while (a.tail != NULL || b.tail != NULL) {
        if (a.tail != NULL && b.tail != NULL) {
            micro = (a.tail->data + b.tail->data + macro) % 10;
            macro = (a.tail->data + b.tail->data + macro) / 10;
            a.tail = a.tail->prev;
            b.tail = b.tail->prev;
        }
        if (b.tail == NULL) {
            micro = (a.tail->data + macro) % 10;
            macro = (a.tail->data + macro) / 10;
            a.tail = a.tail->prev;
        }
        if (a.tail == NULL) {
            micro = (b.tail->data + macro) % 10;
            macro = (b.tail->data + macro) / 10;
            b.tail = b.tail->prev;
        }
        insert_in_front(&result, micro);
    }
    if (macro != 0)
        insert_in_front(&result, macro);

    return result;
}

BigNumber subtraction(BigNumber bn1, BigNumber bn2) {
    remove_leading_zeros(&bn1);
    remove_leading_zeros(&bn2);

    BigNumber a = bn1;
    BigNumber b = bn2;

    int macro, micro, flag = 0;
    BigNumber result, temp;
    init_big_number(&result);

    if (a.sign != b.sign) {
        change_sign(&b);
        return addition(a, b);
    } else if (a.sign && b.sign) {
        change_sign(&a);
        change_sign(&b);
        flag = 1;
    }

    if (compare(a, b) == 0)
        return result;
    if (compare(a, b) == 2) {
        change_sign(&result);
        temp = a;
        a = b;
        b = temp;
    }
    while (a.tail != NULL || b.tail != NULL) {
        if (a.tail != NULL && b.tail != NULL) {
            if (a.tail->data >= b.tail->data + macro) {
                micro = (a.tail->data - b.tail->data - macro);
                macro = 0;
            } else {
                micro = (a.tail->data + 10 - b.tail->data - macro);
                macro = 1;
            }
            a.tail = a.tail->prev;
            b.tail = b.tail->prev;
        }
        if (b.tail == NULL) {
            if (a.tail->data >= macro) {
                micro = a.tail->data - macro;
                macro = 0;
            } else {
                micro = a.tail->data + 10 - macro;
                macro = 1;
            }
            a.tail = a.tail->prev;
        }
        insert_in_front(&result, micro);
    }
    remove_leading_zeros(&result);

    if (flag)
        change_sign(&result);

    return result;
}

BigNumber multiplication(BigNumber bn1, BigNumber bn2) {
    remove_leading_zeros(&bn1);
    remove_leading_zeros(&bn2);
    BigNumber a = bn1;
    BigNumber b = bn2;

    int i, count = 0;
    BigNumber result, temp, fake_a;
    init_big_number(&result);

    if (a.size == 0 || b.size == 0)
        return result;
    if (b.size > a.size) {
        temp = a;
        a = b;
        b = temp;
    }
    init_big_number(&temp);
    while (b.tail != NULL) {
        int micro, macro = 0;
        fake_a = a;
        while (a.tail != NULL) {
            micro = (fake_a.tail->data * b.tail->data + macro) % 10;
            macro = (fake_a.tail->data * b.tail->data + macro) / 10;
            insert_in_front(&temp, micro);
            fake_a.tail = fake_a.tail->prev;
        }
        if (macro != 0)
            insert_in_front(&temp, macro);

        for (i = 0; i < count; i++) {
            insert_in_end(&temp, 0);
        }
        result = addition(result, temp);
        count++;
        b.tail = b.tail->prev;
        destroy_big_number(&temp);
    }
    if (a.sign == b.sign)
        result.sign = 0;
    else
        result.sign = 1;

    return result;
}

BigNumber division(BigNumber bn1, BigNumber bn2) {
    remove_leading_zeros(&bn1);
    remove_leading_zeros(&bn2);

    BigNumber a = bn1;
    BigNumber b = bn2;
    BigNumber one;
    init_big_number(&one);
    insert_in_end(&one, 1);

    BigNumber ex, mp, pr, result;
    init_big_number(&ex);
    init_big_number(&mp);
    init_big_number(&pr);
    init_big_number(&result);

    char sign = 0;
    if (a.sign != b.sign)
        sign = 1;

    if (b.size == 0) {
        fprintf(stderr, "%s", "Zero division\n");
        //написать возвращаемое значение для ошибки
        return ex;
    }
    if (compare(a, b) == 2)
        return result;
    else {
        one.sign = sign;
        return one;
    }
    while(a.head != NULL){
        int i = 0;
        for (i = 0; i < b.size && a.head; i++) {
            insert_in_end(&ex, a.head->data);
            a.head = a.head->next;
        }
        for (i = 1; i < 10; i++) {
            insert_in_end(&mp,i);
            pr = multiplication(b,mp);
            if(compare(pr, ex) == 1){
                destroy_big_number(&mp);
                destroy_big_number(&pr);
                break;
            }
            destroy_big_number(&mp);
            destroy_big_number(&pr);
        }
        int multiplier = i-1;

        insert_in_end(&mp, multiplier);
        pr = multiplication(b, mp);
        ex = subtraction(ex, pr);

        insert_in_end(&result, multiplier);
        remove_leading_zeros(&result);

        destroy_big_number(&pr);
        destroy_big_number(&mp);

        a.head = a.head->next;
    }
    destroy_big_number(&ex);
    remove_leading_zeros(&result);
    result.sign = sign;
    return result;
}

int main() {

    return 0;
};