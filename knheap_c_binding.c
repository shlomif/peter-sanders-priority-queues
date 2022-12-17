#include <limits.h>

#define DEBUGLEVEL 0

#include "knheap.C"

#include "knheap_c_binding.h"

void* KNHeap__create() {
    return new KNHeap<int, void*>(INT_MAX, -INT_MAX);
}

void KNHeap__destroy(void* instance) {
    delete reinterpret_cast<KNHeap<int, void*>*>(instance);
}

int   KNHeap__getSize(void* instance) {
    return reinterpret_cast<KNHeap<int, void*>*>(instance)->getSize();
}
void  KNHeap__getMin(void* instance, int *key, void* *value) {
    reinterpret_cast<KNHeap<int, void*>*>(instance)->getMin(key, value);
}
void  KNHeap__deleteMin(void* instance, int *key, void* *value) {
    reinterpret_cast<KNHeap<int, void*>*>(instance)->deleteMin(key, value);
}
void  KNHeap__insert(void* instance, int key, void* value) {
    reinterpret_cast<KNHeap<int, void*>*>(instance)->insert(key, value);
}
