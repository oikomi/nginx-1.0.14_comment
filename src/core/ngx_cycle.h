
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */
//�ο����ϣ�
//http://blog.csdn.net/benbendy1984/article/details/6007313

#ifndef _NGX_CYCLE_H_INCLUDED_
#define _NGX_CYCLE_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


#ifndef NGX_CYCLE_POOL_SIZE
#define NGX_CYCLE_POOL_SIZE     16384
#endif


#define NGX_DEBUG_POINTS_STOP   1
#define NGX_DEBUG_POINTS_ABORT  2


typedef struct ngx_shm_zone_s  ngx_shm_zone_t;

typedef ngx_int_t (*ngx_shm_zone_init_pt) (ngx_shm_zone_t *zone, void *data);

struct ngx_shm_zone_s {
    void                     *data;
    ngx_shm_t                 shm;
    ngx_shm_zone_init_pt      init;
    void                     *tag;
};

//�ο���
//http://blog.csdn.net/livelylittlefish/article/details/7247080
struct ngx_cycle_s {
    void                  ****conf_ctx; //�������������飨������ģ�飩
    ngx_pool_t               *pool;     //�ڴ��

    ngx_log_t                *log;      //��־
    ngx_log_t                 new_log;  //����־

    ngx_connection_t        **files;    //���socket��connection֮��Ĺ�ϵ
    ngx_connection_t         *free_connections;     //��������
    ngx_uint_t                free_connection_n;    // �������ӵĸ���

    ngx_queue_t               reusable_connections_queue;  //���������Ӷ���

    ngx_array_t               listening;        //�����׽��֣�Ԫ�ؽṹΪngx_listening_t
    ngx_array_t               pathes;           //ϵͳ��ʹ�õ�·�����ϣ�Ԫ�ؽṹΪngx_path_t
    ngx_list_t                open_files;       //���ļ��ļ��ϣ�Ԫ�ؽṹΪngx_open_file_t
    ngx_list_t                shared_memory;    //�����ڴ� ��Ԫ�ؽṹΪngx_shm_zone_t

    ngx_uint_t                connection_n;     //�ܵ�Ԥ�ȴ��������Ӹ���
    ngx_uint_t                files_n;          //���ļ�����

    ngx_connection_t         *connections;      //���е�����
    ngx_event_t              *read_events;      //���¼�
    ngx_event_t              *write_events;     //д�¼�

    ngx_cycle_t              *old_cycle;        //�ɵ�ȫ����Ϣ 

    ngx_str_t                 conf_file;        //�����ļ�
    ngx_str_t                 conf_param;       //���ò���
    ngx_str_t                 conf_prefix;      //�����ļ�ǰ׺ 
    ngx_str_t                 prefix;           //ϵͳ��װ·��ǰ׺ 
    ngx_str_t                 lock_file;        //���ļ�
    ngx_str_t                 hostname;         //������
};

//Coreģ������ýṹ��ngx_core_module_create_conf������ngx_core_module_init_conf��ʼ��
typedef struct {
     ngx_flag_t               daemon;              //�Ƿ�����daemon
     ngx_flag_t               master;

     ngx_msec_t               timer_resolution;

     ngx_int_t                worker_processes;    //�������ٸ�worker����
     ngx_int_t                debug_points;

     ngx_int_t                rlimit_nofile;
     ngx_int_t                rlimit_sigpending;
     off_t                    rlimit_core;

     int                      priority;

     ngx_uint_t               cpu_affinity_n;
     u_long                  *cpu_affinity;

     char                    *username;             /* �û��� */  
     ngx_uid_t                user;                 /* user ID */  
     ngx_gid_t                group;                /* group ID*/ 

     ngx_str_t                working_directory;
     ngx_str_t                lock_file;

     ngx_str_t                pid;
     ngx_str_t                oldpid;				/* ��'.oldbin' ��β */

     ngx_array_t              env;
     char                   **environment;

#if (NGX_THREADS)
     ngx_int_t                worker_threads;
     size_t                   thread_stack_size;
#endif

} ngx_core_conf_t;


typedef struct {
     ngx_pool_t              *pool;   /* pcre's malloc() pool */
} ngx_core_tls_t;


#define ngx_is_init_cycle(cycle)  (cycle->conf_ctx == NULL)


ngx_cycle_t *ngx_init_cycle(ngx_cycle_t *old_cycle);
ngx_int_t ngx_create_pidfile(ngx_str_t *name, ngx_log_t *log);
void ngx_delete_pidfile(ngx_cycle_t *cycle);
ngx_int_t ngx_signal_process(ngx_cycle_t *cycle, char *sig);
void ngx_reopen_files(ngx_cycle_t *cycle, ngx_uid_t user);
char **ngx_set_environment(ngx_cycle_t *cycle, ngx_uint_t *last);
ngx_pid_t ngx_exec_new_binary(ngx_cycle_t *cycle, char *const *argv);
u_long ngx_get_cpu_affinity(ngx_uint_t n);
ngx_shm_zone_t *ngx_shared_memory_add(ngx_conf_t *cf, ngx_str_t *name,
    size_t size, void *tag);


extern volatile ngx_cycle_t  *ngx_cycle;
extern ngx_array_t            ngx_old_cycles;
extern ngx_module_t           ngx_core_module;
extern ngx_uint_t             ngx_test_config;
extern ngx_uint_t             ngx_quiet_mode;
#if (NGX_THREADS)
extern ngx_tls_key_t          ngx_core_tls_key;
#endif


#endif /* _NGX_CYCLE_H_INCLUDED_ */
