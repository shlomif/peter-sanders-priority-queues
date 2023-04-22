#include <limits.h>

#ifndef DEBUGLEVEL
#define DEBUGLEVEL 0
#endif


#include "knheap_c_binding.h"

#include <vector>

typedef std::pair<int, void*> V;
typedef std::vector<V> K;

void* KNHeap__create() {
    return new K();
}

void KNHeap__destroy(void* instance) {
    delete reinterpret_cast<K*>(instance);
}

int   KNHeap__getSize(void* instance) {
    return reinterpret_cast<K*>(instance)->size();
}

int get(K&vec, int key) {
    int i = 0;
    for (V & k : vec) if (k.first == key) return i; else i++;
    return -1;
}

void  KNHeap__getMin(void* instance, int *key, void* *value) {
    K & vec = *reinterpret_cast<K*>(instance);
    int i = get(vec, *key);
    if (i != -1) *value = vec[i].second;
}
void  KNHeap__deleteMin(void* instance, int *key, void* *value) {
    K & vec = *reinterpret_cast<K*>(instance);
    int i = get(vec, *key);
    if (i != -1) {
        *value = vec[i].second;
        vec.erase(vec.begin()+i);
    }
}
void  KNHeap__insert(void* instance, int *key, void* value) {
    K & vec = *reinterpret_cast<K*>(instance);
    vec.insert(vec.end(), {*key, value});
}
