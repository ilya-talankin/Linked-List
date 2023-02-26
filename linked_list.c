#include "linked_list.h"

//lists_table - массив, хранящий заголовки всех списков
//list_t - индекс заголовка в таблице lists_table
head_t* lists_table = NULL;
//резмер таблицы в заголовках
unsigned table_size = 0;

bool init_table() {
    lists_table = malloc(1);
    return lists_table;
}

list_t new_list(unsigned data_size) {

    //создание таблицы списков, если ещё не создана
    if (!lists_table)
        if(!init_table()) {
            printf("Memory allocation error\n");
            return 0;
        }
    list_t new_list;

    //ищем невалидные заголовки, чтобы занять их место
    bool free_found = 0;
    for (unsigned i = 0; i < table_size; ++i) {
        if (!list_valid(i)) {
            free_found = 1;
            new_list = i;
            break;
        }
    }

    //если необходимо, расширяем таблицу для добавления нового заголовка
    if (!free_found) {
        head_t* p = realloc(lists_table, (table_size + 1)*(sizeof(head_t)));
        if(!p) {
            printf("Memory reallocation error\n");
            return 0;
        }
        lists_table = p;
        new_list = table_size;
        ++table_size;
    }

    //инициализируем заголовок
    head_t new_head = {NULL, NULL, 0, data_size, true};
    lists_table[new_list] = new_head;

    return new_list;
}

void list_free(list_t list) {
    if (!list_valid(list))
        return;

    //освобождаем память из-под всех узлов
    head_t* head = &lists_table[list];
    node_t* next_node = head->front;
    for (unsigned i = 1; i <= head->node_count; ++i) {
        node_t* current_node = next_node;
        next_node = current_node->next;
        free(current_node);
    }
    head->valid = false;

    //уменьшаем таблицу, если список был последним в памяти, но не единстенным
    if ((list > 1) && (list == table_size - 1)) {
        head_t* p = realloc(lists_table, (table_size - 1)*sizeof(head_t));
        if (p) {
            lists_table = p;
            --table_size;
        } else {
            printf("Memory allocation error\n");
        }
    }
}

bool list_valid(list_t list) {
    if ((table_size < 1) || (list >= table_size))
        return 0;
    return lists_table[list].valid;
}

unsigned list_size(list_t list) {
    if (!list_valid(list)) {
        printf("Attempt to find out size of non-valid list #%d\n", list);
        return 0;
    }
    return lists_table[list].node_count;
}

bool input_valid(list_t list, void* p_data) {
    if (!list_valid(list)) {
        printf("Attempt to push data into a non-valid list #%d\n", list);
        return 0;
    }
    if (!p_data)
        return 0;
    return 1;
}

unsigned insert_front(list_t list, void* p_data) {
    if (!input_valid(list, p_data))
        return 0;

    //инициализация узла
    head_t* head = &lists_table[list];
    node_t* new_node = (node_t*)malloc(sizeof(node_t) + head->data_size);
    if (!new_node)
        return 0;
    new_node->next = head->front;
    if (memcpy_s(new_node->data, head->data_size, p_data, head->data_size))
        return 0;

    //вставка
    head->front = new_node;
    if (!head->end)
        head->end = new_node;
    ++head->node_count;

    return head->data_size;
}

unsigned insert_at(list_t list, unsigned index, void* p_data) {

    // проверка входных данных
    if (!input_valid(list, p_data))
        return 0;
    head_t* head = &lists_table[list];
    if (index > head->node_count)
        return 0;

    //вставка в начало или конец списка
    if (index == head->node_count)
        return push_back(list, p_data);
    if (index == 0)
        return insert_front(list, p_data);

    //поиск предшествующего узла
    node_t* node_before = head->front;
    for (unsigned i = 0; i < index - 1; ++i)
        node_before = node_before->next;

    //инициализация нового узла
    node_t* new_node = malloc(head->data_size);
    if (!new_node)
        return 0;
    new_node->next = node_before->next;
    if (memcpy_s(new_node->data, head->data_size, p_data, head->data_size))
        return 0;

    //вставка
    node_before->next = new_node;
    ++head->node_count;
    return head->data_size;
}

unsigned push_back(list_t list, void* p_data) {
    if (!input_valid(list, p_data)) {
        return 0;
    }

    //инициализация нового узла
    head_t* head = &lists_table[list];
    node_t* new_node = (node_t*)malloc(sizeof(node_t) + head->data_size);
    if (!new_node)
        return 0;
    if (memcpy_s(new_node->data, head->data_size, p_data, head->data_size))
        return 0;

    //вставка
    if (head->end) {
        head->end->next = new_node;
        head->end = new_node;
    } else {
        head->end = head->front = new_node;
    }
    ++head->node_count;

    return head->data_size;
}


void* get_front(list_t list) {
    if (!list_valid(list))
        return NULL;
    return lists_table[list].front->data;
}

void* get_at(list_t list, unsigned index) {
    // проверка входных данных
    if (!list_valid(list))
        return NULL;
    head_t* head = &lists_table[list];
    if (index > head->node_count - 1)
        return NULL;

    // поиск нужного узла
    node_t* current_node = head->front;
    for (unsigned i = 0; i < index; ++i)
        current_node = current_node->next;

    return current_node->data;
}

void* get_back(list_t list) {
    if (!list_valid(list))
        return NULL;
    return lists_table[list].end->data;
}

long long find(list_t list, void* searchable, bool (*equal)(void*, void*)) {
    // проверка входных данных
    if (!list_valid(list)) {
        printf("Attempt to find element in a non-valid list #%d\n", list);
        return -1;
    }
    head_t* head = &lists_table[list];
    if (head->node_count == 0)
        return -1;

    // поиск соответствующего узла
    unsigned index = 0;
    node_t* current_node = head->front;
    for (; index <  head->node_count; ++index) {
        if (equal(searchable, current_node->data))
            break;
        current_node = current_node->next;
    }
    if (index == head->node_count)
        return -1;

    return index;
}


void delete_front(list_t list) {
    if (!list_valid(list))
        return;
    head_t* head = &lists_table[list];
    node_t* front_node = head->front;

    if (!front_node)
        return;
    head->front = front_node->next;
    free(front_node);

    if (head->end == front_node)
        head->end = NULL;
    --head->node_count;
}

void delete_at(list_t list, unsigned index) {
    // проверка входных данных
    if (!list_valid(list))
        return;
    head_t* head = &lists_table[list];
    if (index >= head->node_count)
        return;

    // удаление граничных узлов
    if (index == 0) {
        delete_front(list);
        return;
    }
    if (index == head->node_count - 1) {
        delete_back(list);
        return;
    }

    // поиск и удаление
    node_t* node_before = head->front;
    for (unsigned i = 0; i < index - 1; ++i)
        node_before = node_before->next;
    node_t* node_to_delete = node_before->next;
    node_before->next = node_to_delete->next;
    free(node_to_delete);
    --head->node_count;
}

void delete_back(list_t list) {
    // проверка входных данных
    if (!list_valid(list))
        printf("Attempt to find out size of non-valid list #%d\n", list);
    head_t* head = &lists_table[list];
    if (!head->end)
        return;
    free(head->end);

    //обновление указателя на последний узел
    if (head->node_count == 1) {
        head->front = head->end = NULL;
        --head->node_count;
        return;
    }
    node_t* before_last = head->front;
    for (unsigned i = 1; i < head->node_count - 1; ++i)
        before_last = before_last->next;
    head->end = before_last;
    --head->node_count;
}

