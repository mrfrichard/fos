//
// Created by root on 18-8-2.
//

#ifndef FOS_LINK_H
#define FOS_LINK_H

#include "type.h"
#include "const.h"
#include "mm.h"

typedef struct _link_node {
    void* data;
    struct _link_node* next;
} LINK_NODE;

typedef struct _link {
    T64 length;
    LINK_NODE* data;
} LINK;

static inline LINK_NODE* createLinkNode(void* data) {
    LINK_NODE* node = (LINK_NODE*) kmalloc(sizeof(LINK_NODE));
    if (node != NULL) {
        node->data = data;
        return node;
    }
    return NULL;
}

static inline LINK_NODE* initNodeLink(LINK_NODE* node) {
    if (node != NULL) {
        node->next = NULL;
    }
    return node;
}

static inline LINK_NODE* createNodeLink_data(void* data) {
    LINK_NODE* node = createLinkNode(data);
    if (node!= NULL) {
        return initNodeLink(node);
    }
    return NULL;
}

static inline LINK* createLink_node(LINK_NODE* node) {
    if (node != NULL) {
        LINK* link = (LINK*)kmalloc(sizeof(LINK));
        if (link != NULL) {
            link->length = 1;
            link->data = node;
        }
        return link;
    }
    return NULL;
}

static inline LINK* createLink_data(void* data) {
    LINK_NODE* node = createLinkNode(data);
    if (node!= NULL) {
        return createLink_node(node);
    }
    return NULL;
}

static inline void initLink(LINK* link) {
    link->length = 0;
    link->data = NULL;
}

static inline LINK* createLink() {
    LINK* link = (LINK*)kmalloc(sizeof(LINK));
    if (link != NULL) {
        link->length = 0;
        link->data = NULL;
    }
    return link;
}

static inline BOOL nodeLinkAppend_node(LINK_NODE* link, LINK_NODE* node) {
    if (link != NULL) {
        LINK_NODE* link_node = link;
        while (link_node->next != NULL) {
            link_node = link_node->next;
        }
        link_node->next = node;
        node->next = NULL;
        return TRUE;
    }
    return FALSE;
}

static inline BOOL nodeLinkCreateAppend_node(LINK_NODE** link, LINK_NODE* node) {
    if (*link == NULL) {
        *link = initNodeLink(node);
        if (*link != NULL) {
            return TRUE;
        }
        return FALSE;
    } else {
        return nodeLinkAppend_node(*link, node);
    }
}

static inline BOOL linkAppend_node(LINK* link, LINK_NODE* node) {
    if ((link != NULL) && (node != NULL)) {
        if (nodeLinkCreateAppend_node(&(link->data), node)) {
            link->length ++;
            return TRUE;
        }
    }
    return FALSE;
}

static inline BOOL linkCreateApppend_node(LINK** link, LINK_NODE* node) {
    if (*link == NULL) {
        *link = createLink_node(node);
        if (*link != NULL) {
            return TRUE;
        }
        return FALSE;
    } else {
        return linkAppend_node(*link, node);
    }
}

static inline BOOL nodelinkAppend_data(LINK_NODE* link, void* data) {
    LINK_NODE* node = createLinkNode(data);
    if (node!= NULL) {
        return nodeLinkAppend_node(link, node);
    }
    return FALSE;
}

static inline BOOL nodelinkCreateAppend_data(LINK_NODE** link, void* data) {
    if (*link == NULL) {
        *link = createNodeLink_data(data);
        if (*link != NULL) {
            return TRUE;
        }
        return FALSE;
    } else {
        return nodelinkAppend_data(*link, data);
    }
}

static inline BOOL linkAppend_date(LINK* link, void* data) {
    LINK_NODE* node = createLinkNode(data);
    if (node!= NULL) {
        return linkAppend_node(link, node);
    }
    return FALSE;
}

static inline BOOL linkCreateAppend_data(LINK** link, void* data) {
    if (*link == NULL) {
        *link = createLink_data(data);
        if (*link != NULL) {
            return TRUE;
        }
        return FALSE;
    } else {
        return linkAppend_date(*link, data);
    }
}
#endif //FOS_LINK_H
