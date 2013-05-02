
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


#ifndef _NGX_QUEUE_H_INCLUDED_
#define _NGX_QUEUE_H_INCLUDED_


typedef struct ngx_queue_s  ngx_queue_t;

//�ο���
//http://blog.csdn.net/livelylittlefish/article/details/6607324
struct ngx_queue_s {
    ngx_queue_t  *prev;   //ǰһ��
    ngx_queue_t  *next;   //��һ��
};

//��ʼ������  
#define ngx_queue_init(q)                                                     \
    (q)->prev = q;                                                            \
    (q)->next = q

//�ж϶����Ƿ�Ϊ��
#define ngx_queue_empty(h)                                                    \
    (h == (h)->prev)

//��ͷ�ڵ�֮������½ڵ�
#define ngx_queue_insert_head(h, x)                                           \
    (x)->next = (h)->next;                                                    \
    (x)->next->prev = x;                                                      \
    (x)->prev = h;                                                            \
    (h)->next = x

#define ngx_queue_insert_after   ngx_queue_insert_head

//��β�ڵ�֮������½ڵ�
#define ngx_queue_insert_tail(h, x)                                           \
    (x)->prev = (h)->prev;                                                    \
    (x)->prev->next = x;                                                      \
    (x)->next = h;                                                            \
    (h)->prev = x

//ͷ�ڵ�
#define ngx_queue_head(h)                                                     \
    (h)->next

//β�ڵ�
#define ngx_queue_last(h)                                                     \
    (h)->prev

//ͷ����־�ڵ�
#define ngx_queue_sentinel(h)                                                 \
    (h)

//��һ���ڵ�
#define ngx_queue_next(q)                                                     \
    (q)->next

//��һ���ڵ�
#define ngx_queue_prev(q)                                                     \
    (q)->prev


#if (NGX_DEBUG)

//ɾ���ڵ�
#define ngx_queue_remove(x)                                                   \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next;                                              \
    (x)->prev = NULL;                                                         \
    (x)->next = NULL

#else

#define ngx_queue_remove(x)                                                   \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next

#endif

//�ָ�����
#define ngx_queue_split(h, q, n)                                              \
    (n)->prev = (h)->prev;                                                    \
    (n)->prev->next = n;                                                      \
    (n)->next = q;                                                            \
    (h)->prev = (q)->prev;                                                    \
    (h)->prev->next = h;                                                      \
    (q)->prev = n;

//���Ӷ���
#define ngx_queue_add(h, n)                                                   \
    (h)->prev->next = (n)->next;                                              \
    (n)->next->prev = (h)->prev;                                              \
    (h)->prev = (n)->prev;                                                    \
    (h)->prev->next = h;

//��ȡ�����нڵ����ݣ� q�Ƕ����еĽڵ㣬type�������ͣ�link�Ƕ���������ngx_queue_t��Ԫ����
#define ngx_queue_data(q, type, link)                                         \
    (type *) ((u_char *) q - offsetof(type, link))

//���е��м�ڵ�
ngx_queue_t *ngx_queue_middle(ngx_queue_t *queue);
void ngx_queue_sort(ngx_queue_t *queue,
    ngx_int_t (*cmp)(const ngx_queue_t *, const ngx_queue_t *));


#endif /* _NGX_QUEUE_H_INCLUDED_ */
