
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */

//���鶨��ο���http://blog.csdn.net/sunhappy2200/article/details/5915189

#ifndef _NGX_ARRAY_H_INCLUDED_
#define _NGX_ARRAY_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


struct ngx_array_s {
    void        *elts;  //�����о�������������ָ��
    ngx_uint_t   nelts; //������ʵ�ʰ�����Ԫ������
    size_t       size;  //�����е���Ԫ�صĴ�С���ֽڣ�
    ngx_uint_t   nalloc; //����������������Ԥ�ȷ�����ڴ��С
    ngx_pool_t  *pool;  //�ڴ��
};

//���ڴ���д���n��Ԫ�ص����飬Ԫ�ش�СΪsize
ngx_array_t *ngx_array_create(ngx_pool_t *p, ngx_uint_t n, size_t size);

//��������䵽���ڴ��ͷŵ��ڴ��
void ngx_array_destroy(ngx_array_t *a);

//���ؽ�Ҫ��ӵ�������Ԫ�صĵ�ַ��������������������·���������nalloc*size)���ڴ�ռ䣬��nalloc����Ϊ2*nalloc
void *ngx_array_push(ngx_array_t *a);

//���ؽ�Ҫ���n��Ԫ�ص����������׸�Ԫ�صĵ�ַ
void *ngx_array_push_n(ngx_array_t *a, ngx_uint_t n);


static ngx_inline ngx_int_t
ngx_array_init(ngx_array_t *array, ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    /*
     * set "array->nelts" before "array->elts", otherwise MSVC thinks
     * that "array->nelts" may be used without having been initialized
     */

    array->nelts = 0;
    array->size = size;
    array->nalloc = n;
    array->pool = pool;

    array->elts = ngx_palloc(pool, n * size);
    if (array->elts == NULL) {
        return NGX_ERROR;
    }

    return NGX_OK;
}


#endif /* _NGX_ARRAY_H_INCLUDED_ */
