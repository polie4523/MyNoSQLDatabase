#include <string.h>
#include <limits.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include "Sorted_set.h"

static int randomLevel(void) {
    int level = 0;
    while ((rand() < RAND_MAX / 2) && level < MAX_LEVEL) level++;
    return level;
}

static Sorted_set_element *NewElement(const double score, const char *const name) {
    Sorted_set_element *newelement = (Sorted_set_element *) malloc(sizeof(Sorted_set_element));
    newelement->name = (char *) malloc((strlen(name)+1)*sizeof(char));
    if ((!newelement) || (!newelement->name)) exit(1);
    memcpy(newelement->name, name, strlen(name)+1);
    newelement->score = score;
    for (int i = 0; i < MAX_LEVEL+1; i++) newelement->forward[i] = NULL;
    return newelement;
}

static DATA *NewSortedSet(DATABASE *const database, const char *const key) {
    DATA *newnode = NewData(key);
    Sorted_set *newset = (Sorted_set *) malloc(sizeof(Sorted_set));
    if ((!newset) || (!newnode)) exit(1);
    newset->len = 0;
    newset->level = 0;
    newset->head = NewElement(INT_MIN, "head");
    newnode->value = (void *) newset;
    if (database->length[TYPE_SORTEDSET]==0) {
        database->datalist_head[TYPE_SORTEDSET] = newnode;
        database->datalist_tail[TYPE_SORTEDSET] = newnode;
        database->length[TYPE_SORTEDSET] = 1;
    } else {
        newnode->next = database->datalist_head[TYPE_SORTEDSET];
        database->datalist_head[TYPE_SORTEDSET] = newnode;
        (database->length[TYPE_SORTEDSET])++;
    }
    return newnode;
}

// Sorted_set_element *SearchElement(const Sorted_set *const set, const double score) {
//     if (!set || !set->head) return NULL;
//     Sorted_set_element *current = set->head;
//     for (int i=set->level;i >= 0;i--) {
//         while (current->forward[i] && current->forward[i]->score < score) {
//             current = current->forward[i];
//         }
//     }
//     current = current->forward[0];
//     if (current && current->score==score) return current;
//     return NULL;
// }

static void InsertElement(Sorted_set *const set, const double score, const char *const name) {
    Sorted_set_element *update[MAX_LEVEL+1], *current = set->head;
    for (int i=set->level;i >= 0;i--) {
        while (current->forward[i] && current->forward[i]->score < score) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];
    if (!current || current->score != score) {
        int newLevel = randomLevel();
        if (newLevel > set->level) {
            for (int i=set->level+1;i<newLevel+1;i++) update[i] = set->head;
            set->level = newLevel;
        }
        Sorted_set_element *newelement = NewElement(score, name);
        for (int i=0;i<=newLevel;i++) {
            newelement->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newelement;
        }
    }
}

void FreeSortedSet(DATA *const data) {
    Sorted_set *set = (Sorted_set *) data->value;
    Sorted_set_element *curr = set->head, *prev=NULL;
    while (curr) {
        prev = curr;
        curr = curr->forward[0];
        free(prev->name);
        free(prev);
    }
    free(set);
}

int _ZADD(DATABASE *const database, const char *const key, const double score, const char *const name) {
    DATA *tmp = SearchKey(database, key, TYPE_SORTEDSET);
    if (!tmp) tmp = NewSortedSet(database, key);
    Sorted_set *tmp_set = (Sorted_set *) tmp->value;
    InsertElement(tmp_set, score, name);
    tmp_set->len++;
    return 0;
}

size_t _ZCARD(const DATABASE *const database, const char *const key) {
    DATA *tmp = SearchKey(database, key, TYPE_SORTEDSET);
    if (!tmp) return 0;
    Sorted_set *tmp_set = (Sorted_set *) tmp->value;
    return tmp_set->len;
}

size_t _ZCOUNT(const DATABASE *const database, const char *const key, const double min, const double max) {
    DATA *tmp = SearchKey(database, key, TYPE_SORTEDSET);
    if (!tmp) return 0;
    Sorted_set *set = (Sorted_set *) tmp->value;
    Sorted_set_element *current = set->head;
    for (int i=set->level;i >= 0;i--) {
        while (current->forward[i] && current->forward[i]->score < min) {
            current = current->forward[i];
        }
    }
    current = current->forward[0];
    size_t count = 0;
    while (current && (current->score <= max)) {
        count++;
        current = current->forward[0];
    }
    return count;
}

size_t _ZRANGE(const DATABASE *const database, const char *const key, long start, long stop) {
    DATA *tmp = SearchKey(database, key, TYPE_SORTEDSET);
    if (!tmp) return 0;
    Sorted_set *set = (Sorted_set *) tmp->value;
    if (start < 0) start = set->len+start;
    if (start < 0) start = 0;
    if (stop < 0) stop = set->len+stop;
    if ((set->len<=start) || (stop<start)) {printf("Empty\n");return 0;}
    if (stop >= set->len) stop = set->len-1;
    Sorted_set_element *current = set->head->forward[0];
    for (size_t i = 0; i < start; i++) current = current->forward[0];

    printf("Sorted set : %s\n", tmp->key);
    printf("Index\tname\n");
    printf("---------------------------\n");
    size_t num = start;
    while (num<=stop) {
        printf("%ld\t%s\n", num, current->name);
        current = current->forward[0];
        num++;
    }
    printf("---------------------------\n");
    return num-start;
}

size_t _ZRANGEBYSCORE(const DATABASE *const database, const char *const key, const double min, const double max) {
    DATA *tmp = SearchKey(database, key, TYPE_SORTEDSET);
    if (!tmp) return 0;
    Sorted_set *set = (Sorted_set *) tmp->value;
    Sorted_set_element *current = set->head;
    for (int i=set->level;i >= 0;i--) {
        while (current->forward[i] && current->forward[i]->score < min) {
            current = current->forward[i];
        }
    }
    current = current->forward[0];
    printf("Sorted set : %s\n", tmp->key);
    printf("Index\tscore\tname\n");
    printf("---------------------------\n");
    size_t num = 0;
    while (current && (current->score <= max)) {
        printf("%ld\t%.2f\t%s\n", num, current->score, current->name);
        num++;
        current = current->forward[0];
    }
    printf("---------------------------\n");           
    return num;
}

size_t _ZRANK(const DATABASE *const database, const char *const key, const char *const name) {
    DATA *tmp = SearchKey(database, key, TYPE_SORTEDSET);
    if (!tmp) return (size_t)-1;
    Sorted_set *set = (Sorted_set *) tmp->value;
    
    Sorted_set_element *current = set->head->forward[0];
    size_t index = 0;
    while (current && strncmp(current->name, name, strlen(name)+1)) {
        index++;
        current = current->forward[0];
    }
    if (current && !strncmp(current->name, name, strlen(name)+1)) {
        return index;
    }
    return (size_t)-1;
}

int _ZREM(const DATABASE *const database, const char *const key, const char *const name) {
    DATA *tmp = SearchKey(database, key, TYPE_SORTEDSET);
    if (!tmp) return 1;
    Sorted_set *set = (Sorted_set *) tmp->value;

    Sorted_set_element *update[MAX_LEVEL+1], *current = set->head;
    for (int i=set->level;i >= 0;i--) {
        current = set->head;
        while (current->forward[i] && strncmp(current->forward[i]->name, name, strlen(name)+1)) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];
    if(current && !strncmp(current->name, name, strlen(name)+1)) {
        for (int i=0;i<=set->level;i++) {
            if (update[i]->forward[i] != current) break;
            update[i]->forward[i] = current->forward[i];
        }
        free(current->name);
        free(current);
        set->len--;
        while(set->level>0 && set->head->forward[set->level] == NULL) set->level--;
    }
    return 0;
}

int _ZREMRANGEBYSCORE(const DATABASE *const database, const char *const key, const double min, const double max) {
    DATA *tmp = SearchKey(database, key, TYPE_SORTEDSET);
    if (!tmp) return 1;
    Sorted_set *set = (Sorted_set *) tmp->value;

    Sorted_set_element *update[MAX_LEVEL+1], *del_node=set->head;
    for (int i=set->level;i >= 0;i--) {
        while (del_node->forward[i] && del_node->forward[i]->score < min) {
            del_node = del_node->forward[i];
        }
        update[i] = del_node;
    }
    del_node = del_node->forward[0];
    Sorted_set_element *next[MAX_LEVEL+1], *max_node = del_node;
    for (int i=set->level;i >= 0;i--) {
        while (max_node->forward[i] && max_node->forward[i]->score <= max) {
            max_node = max_node->forward[i];
        }
        next[i] = max_node->forward[i];
    }
    for (int i=0;i <= set->level;i++) {
        if (update[i]->forward[i] == next[i]) break;
        update[i]->forward[i] = next[i];
    }
    max_node = del_node;
    while (max_node!=next[0]) {
        del_node = max_node;
        max_node = max_node->forward[0];
        free(del_node->name);
        free(del_node);
        set->len--;
    }
    while(set->level>0 && set->head->forward[set->level] == NULL) set->level--;
    return 0;
}
