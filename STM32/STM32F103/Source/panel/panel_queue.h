


#ifndef _PANEL_QUEUE_H_INCLUDED_
#define _PANEL_QUEUE_H_INCLUDED_


typedef struct panel_queue_s  panel_queue_t;

struct panel_queue_s {
    panel_queue_t  *prev;
    panel_queue_t  *next;
};


#define panel_queue_init(q)                                                     \
    (q)->prev = q;                                                            \
    (q)->next = q


#define panel_queue_empty(h)                                                    \
    (h == (h)->prev)


#define panel_queue_insert_head(h, x)                                           \
    (x)->next = (h)->next;                                                    \
    (x)->next->prev = x;                                                      \
    (x)->prev = h;                                                            \
    (h)->next = x


#define panel_queue_insert_after   panel_queue_insert_head


#define panel_queue_insert_tail(h, x)                                           \
    (x)->prev = (h)->prev;                                                    \
    (x)->prev->next = x;                                                      \
    (x)->next = h;                                                            \
    (h)->prev = x


#define panel_queue_head(h)                                                     \
    (h)->next


#define panel_queue_last(h)                                                     \
    (h)->prev


#define panel_queue_sentinel(h)                                                 \
    (h)


#define panel_queue_next(q)                                                     \
    (q)->next


#define panel_queue_prev(q)                                                     \
    (q)->prev


#if (panel_DEBUG)

#define panel_queue_remove(x)                                                   \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next;                                              \
    (x)->prev = NULL;                                                         \
    (x)->next = NULL

#else

#define panel_queue_remove(x)                                                   \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next

#endif


#define panel_queue_split(h, q, n)                                              \
    (n)->prev = (h)->prev;                                                    \
    (n)->prev->next = n;                                                      \
    (n)->next = q;                                                            \
    (h)->prev = (q)->prev;                                                    \
    (h)->prev->next = h;                                                      \
    (q)->prev = n;


#define panel_queue_add(h, n)                                                   \
    (h)->prev->next = (n)->next;                                              \
    (n)->next->prev = (h)->prev;                                              \
    (h)->prev = (n)->prev;                                                    \
    (h)->prev->next = h;


#define panel_queue_data(q, type, link)                                         \
    (type *) ((u_char *) q - offsetof(type, link))





#endif /* _PANEL_QUEUE_H_INCLUDED_ */
