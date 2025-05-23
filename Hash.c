#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Hash.h"

static size_t HashFcn(const char *const key, const size_t tablesize) { // djb2
    size_t index = 5381;
    size_t len = strlen(key);
    int c;
    for (size_t i = 0; i < len; i++) {
        c = *(key+i);
        index = ((index << 5) + index) + c; // index * 33 + c
    }
    return index%tablesize;
}

static DATA *NewHash(DATABASE *const database, const char *const key) {
    DATA *newnode = NewData(key);
    HashTable_t *newhash = (HashTable_t *) malloc(sizeof(HashTable_t));
    if ((!newhash) || (!newnode)) exit(1);
    
    newhash->load = 0;
    newhash->tablesize = TABLE_SIZE;
    newhash->load_factor = 0.0;
    HashNode_t **temp_pptr = (HashNode_t **) malloc(TABLE_SIZE*sizeof(HashNode_t *));
    if (!temp_pptr) exit(1);
    for (size_t i = 0; i < TABLE_SIZE; i++) *(temp_pptr+i) = NULL;
    newhash->head = temp_pptr;
    newnode->value = (void *) newhash;

    if (database->length[TYPE_HASH]==0) {
        database->datalist_head[TYPE_HASH] = newnode;
        database->datalist_tail[TYPE_HASH] = newnode;
        database->length[TYPE_HASH] = 1;
    } else {
        newnode->next = database->datalist_head[TYPE_HASH];
        database->datalist_head[TYPE_HASH] = newnode;
        (database->length[TYPE_HASH])++;
    }
    return newnode;    
}

static HashTable_t *ReArrange(HashTable_t *hashtable) {
    HashTable_t *new_hashtable = (HashTable_t *) malloc(sizeof(HashTable_t));
    if (!new_hashtable) exit(1);
    new_hashtable->load = hashtable->load;
    new_hashtable->tablesize = 2*hashtable->tablesize;
    new_hashtable->load_factor = (float) new_hashtable->load / new_hashtable->tablesize;
    new_hashtable->head = (HashNode_t **) malloc(new_hashtable->tablesize*sizeof(HashNode_t *));
    for (size_t i = 0; i < new_hashtable->tablesize; i++) *(new_hashtable->head+i) = NULL;
    for (size_t i = 0; i < hashtable->tablesize; i++) {
        HashNode_t *curr = *(hashtable->head+i);
        while (curr) {
            HashNode_t *next = curr->next;
            curr->next = NULL;
            size_t new_index = HashFcn(curr->field, new_hashtable->tablesize);
            if (!*(new_hashtable->head+new_index)) {
                *(new_hashtable->head+new_index) = curr;
            } else {
                HashNode_t *new_curr = *(new_hashtable->head+new_index);
                while (new_curr->next) new_curr = new_curr->next;
                new_curr->next = curr;
            }
            curr = next;
        }
    }
    free(hashtable->head);
    free(hashtable);
    return new_hashtable;
}

void _HSET(DATABASE *const database, const char *const key, const char *const field, const char *const value) {
    DATA *node = SearchKey(database, key, TYPE_HASH);
    if (!node) node = NewHash(database, key);
    HashTable_t *hashtable = (HashTable_t *) node->value;

    HashNode_t *newhashnode;
    size_t index = HashFcn(field, hashtable->tablesize);
    if (!*(hashtable->head+index)) {
        newhashnode = (HashNode_t *) malloc(sizeof(HashNode_t));
        newhashnode->field = (char *) malloc(strlen(field)+1);
        newhashnode->value = (char *) malloc(strlen(value)+1);
        memcpy(newhashnode->field, field, strlen(field)+1);
        memcpy(newhashnode->value, value, strlen(value)+1);
        newhashnode->next = NULL;
        *(hashtable->head+index) = newhashnode;
    } else {
        if (strncmp((*(hashtable->head+index))->field, field, strlen(field)+1)==0) {
            free((*(hashtable->head+index))->value);
            (*(hashtable->head+index))->value = (char *) malloc(strlen(value)+1);
            memcpy((*(hashtable->head+index))->value, value, strlen(value)+1);
            hashtable->load--;
        } else {
            HashNode_t *curr = *(hashtable->head+index);
            while (curr->next) curr = curr->next;
            newhashnode = (HashNode_t *) malloc(sizeof(HashNode_t));
            newhashnode->field = (char *) malloc(strlen(field)+1);
            newhashnode->value = (char *) malloc(strlen(value)+1);
            memcpy(newhashnode->field, field, strlen(field)+1);
            memcpy(newhashnode->value, value, strlen(value)+1);
            newhashnode->next = NULL;
            curr->next = newhashnode;
        }
    }
    hashtable->load_factor = (float) (++hashtable->load)/hashtable->tablesize;
    if (hashtable->load_factor > LOAD_THERSHOLD) {
        hashtable = ReArrange(hashtable);
        node->value = (void *) hashtable;
    }
}

char *_HGET(const DATABASE *const database, const char *const key, const char *const field) {
    DATA *node = SearchKey(database, key, TYPE_HASH);
    if (!node) return NULL;
    HashTable_t *hashtable = (HashTable_t *) node->value;
    size_t index = HashFcn(field, hashtable->tablesize);
    if (!*(hashtable->head+index)) return NULL;
    if (strncmp((*(hashtable->head+index))->field, field, strlen(field)+1)==0) {
        printf("\"%s\"\n", (*(hashtable->head+index))->value);
        return (*(hashtable->head+index))->value;
    } else {
        HashNode_t *curr = *(hashtable->head+index);
        while (curr) {
            if (strncmp(curr->field, field, strlen(field)+1)==0) return curr->value;
            curr = curr->next;
        }
    }
    return NULL;
}

int _HDEL(DATABASE *const database, const char *const key, const char *const field) {
    DATA *node = SearchKey(database, key, TYPE_HASH);
    if (!node) return 1;
    HashTable_t *hashtable = (HashTable_t *) node->value;
    size_t index = HashFcn(field, hashtable->tablesize);
    if (!*(hashtable->head+index)) return 1;
    
    if (strncmp((*(hashtable->head+index))->field, field, strlen(field)+1)==0) {
        HashNode_t *del = *(hashtable->head+index);
        *(hashtable->head+index) = (*(hashtable->head+index))->next;
        free(del->field);
        free(del->value);
        free(del);
        return 0;
    } else {
        HashNode_t *curr = *(hashtable->head+index), *prev=NULL;
        while (curr) {
            if (strncmp(curr->field, field, strlen(field)+1)==0) {
                prev->next = curr->next;
                free(curr->field);
                free(curr->value);
                free(curr);
                return 0;
            }
            prev = curr;
            curr = curr->next;
        }        
    }
}

void FreeHash(DATA *const data) {
    HashTable_t *hashtable = (HashTable_t *) data->value;
    for (size_t i = 0; i < hashtable->tablesize; i++) {
        HashNode_t *curr = *(hashtable->head+i), *prev=NULL;
        while (curr) {
            free(curr->field);
            free(curr->value);
            prev = curr;
            curr = curr->next;
            free(prev);
        }
    }
    free(hashtable->head);
    free(hashtable);
}