#include "server.h"
#include "hellotype.h"


HelloTypeObject *hellotypeNew(void){
    HelloTypeObject *h = zmalloc(sizeof(*h));
    h->head = NULL;
    h->len = 0;
    return h;
}

void HelloTypeInsert(struct HelloTypeObject *o, int64_t ele) {
    struct HelloTypeNode *next = o->head, *newnode, *prev = NULL;

    while(next && next->value < ele) {
        prev = next;
        next = next->next;
    }
    newnode = zmalloc(sizeof(*newnode));
    newnode->value = ele;
    newnode->next = next;
    if (prev) {
        prev->next = newnode;
    } else {
        o->head = newnode;
    }
    o->len++;
}
void HelloTypeReleaseObject(struct HelloTypeObject *o) {
    struct HelloTypeNode *cur, *next;
    cur = o->head;
    while(cur) {
        next = cur->next;
        zfree(cur);
        cur = next;
    }
    zfree(o);
}

/* HELLOTYPE.INSERT key value */
void htinsertCommand(client *c) {

     robj *o =o = lookupKeyWrite(c->db,c->argv[1]);

     if (o != NULL && checkType(c,o,OBJ_HELLO_TYPE)) return;

    long long value;
    if (!string2ll(c->argv[2]->ptr,sdslen(c->argv[2]->ptr), &value)) {
         addReplyError(c,"invalid value: must be a signed 64 bit integer");
         return;
    }

     /* Create an empty value object if the key is currently empty. */
     struct HelloTypeObject *hto = NULL;
     if (o == NULL) {
         o = createHelloTypeObject();
         dbAdd(c->db,c->argv[1],o);
     }
     hto = o->ptr;

     HelloTypeInsert(hto,value);

     addReplyLongLong(c,hto->len);
    return;
}


/* HELLOTYPE.RANGE key first count */
void htrangeCommand(client * c) {
    void *replylen = NULL;
    robj *o  = lookupKeyWrite(c->db,c->argv[1]);

    if (o != NULL && checkType(c,o,OBJ_HELLO_TYPE)) return;

    long long first, count;
    if (!string2ll(c->argv[2]->ptr,sdslen(c->argv[2]->ptr),&first) ||
        !string2ll(c->argv[3]->ptr,sdslen(c->argv[3]->ptr),&count) ||
        first < 0 || count < 0)
    {
         addReplyError(c,
            "invalid first or count parameters");
            return;
    }

    struct HelloTypeObject *hto = o ? o->ptr:NULL;
    struct HelloTypeNode *node = hto ? hto->head : NULL;

    replylen = addDeferredMultiBulkLength(c);
    long long arraylen = 0;
    while(node && count--) {
        addReplyLongLong(c,node->value);
        arraylen++;
        node = node->next;
    }
    setDeferredMultiBulkLength(c, replylen, arraylen);
    return ;
}

/* HELLOTYPE.LEN key */
void htlenCommand(client * c) {
   robj *o  = lookupKeyWrite(c->db,c->argv[1]);

   if (o != NULL && checkType(c,o,OBJ_HELLO_TYPE)) return;

    struct HelloTypeObject *hto = o ? o->ptr:NULL;
    addReplyLongLong(c,hto ? hto->len : 0);
    return ;
}
