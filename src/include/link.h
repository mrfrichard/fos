//
// Created by root on 18-8-2.
//

#ifndef FOS_LINK_H
#define FOS_LINK_H

#include "mm.h"

typedef struct _link_node {
    void* data;
    struct _link_node* next;
} LINK_NODE;

static inline LINK_NODE* createLinkNode(void* data) {
    LINK_NODE* node = (LINK_NODE*) kmalloc(sizeof(LINK_NODE));
    if (node != NULL) {
        node->data = data;
        return node;
    }
    return NULL;
}

static inline LINK_NODE* createLink(LINK_NODE* node) {
    node->next = NULL;
    return node;
}

static inline void link_append(LINK_NODE* link, LINK_NODE* node) {
    LINK_NODE* link_node = link;
    while (link_node->next != NULL) {
        link_node = link_node->next;
    }
    link_node->next = node;
    node->next = NULL;
}

static inline void link_append(LINK_NODE** link, LINK_NODE* node) {
    if (*link == NULL) {
        *link = createLink(node);
    } else {
        link_append(*link, node);
    }
}
#endif //FOS_LINK_H
