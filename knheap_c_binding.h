void* KNHeap__create();
void KNHeap__destroy(void* instance);
int   KNHeap__getSize(void* instance);
void  KNHeap__getMin(void* instance, int *key, void* *value);
void  KNHeap__deleteMin(void* instance, int *key, void* *value);
void  KNHeap__insert(void* instance, int key, void* value);