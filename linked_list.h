#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef unsigned list_t;

//Узел списка
typedef struct Node {
    struct Node* next; // указатель на следующий узел
    char data[];       // хранимые данные
} node_t;

//Заголовок списка
typedef struct Head {
    node_t* front;       // указатель на первый узел списка
    node_t* end;         // указатель на последний узел списка
    unsigned node_count; // количество узлов в списке
    unsigned data_size;  // размер хранимых данных в байтах
    bool valid;          // флаг валидности списка
} head_t;

//Операции со списками

//Возвращает идентификатор списка
list_t new_list(unsigned data_size);

void list_free(list_t list);
//Возвращает true, если лист по данному идентификатору существует
bool list_valid(list_t);
//Возвращает размер списка в узлах
unsigned list_size(list_t list);

//Функции добавления и вставки
//Возвращают количество записанных байт

//сложность операции O(1)
unsigned insert_front(list_t list, void* p_data);
//сложность операции O(n)
unsigned insert_at(list_t list, unsigned index, void* p_data);
//сложность операции O(1)
unsigned push_back(list_t list, void* p_data);

//Функции доступа к элементам
//Возвращают указатель на данные в узле

//сложность операции O(1)
void* get_front(list_t list);
//сложность операции O(n)
void* get_at(list_t list, unsigned index);
//сложность операции O(1)
void* get_back(list_t list);

//Функция поиска элемента
//сложность операции O(n)
/*
 * Функция поиска элемента
 * сложность операции O(n)
 * возвращает индекс узла если элемент найден, в противном случае -1
 * searchable - указатель на искомый элемент
 * *equal - указатель на функцию сравнения данных (memcpm может сработать неправильно из-за выравнивания структур)
 */
long long find(list_t list, void* searchable, bool (*equal)(void*, void*));

//Функции удаления

//сложность операции O(1)
void delete_front(list_t list);
//сложность операции O(n)
void delete_at(list_t list, unsigned index);
//сложность операции O(n)
void delete_back(list_t list);


#endif // LINKED_LIST_H
