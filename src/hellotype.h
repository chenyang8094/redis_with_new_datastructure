#ifndef HELLO_TYPE_H
#define HELLO_TYPE_H
#include "server.h"

struct HelloTypeNode {
    int64_t value;
    struct HelloTypeNode *next;
};

typedef struct HelloTypeObject {
    struct HelloTypeNode *head;
    size_t len;
}HelloTypeObject;


HelloTypeObject *hellotypeNew(void);
void HelloTypeInsert(struct HelloTypeObject *o, int64_t ele);
void HelloTypeReleaseObject(struct HelloTypeObject *o);

#endif
