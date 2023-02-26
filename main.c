#include "linked_list.h"

typedef struct Person {
    int user_id;
    unsigned age;
    const char* user_name;
    unsigned tel_number;
    unsigned scores;
} person_t;

bool id_equal(void* person_a, void* person_b) {
    return (((person_t*)person_a)->user_id == ((person_t*)person_b)->user_id) ;
}

int main() {
    list_t juniors = new_list(sizeof(person_t));
    list_t middles = new_list(sizeof(person_t));
    list_t senjors = new_list(sizeof(person_t));
    person_t buffer;
    FILE* file = fopen("participants.dat", "r");
    while (fread(&buffer, sizeof(person_t), 1, file) == sizeof(person_t)) {
        unsigned age = buffer.age;
        if ((age < 14) || (65 < age))
            continue;
        if (age < 25) {
            insert_front(juniors, &buffer);
            continue;
        }
        if ((25 <= age) && (age < 40)) {
            insert_front(middles, &buffer);
            continue;
        }
        if (40 <= age) {
            insert_front(middles, &buffer);
        }
    }
    printf("Young age participants: %d\n", list_size(juniors));
    printf("Middle-aged participants: %d\n", list_size(middles));
    printf("Senjor participants: %d\n", list_size(senjors));

    person_t junior1; // = ...
    if (find(juniors, &junior1, id_equal)) {
        //...
    }
    /*
     * Обработка списков участников
     */
    list_free(juniors);
    list_free(middles);
    list_free(senjors);

    return 0;
}
