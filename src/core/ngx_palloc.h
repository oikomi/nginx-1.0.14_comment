
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PALLOC_H_INCLUDED_
#define _NGX_PALLOC_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


/*
 * NGX_MAX_ALLOC_FROM_POOL should be (ngx_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
#define NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1)

#define NGX_DEFAULT_POOL_SIZE    (16 * 1024)

#define NGX_POOL_ALIGNMENT       16
#define NGX_MIN_POOL_SIZE                                                     \
    ngx_align((sizeof(ngx_pool_t) + 2 * sizeof(ngx_pool_large_t)),            \
              NGX_POOL_ALIGNMENT)


typedef void (*ngx_pool_cleanup_pt)(void *data);

typedef struct ngx_pool_cleanup_s  ngx_pool_cleanup_t;

struct ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;
    void                 *data;
    ngx_pool_cleanup_t   *next;
};


typedef struct ngx_pool_large_s  ngx_pool_large_t;
//���ڴ�ṹ
struct ngx_pool_large_s {
    ngx_pool_large_t     *next; //��һ������ڴ�
    void                 *alloc;//nginx����Ĵ���ڴ�ռ�
};

//�ýṹ����ά���ڴ�ص����ݿ飬���û�����֮��
typedef struct {
    u_char               *last;  //��ǰ�ڴ�������λ�ã�����һ�οɷ����ڴ����ʼλ��
    u_char               *end;   //�ڴ�ؽ���λ��
    ngx_pool_t           *next;  //���ӵ���һ���ڴ��
    ngx_uint_t            failed;//ͳ�Ƹ��ڴ�ز��������������Ĵ���
} ngx_pool_data_t;

//�ýṹά�������ڴ�ص�ͷ����Ϣ
struct ngx_pool_s {
    ngx_pool_data_t       d;       //���ݿ�
    size_t                max;     //���ݿ��С����С���ڴ�����ֵ
    ngx_pool_t           *current; //���浱ǰ�ڴ�ֵ
    ngx_chain_t          *chain;   //���Թ�һ��chain�ṹ
    ngx_pool_large_t     *large;   //�������ڴ��ã�������max���ڴ�����
    ngx_pool_cleanup_t   *cleanup; //����һЩ�ڴ���ͷŵ�ʱ��ͬʱ�ͷŵ���Դ
    ngx_log_t            *log;
};


typedef struct {
    ngx_fd_t              fd;
    u_char               *name;
    ngx_log_t            *log;
} ngx_pool_cleanup_file_t;

//ʹ��malloc�����ڴ�ռ�
void *ngx_alloc(size_t size, ngx_log_t *log);

//ʹ��malloc�����ڴ�ռ䣬���ҽ��ռ����ݳ�ʼ��Ϊ0
void *ngx_calloc(size_t size, ngx_log_t *log);

//�����ڴ��
ngx_pool_t *ngx_create_pool(size_t size, ngx_log_t *log);
void ngx_destroy_pool(ngx_pool_t *pool);
void ngx_reset_pool(ngx_pool_t *pool);

void *ngx_palloc(ngx_pool_t *pool, size_t size);    //pallocȡ�õ��ڴ��Ƕ����
void *ngx_pnalloc(ngx_pool_t *pool, size_t size);   //pnallocȡ�õ��ڴ��ǲ������
void *ngx_pcalloc(ngx_pool_t *pool, size_t size);   //pcallocֱ�ӵ���palloc������ڴ棬Ȼ�����һ��0��ʼ������
void *ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment); //�ڷ���size��С���ڴ棬������alignment���룬Ȼ��ҵ�large�ֶ���
ngx_int_t ngx_pfree(ngx_pool_t *pool, void *p);


ngx_pool_cleanup_t *ngx_pool_cleanup_add(ngx_pool_t *p, size_t size);
void ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);
void ngx_pool_cleanup_file(void *data);
void ngx_pool_delete_file(void *data);


#endif /* _NGX_PALLOC_H_INCLUDED_ */
