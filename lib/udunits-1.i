# 1 "udunits-1.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "udunits-1.c"
# 9 "udunits-1.c"
# 1 "/usr/include/errno.h" 1 3 4
# 28 "/usr/include/errno.h" 3 4
# 1 "/usr/include/features.h" 1 3 4
# 375 "/usr/include/features.h" 3 4
# 1 "/usr/include/sys/cdefs.h" 1 3 4
# 392 "/usr/include/sys/cdefs.h" 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 393 "/usr/include/sys/cdefs.h" 2 3 4
# 376 "/usr/include/features.h" 2 3 4
# 399 "/usr/include/features.h" 3 4
# 1 "/usr/include/gnu/stubs.h" 1 3 4
# 10 "/usr/include/gnu/stubs.h" 3 4
# 1 "/usr/include/gnu/stubs-64.h" 1 3 4
# 11 "/usr/include/gnu/stubs.h" 2 3 4
# 400 "/usr/include/features.h" 2 3 4
# 29 "/usr/include/errno.h" 2 3 4






# 1 "/usr/include/bits/errno.h" 1 3 4
# 24 "/usr/include/bits/errno.h" 3 4
# 1 "/usr/include/linux/errno.h" 1 3 4
# 1 "/usr/include/asm/errno.h" 1 3 4
# 1 "/usr/include/asm-generic/errno.h" 1 3 4



# 1 "/usr/include/asm-generic/errno-base.h" 1 3 4
# 5 "/usr/include/asm-generic/errno.h" 2 3 4
# 1 "/usr/include/asm/errno.h" 2 3 4
# 1 "/usr/include/linux/errno.h" 2 3 4
# 25 "/usr/include/bits/errno.h" 2 3 4
# 50 "/usr/include/bits/errno.h" 3 4
extern int *__errno_location (void) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__));
# 36 "/usr/include/errno.h" 2 3 4
# 58 "/usr/include/errno.h" 3 4

# 10 "udunits-1.c" 2



# 1 "/usr/include/search.h" 1 3 4
# 25 "/usr/include/search.h" 3 4
# 1 "/usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stddef.h" 1 3 4
# 212 "/usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stddef.h" 3 4
typedef long unsigned int size_t;
# 26 "/usr/include/search.h" 2 3 4


# 44 "/usr/include/search.h" 3 4
extern void insque (void *__elem, void *__prev) __attribute__ ((__nothrow__ , __leaf__));


extern void remque (void *__elem) __attribute__ ((__nothrow__ , __leaf__));






typedef int (*__compar_fn_t) (const void *, const void *);







typedef enum
  {
    FIND,
    ENTER
  }
ACTION;

typedef struct entry
  {
    char *key;
    void *data;
  }
ENTRY;


struct _ENTRY;
# 87 "/usr/include/search.h" 3 4
extern ENTRY *hsearch (ENTRY __item, ACTION __action) __attribute__ ((__nothrow__ , __leaf__));


extern int hcreate (size_t __nel) __attribute__ ((__nothrow__ , __leaf__));


extern void hdestroy (void) __attribute__ ((__nothrow__ , __leaf__));
# 118 "/usr/include/search.h" 3 4
typedef enum
{
  preorder,
  postorder,
  endorder,
  leaf
}
VISIT;



extern void *tsearch (const void *__key, void **__rootp,
        __compar_fn_t __compar);



extern void *tfind (const void *__key, void *const *__rootp,
      __compar_fn_t __compar);


extern void *tdelete (const void *__restrict __key,
        void **__restrict __rootp,
        __compar_fn_t __compar);



typedef void (*__action_fn_t) (const void *__nodep, VISIT __value,
          int __level);




extern void twalk (const void *__root, __action_fn_t __action);
# 164 "/usr/include/search.h" 3 4
extern void *lfind (const void *__key, const void *__base,
      size_t *__nmemb, size_t __size, __compar_fn_t __compar);



extern void *lsearch (const void *__key, void *__base,
        size_t *__nmemb, size_t __size, __compar_fn_t __compar);


# 14 "udunits-1.c" 2

# 1 "/usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stddef.h" 1 3 4
# 147 "/usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stddef.h" 3 4
typedef long int ptrdiff_t;
# 324 "/usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stddef.h" 3 4
typedef int wchar_t;
# 16 "udunits-1.c" 2
# 1 "/usr/include/stdlib.h" 1 3 4
# 32 "/usr/include/stdlib.h" 3 4
# 1 "/usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stddef.h" 1 3 4
# 33 "/usr/include/stdlib.h" 2 3 4








# 1 "/usr/include/bits/waitflags.h" 1 3 4
# 42 "/usr/include/stdlib.h" 2 3 4
# 1 "/usr/include/bits/waitstatus.h" 1 3 4
# 64 "/usr/include/bits/waitstatus.h" 3 4
# 1 "/usr/include/endian.h" 1 3 4
# 36 "/usr/include/endian.h" 3 4
# 1 "/usr/include/bits/endian.h" 1 3 4
# 37 "/usr/include/endian.h" 2 3 4
# 60 "/usr/include/endian.h" 3 4
# 1 "/usr/include/bits/byteswap.h" 1 3 4
# 27 "/usr/include/bits/byteswap.h" 3 4
# 1 "/usr/include/bits/types.h" 1 3 4
# 27 "/usr/include/bits/types.h" 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 28 "/usr/include/bits/types.h" 2 3 4


typedef unsigned char __u_char;
typedef unsigned short int __u_short;
typedef unsigned int __u_int;
typedef unsigned long int __u_long;


typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;

typedef signed long int __int64_t;
typedef unsigned long int __uint64_t;







typedef long int __quad_t;
typedef unsigned long int __u_quad_t;
# 130 "/usr/include/bits/types.h" 3 4
# 1 "/usr/include/bits/typesizes.h" 1 3 4
# 131 "/usr/include/bits/types.h" 2 3 4


typedef unsigned long int __dev_t;
typedef unsigned int __uid_t;
typedef unsigned int __gid_t;
typedef unsigned long int __ino_t;
typedef unsigned long int __ino64_t;
typedef unsigned int __mode_t;
typedef unsigned long int __nlink_t;
typedef long int __off_t;
typedef long int __off64_t;
typedef int __pid_t;
typedef struct { int __val[2]; } __fsid_t;
typedef long int __clock_t;
typedef unsigned long int __rlim_t;
typedef unsigned long int __rlim64_t;
typedef unsigned int __id_t;
typedef long int __time_t;
typedef unsigned int __useconds_t;
typedef long int __suseconds_t;

typedef int __daddr_t;
typedef int __key_t;


typedef int __clockid_t;


typedef void * __timer_t;


typedef long int __blksize_t;




typedef long int __blkcnt_t;
typedef long int __blkcnt64_t;


typedef unsigned long int __fsblkcnt_t;
typedef unsigned long int __fsblkcnt64_t;


typedef unsigned long int __fsfilcnt_t;
typedef unsigned long int __fsfilcnt64_t;


typedef long int __fsword_t;

typedef long int __ssize_t;


typedef long int __syscall_slong_t;

typedef unsigned long int __syscall_ulong_t;



typedef __off64_t __loff_t;
typedef __quad_t *__qaddr_t;
typedef char *__caddr_t;


typedef long int __intptr_t;


typedef unsigned int __socklen_t;
# 28 "/usr/include/bits/byteswap.h" 2 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 29 "/usr/include/bits/byteswap.h" 2 3 4






# 1 "/usr/include/bits/byteswap-16.h" 1 3 4
# 36 "/usr/include/bits/byteswap.h" 2 3 4
# 44 "/usr/include/bits/byteswap.h" 3 4
static __inline unsigned int
__bswap_32 (unsigned int __bsx)
{
  return __builtin_bswap32 (__bsx);
}
# 108 "/usr/include/bits/byteswap.h" 3 4
static __inline __uint64_t
__bswap_64 (__uint64_t __bsx)
{
  return __builtin_bswap64 (__bsx);
}
# 61 "/usr/include/endian.h" 2 3 4
# 65 "/usr/include/bits/waitstatus.h" 2 3 4

union wait
  {
    int w_status;
    struct
      {

 unsigned int __w_termsig:7;
 unsigned int __w_coredump:1;
 unsigned int __w_retcode:8;
 unsigned int:16;







      } __wait_terminated;
    struct
      {

 unsigned int __w_stopval:8;
 unsigned int __w_stopsig:8;
 unsigned int:16;






      } __wait_stopped;
  };
# 43 "/usr/include/stdlib.h" 2 3 4
# 67 "/usr/include/stdlib.h" 3 4
typedef union
  {
    union wait *__uptr;
    int *__iptr;
  } __WAIT_STATUS __attribute__ ((__transparent_union__));
# 95 "/usr/include/stdlib.h" 3 4


typedef struct
  {
    int quot;
    int rem;
  } div_t;



typedef struct
  {
    long int quot;
    long int rem;
  } ldiv_t;







__extension__ typedef struct
  {
    long long int quot;
    long long int rem;
  } lldiv_t;


# 139 "/usr/include/stdlib.h" 3 4
extern size_t __ctype_get_mb_cur_max (void) __attribute__ ((__nothrow__ , __leaf__)) ;




extern double atof (const char *__nptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1))) ;

extern int atoi (const char *__nptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1))) ;

extern long int atol (const char *__nptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1))) ;





__extension__ extern long long int atoll (const char *__nptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1))) ;





extern double strtod (const char *__restrict __nptr,
        char **__restrict __endptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));





extern float strtof (const char *__restrict __nptr,
       char **__restrict __endptr) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));

extern long double strtold (const char *__restrict __nptr,
       char **__restrict __endptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));





extern long int strtol (const char *__restrict __nptr,
   char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));

extern unsigned long int strtoul (const char *__restrict __nptr,
      char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));




__extension__
extern long long int strtoq (const char *__restrict __nptr,
        char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));

__extension__
extern unsigned long long int strtouq (const char *__restrict __nptr,
           char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));





__extension__
extern long long int strtoll (const char *__restrict __nptr,
         char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));

__extension__
extern unsigned long long int strtoull (const char *__restrict __nptr,
     char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));

# 305 "/usr/include/stdlib.h" 3 4
extern char *l64a (long int __n) __attribute__ ((__nothrow__ , __leaf__)) ;


extern long int a64l (const char *__s)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1))) ;




# 1 "/usr/include/sys/types.h" 1 3 4
# 27 "/usr/include/sys/types.h" 3 4






typedef __u_char u_char;
typedef __u_short u_short;
typedef __u_int u_int;
typedef __u_long u_long;
typedef __quad_t quad_t;
typedef __u_quad_t u_quad_t;
typedef __fsid_t fsid_t;




typedef __loff_t loff_t;



typedef __ino_t ino_t;
# 60 "/usr/include/sys/types.h" 3 4
typedef __dev_t dev_t;




typedef __gid_t gid_t;




typedef __mode_t mode_t;




typedef __nlink_t nlink_t;




typedef __uid_t uid_t;





typedef __off_t off_t;
# 98 "/usr/include/sys/types.h" 3 4
typedef __pid_t pid_t;





typedef __id_t id_t;




typedef __ssize_t ssize_t;





typedef __daddr_t daddr_t;
typedef __caddr_t caddr_t;





typedef __key_t key_t;
# 132 "/usr/include/sys/types.h" 3 4
# 1 "/usr/include/time.h" 1 3 4
# 57 "/usr/include/time.h" 3 4


typedef __clock_t clock_t;



# 73 "/usr/include/time.h" 3 4


typedef __time_t time_t;



# 91 "/usr/include/time.h" 3 4
typedef __clockid_t clockid_t;
# 103 "/usr/include/time.h" 3 4
typedef __timer_t timer_t;
# 133 "/usr/include/sys/types.h" 2 3 4
# 146 "/usr/include/sys/types.h" 3 4
# 1 "/usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stddef.h" 1 3 4
# 147 "/usr/include/sys/types.h" 2 3 4



typedef unsigned long int ulong;
typedef unsigned short int ushort;
typedef unsigned int uint;
# 194 "/usr/include/sys/types.h" 3 4
typedef int int8_t __attribute__ ((__mode__ (__QI__)));
typedef int int16_t __attribute__ ((__mode__ (__HI__)));
typedef int int32_t __attribute__ ((__mode__ (__SI__)));
typedef int int64_t __attribute__ ((__mode__ (__DI__)));


typedef unsigned int u_int8_t __attribute__ ((__mode__ (__QI__)));
typedef unsigned int u_int16_t __attribute__ ((__mode__ (__HI__)));
typedef unsigned int u_int32_t __attribute__ ((__mode__ (__SI__)));
typedef unsigned int u_int64_t __attribute__ ((__mode__ (__DI__)));

typedef int register_t __attribute__ ((__mode__ (__word__)));
# 219 "/usr/include/sys/types.h" 3 4
# 1 "/usr/include/sys/select.h" 1 3 4
# 30 "/usr/include/sys/select.h" 3 4
# 1 "/usr/include/bits/select.h" 1 3 4
# 22 "/usr/include/bits/select.h" 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 23 "/usr/include/bits/select.h" 2 3 4
# 31 "/usr/include/sys/select.h" 2 3 4


# 1 "/usr/include/bits/sigset.h" 1 3 4
# 23 "/usr/include/bits/sigset.h" 3 4
typedef int __sig_atomic_t;




typedef struct
  {
    unsigned long int __val[(1024 / (8 * sizeof (unsigned long int)))];
  } __sigset_t;
# 34 "/usr/include/sys/select.h" 2 3 4



typedef __sigset_t sigset_t;





# 1 "/usr/include/time.h" 1 3 4
# 120 "/usr/include/time.h" 3 4
struct timespec
  {
    __time_t tv_sec;
    __syscall_slong_t tv_nsec;
  };
# 44 "/usr/include/sys/select.h" 2 3 4

# 1 "/usr/include/bits/time.h" 1 3 4
# 30 "/usr/include/bits/time.h" 3 4
struct timeval
  {
    __time_t tv_sec;
    __suseconds_t tv_usec;
  };
# 46 "/usr/include/sys/select.h" 2 3 4


typedef __suseconds_t suseconds_t;





typedef long int __fd_mask;
# 64 "/usr/include/sys/select.h" 3 4
typedef struct
  {






    __fd_mask __fds_bits[1024 / (8 * (int) sizeof (__fd_mask))];


  } fd_set;






typedef __fd_mask fd_mask;
# 96 "/usr/include/sys/select.h" 3 4

# 106 "/usr/include/sys/select.h" 3 4
extern int select (int __nfds, fd_set *__restrict __readfds,
     fd_set *__restrict __writefds,
     fd_set *__restrict __exceptfds,
     struct timeval *__restrict __timeout);
# 118 "/usr/include/sys/select.h" 3 4
extern int pselect (int __nfds, fd_set *__restrict __readfds,
      fd_set *__restrict __writefds,
      fd_set *__restrict __exceptfds,
      const struct timespec *__restrict __timeout,
      const __sigset_t *__restrict __sigmask);
# 131 "/usr/include/sys/select.h" 3 4

# 220 "/usr/include/sys/types.h" 2 3 4


# 1 "/usr/include/sys/sysmacros.h" 1 3 4
# 29 "/usr/include/sys/sysmacros.h" 3 4


__extension__
extern unsigned int gnu_dev_major (unsigned long long int __dev)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__));
__extension__
extern unsigned int gnu_dev_minor (unsigned long long int __dev)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__));
__extension__
extern unsigned long long int gnu_dev_makedev (unsigned int __major,
            unsigned int __minor)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__));
# 63 "/usr/include/sys/sysmacros.h" 3 4

# 223 "/usr/include/sys/types.h" 2 3 4





typedef __blksize_t blksize_t;






typedef __blkcnt_t blkcnt_t;



typedef __fsblkcnt_t fsblkcnt_t;



typedef __fsfilcnt_t fsfilcnt_t;
# 270 "/usr/include/sys/types.h" 3 4
# 1 "/usr/include/bits/pthreadtypes.h" 1 3 4
# 21 "/usr/include/bits/pthreadtypes.h" 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 22 "/usr/include/bits/pthreadtypes.h" 2 3 4
# 60 "/usr/include/bits/pthreadtypes.h" 3 4
typedef unsigned long int pthread_t;


union pthread_attr_t
{
  char __size[56];
  long int __align;
};

typedef union pthread_attr_t pthread_attr_t;





typedef struct __pthread_internal_list
{
  struct __pthread_internal_list *__prev;
  struct __pthread_internal_list *__next;
} __pthread_list_t;
# 90 "/usr/include/bits/pthreadtypes.h" 3 4
typedef union
{
  struct __pthread_mutex_s
  {
    int __lock;
    unsigned int __count;
    int __owner;

    unsigned int __nusers;



    int __kind;

    int __spins;
    __pthread_list_t __list;
# 115 "/usr/include/bits/pthreadtypes.h" 3 4
  } __data;
  char __size[40];
  long int __align;
} pthread_mutex_t;

typedef union
{
  char __size[4];
  int __align;
} pthread_mutexattr_t;




typedef union
{
  struct
  {
    int __lock;
    unsigned int __futex;
    __extension__ unsigned long long int __total_seq;
    __extension__ unsigned long long int __wakeup_seq;
    __extension__ unsigned long long int __woken_seq;
    void *__mutex;
    unsigned int __nwaiters;
    unsigned int __broadcast_seq;
  } __data;
  char __size[48];
  __extension__ long long int __align;
} pthread_cond_t;

typedef union
{
  char __size[4];
  int __align;
} pthread_condattr_t;



typedef unsigned int pthread_key_t;



typedef int pthread_once_t;





typedef union
{

  struct
  {
    int __lock;
    unsigned int __nr_readers;
    unsigned int __readers_wakeup;
    unsigned int __writer_wakeup;
    unsigned int __nr_readers_queued;
    unsigned int __nr_writers_queued;
    int __writer;
    int __shared;
    unsigned long int __pad1;
    unsigned long int __pad2;


    unsigned int __flags;

  } __data;
# 202 "/usr/include/bits/pthreadtypes.h" 3 4
  char __size[56];
  long int __align;
} pthread_rwlock_t;

typedef union
{
  char __size[8];
  long int __align;
} pthread_rwlockattr_t;





typedef volatile int pthread_spinlock_t;




typedef union
{
  char __size[32];
  long int __align;
} pthread_barrier_t;

typedef union
{
  char __size[4];
  int __align;
} pthread_barrierattr_t;
# 271 "/usr/include/sys/types.h" 2 3 4



# 315 "/usr/include/stdlib.h" 2 3 4






extern long int random (void) __attribute__ ((__nothrow__ , __leaf__));


extern void srandom (unsigned int __seed) __attribute__ ((__nothrow__ , __leaf__));





extern char *initstate (unsigned int __seed, char *__statebuf,
   size_t __statelen) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2)));



extern char *setstate (char *__statebuf) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));







struct random_data
  {
    int32_t *fptr;
    int32_t *rptr;
    int32_t *state;
    int rand_type;
    int rand_deg;
    int rand_sep;
    int32_t *end_ptr;
  };

extern int random_r (struct random_data *__restrict __buf,
       int32_t *__restrict __result) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));

extern int srandom_r (unsigned int __seed, struct random_data *__buf)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2)));

extern int initstate_r (unsigned int __seed, char *__restrict __statebuf,
   size_t __statelen,
   struct random_data *__restrict __buf)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2, 4)));

extern int setstate_r (char *__restrict __statebuf,
         struct random_data *__restrict __buf)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));






extern int rand (void) __attribute__ ((__nothrow__ , __leaf__));

extern void srand (unsigned int __seed) __attribute__ ((__nothrow__ , __leaf__));




extern int rand_r (unsigned int *__seed) __attribute__ ((__nothrow__ , __leaf__));







extern double drand48 (void) __attribute__ ((__nothrow__ , __leaf__));
extern double erand48 (unsigned short int __xsubi[3]) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));


extern long int lrand48 (void) __attribute__ ((__nothrow__ , __leaf__));
extern long int nrand48 (unsigned short int __xsubi[3])
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));


extern long int mrand48 (void) __attribute__ ((__nothrow__ , __leaf__));
extern long int jrand48 (unsigned short int __xsubi[3])
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));


extern void srand48 (long int __seedval) __attribute__ ((__nothrow__ , __leaf__));
extern unsigned short int *seed48 (unsigned short int __seed16v[3])
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));
extern void lcong48 (unsigned short int __param[7]) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));





struct drand48_data
  {
    unsigned short int __x[3];
    unsigned short int __old_x[3];
    unsigned short int __c;
    unsigned short int __init;
    unsigned long long int __a;
  };


extern int drand48_r (struct drand48_data *__restrict __buffer,
        double *__restrict __result) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));
extern int erand48_r (unsigned short int __xsubi[3],
        struct drand48_data *__restrict __buffer,
        double *__restrict __result) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));


extern int lrand48_r (struct drand48_data *__restrict __buffer,
        long int *__restrict __result)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));
extern int nrand48_r (unsigned short int __xsubi[3],
        struct drand48_data *__restrict __buffer,
        long int *__restrict __result)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));


extern int mrand48_r (struct drand48_data *__restrict __buffer,
        long int *__restrict __result)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));
extern int jrand48_r (unsigned short int __xsubi[3],
        struct drand48_data *__restrict __buffer,
        long int *__restrict __result)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));


extern int srand48_r (long int __seedval, struct drand48_data *__buffer)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2)));

extern int seed48_r (unsigned short int __seed16v[3],
       struct drand48_data *__buffer) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));

extern int lcong48_r (unsigned short int __param[7],
        struct drand48_data *__buffer)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));









extern void *malloc (size_t __size) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) ;

extern void *calloc (size_t __nmemb, size_t __size)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) ;










extern void *realloc (void *__ptr, size_t __size)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__warn_unused_result__));

extern void free (void *__ptr) __attribute__ ((__nothrow__ , __leaf__));




extern void cfree (void *__ptr) __attribute__ ((__nothrow__ , __leaf__));



# 1 "/usr/include/alloca.h" 1 3 4
# 24 "/usr/include/alloca.h" 3 4
# 1 "/usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stddef.h" 1 3 4
# 25 "/usr/include/alloca.h" 2 3 4







extern void *alloca (size_t __size) __attribute__ ((__nothrow__ , __leaf__));






# 492 "/usr/include/stdlib.h" 2 3 4





extern void *valloc (size_t __size) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) ;




extern int posix_memalign (void **__memptr, size_t __alignment, size_t __size)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1))) ;
# 512 "/usr/include/stdlib.h" 3 4


extern void abort (void) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__noreturn__));



extern int atexit (void (*__func) (void)) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));
# 529 "/usr/include/stdlib.h" 3 4





extern int on_exit (void (*__func) (int __status, void *__arg), void *__arg)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));






extern void exit (int __status) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__noreturn__));













extern void _Exit (int __status) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__noreturn__));






extern char *getenv (const char *__name) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1))) ;

# 577 "/usr/include/stdlib.h" 3 4
extern int putenv (char *__string) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));





extern int setenv (const char *__name, const char *__value, int __replace)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2)));


extern int unsetenv (const char *__name) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));






extern int clearenv (void) __attribute__ ((__nothrow__ , __leaf__));
# 605 "/usr/include/stdlib.h" 3 4
extern char *mktemp (char *__template) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));
# 619 "/usr/include/stdlib.h" 3 4
extern int mkstemp (char *__template) __attribute__ ((__nonnull__ (1))) ;
# 641 "/usr/include/stdlib.h" 3 4
extern int mkstemps (char *__template, int __suffixlen) __attribute__ ((__nonnull__ (1))) ;
# 662 "/usr/include/stdlib.h" 3 4
extern char *mkdtemp (char *__template) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1))) ;
# 711 "/usr/include/stdlib.h" 3 4





extern int system (const char *__command) ;

# 733 "/usr/include/stdlib.h" 3 4
extern char *realpath (const char *__restrict __name,
         char *__restrict __resolved) __attribute__ ((__nothrow__ , __leaf__)) ;
# 751 "/usr/include/stdlib.h" 3 4



extern void *bsearch (const void *__key, const void *__base,
        size_t __nmemb, size_t __size, __compar_fn_t __compar)
     __attribute__ ((__nonnull__ (1, 2, 5))) ;



extern void qsort (void *__base, size_t __nmemb, size_t __size,
     __compar_fn_t __compar) __attribute__ ((__nonnull__ (1, 4)));
# 770 "/usr/include/stdlib.h" 3 4
extern int abs (int __x) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__)) ;
extern long int labs (long int __x) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__)) ;



__extension__ extern long long int llabs (long long int __x)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__)) ;







extern div_t div (int __numer, int __denom)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__)) ;
extern ldiv_t ldiv (long int __numer, long int __denom)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__)) ;




__extension__ extern lldiv_t lldiv (long long int __numer,
        long long int __denom)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__)) ;

# 807 "/usr/include/stdlib.h" 3 4
extern char *ecvt (double __value, int __ndigit, int *__restrict __decpt,
     int *__restrict __sign) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3, 4))) ;




extern char *fcvt (double __value, int __ndigit, int *__restrict __decpt,
     int *__restrict __sign) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3, 4))) ;




extern char *gcvt (double __value, int __ndigit, char *__buf)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3))) ;




extern char *qecvt (long double __value, int __ndigit,
      int *__restrict __decpt, int *__restrict __sign)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3, 4))) ;
extern char *qfcvt (long double __value, int __ndigit,
      int *__restrict __decpt, int *__restrict __sign)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3, 4))) ;
extern char *qgcvt (long double __value, int __ndigit, char *__buf)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3))) ;




extern int ecvt_r (double __value, int __ndigit, int *__restrict __decpt,
     int *__restrict __sign, char *__restrict __buf,
     size_t __len) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3, 4, 5)));
extern int fcvt_r (double __value, int __ndigit, int *__restrict __decpt,
     int *__restrict __sign, char *__restrict __buf,
     size_t __len) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3, 4, 5)));

extern int qecvt_r (long double __value, int __ndigit,
      int *__restrict __decpt, int *__restrict __sign,
      char *__restrict __buf, size_t __len)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3, 4, 5)));
extern int qfcvt_r (long double __value, int __ndigit,
      int *__restrict __decpt, int *__restrict __sign,
      char *__restrict __buf, size_t __len)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3, 4, 5)));







extern int mblen (const char *__s, size_t __n) __attribute__ ((__nothrow__ , __leaf__)) ;


extern int mbtowc (wchar_t *__restrict __pwc,
     const char *__restrict __s, size_t __n) __attribute__ ((__nothrow__ , __leaf__)) ;


extern int wctomb (char *__s, wchar_t __wchar) __attribute__ ((__nothrow__ , __leaf__)) ;



extern size_t mbstowcs (wchar_t *__restrict __pwcs,
   const char *__restrict __s, size_t __n) __attribute__ ((__nothrow__ , __leaf__));

extern size_t wcstombs (char *__restrict __s,
   const wchar_t *__restrict __pwcs, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__));








extern int rpmatch (const char *__response) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1))) ;
# 895 "/usr/include/stdlib.h" 3 4
extern int getsubopt (char **__restrict __optionp,
        char *const *__restrict __tokens,
        char **__restrict __valuep)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2, 3))) ;
# 947 "/usr/include/stdlib.h" 3 4
extern int getloadavg (double __loadavg[], int __nelem)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));


# 1 "/usr/include/bits/stdlib-float.h" 1 3 4
# 952 "/usr/include/stdlib.h" 2 3 4
# 964 "/usr/include/stdlib.h" 3 4

# 17 "udunits-1.c" 2
# 1 "/usr/include/string.h" 1 3 4
# 27 "/usr/include/string.h" 3 4





# 1 "/usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stddef.h" 1 3 4
# 33 "/usr/include/string.h" 2 3 4









extern void *memcpy (void *__restrict __dest, const void *__restrict __src,
       size_t __n) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));


extern void *memmove (void *__dest, const void *__src, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));






extern void *memccpy (void *__restrict __dest, const void *__restrict __src,
        int __c, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));





extern void *memset (void *__s, int __c, size_t __n) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));


extern int memcmp (const void *__s1, const void *__s2, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));
# 92 "/usr/include/string.h" 3 4
extern void *memchr (const void *__s, int __c, size_t __n)
      __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));


# 123 "/usr/include/string.h" 3 4


extern char *strcpy (char *__restrict __dest, const char *__restrict __src)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));

extern char *strncpy (char *__restrict __dest,
        const char *__restrict __src, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));


extern char *strcat (char *__restrict __dest, const char *__restrict __src)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));

extern char *strncat (char *__restrict __dest, const char *__restrict __src,
        size_t __n) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));


extern int strcmp (const char *__s1, const char *__s2)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));

extern int strncmp (const char *__s1, const char *__s2, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));


extern int strcoll (const char *__s1, const char *__s2)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));

extern size_t strxfrm (char *__restrict __dest,
         const char *__restrict __src, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2)));






# 1 "/usr/include/xlocale.h" 1 3 4
# 27 "/usr/include/xlocale.h" 3 4
typedef struct __locale_struct
{

  struct __locale_data *__locales[13];


  const unsigned short int *__ctype_b;
  const int *__ctype_tolower;
  const int *__ctype_toupper;


  const char *__names[13];
} *__locale_t;


typedef __locale_t locale_t;
# 160 "/usr/include/string.h" 2 3 4


extern int strcoll_l (const char *__s1, const char *__s2, __locale_t __l)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2, 3)));

extern size_t strxfrm_l (char *__dest, const char *__src, size_t __n,
    __locale_t __l) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2, 4)));





extern char *strdup (const char *__s)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) __attribute__ ((__nonnull__ (1)));






extern char *strndup (const char *__string, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) __attribute__ ((__nonnull__ (1)));
# 207 "/usr/include/string.h" 3 4

# 232 "/usr/include/string.h" 3 4
extern char *strchr (const char *__s, int __c)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));
# 259 "/usr/include/string.h" 3 4
extern char *strrchr (const char *__s, int __c)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));


# 278 "/usr/include/string.h" 3 4



extern size_t strcspn (const char *__s, const char *__reject)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));


extern size_t strspn (const char *__s, const char *__accept)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));
# 311 "/usr/include/string.h" 3 4
extern char *strpbrk (const char *__s, const char *__accept)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));
# 338 "/usr/include/string.h" 3 4
extern char *strstr (const char *__haystack, const char *__needle)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));




extern char *strtok (char *__restrict __s, const char *__restrict __delim)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2)));




extern char *__strtok_r (char *__restrict __s,
    const char *__restrict __delim,
    char **__restrict __save_ptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2, 3)));

extern char *strtok_r (char *__restrict __s, const char *__restrict __delim,
         char **__restrict __save_ptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2, 3)));
# 393 "/usr/include/string.h" 3 4


extern size_t strlen (const char *__s)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));





extern size_t strnlen (const char *__string, size_t __maxlen)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));





extern char *strerror (int __errnum) __attribute__ ((__nothrow__ , __leaf__));

# 423 "/usr/include/string.h" 3 4
extern int strerror_r (int __errnum, char *__buf, size_t __buflen) __asm__ ("" "__xpg_strerror_r") __attribute__ ((__nothrow__ , __leaf__))

                        __attribute__ ((__nonnull__ (2)));
# 441 "/usr/include/string.h" 3 4
extern char *strerror_l (int __errnum, __locale_t __l) __attribute__ ((__nothrow__ , __leaf__));





extern void __bzero (void *__s, size_t __n) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));



extern void bcopy (const void *__src, void *__dest, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));


extern void bzero (void *__s, size_t __n) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));


extern int bcmp (const void *__s1, const void *__s2, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));
# 485 "/usr/include/string.h" 3 4
extern char *index (const char *__s, int __c)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));
# 513 "/usr/include/string.h" 3 4
extern char *rindex (const char *__s, int __c)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));




extern int ffs (int __i) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__));
# 532 "/usr/include/string.h" 3 4
extern int strcasecmp (const char *__s1, const char *__s2)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));


extern int strncasecmp (const char *__s1, const char *__s2, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));
# 555 "/usr/include/string.h" 3 4
extern char *strsep (char **__restrict __stringp,
       const char *__restrict __delim)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));




extern char *strsignal (int __sig) __attribute__ ((__nothrow__ , __leaf__));


extern char *__stpcpy (char *__restrict __dest, const char *__restrict __src)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));
extern char *stpcpy (char *__restrict __dest, const char *__restrict __src)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));



extern char *__stpncpy (char *__restrict __dest,
   const char *__restrict __src, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));
extern char *stpncpy (char *__restrict __dest,
        const char *__restrict __src, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));
# 642 "/usr/include/string.h" 3 4

# 18 "udunits-1.c" 2
# 1 "/usr/include/time.h" 1 3 4
# 29 "/usr/include/time.h" 3 4








# 1 "/usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stddef.h" 1 3 4
# 38 "/usr/include/time.h" 2 3 4



# 1 "/usr/include/bits/time.h" 1 3 4
# 42 "/usr/include/time.h" 2 3 4
# 131 "/usr/include/time.h" 3 4


struct tm
{
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;


  long int tm_gmtoff;
  const char *tm_zone;




};








struct itimerspec
  {
    struct timespec it_interval;
    struct timespec it_value;
  };


struct sigevent;
# 186 "/usr/include/time.h" 3 4



extern clock_t clock (void) __attribute__ ((__nothrow__ , __leaf__));


extern time_t time (time_t *__timer) __attribute__ ((__nothrow__ , __leaf__));


extern double difftime (time_t __time1, time_t __time0)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__));


extern time_t mktime (struct tm *__tp) __attribute__ ((__nothrow__ , __leaf__));





extern size_t strftime (char *__restrict __s, size_t __maxsize,
   const char *__restrict __format,
   const struct tm *__restrict __tp) __attribute__ ((__nothrow__ , __leaf__));

# 223 "/usr/include/time.h" 3 4
extern size_t strftime_l (char *__restrict __s, size_t __maxsize,
     const char *__restrict __format,
     const struct tm *__restrict __tp,
     __locale_t __loc) __attribute__ ((__nothrow__ , __leaf__));
# 236 "/usr/include/time.h" 3 4



extern struct tm *gmtime (const time_t *__timer) __attribute__ ((__nothrow__ , __leaf__));



extern struct tm *localtime (const time_t *__timer) __attribute__ ((__nothrow__ , __leaf__));





extern struct tm *gmtime_r (const time_t *__restrict __timer,
       struct tm *__restrict __tp) __attribute__ ((__nothrow__ , __leaf__));



extern struct tm *localtime_r (const time_t *__restrict __timer,
          struct tm *__restrict __tp) __attribute__ ((__nothrow__ , __leaf__));





extern char *asctime (const struct tm *__tp) __attribute__ ((__nothrow__ , __leaf__));


extern char *ctime (const time_t *__timer) __attribute__ ((__nothrow__ , __leaf__));







extern char *asctime_r (const struct tm *__restrict __tp,
   char *__restrict __buf) __attribute__ ((__nothrow__ , __leaf__));


extern char *ctime_r (const time_t *__restrict __timer,
        char *__restrict __buf) __attribute__ ((__nothrow__ , __leaf__));




extern char *__tzname[2];
extern int __daylight;
extern long int __timezone;




extern char *tzname[2];



extern void tzset (void) __attribute__ ((__nothrow__ , __leaf__));



extern int daylight;
extern long int timezone;





extern int stime (const time_t *__when) __attribute__ ((__nothrow__ , __leaf__));
# 319 "/usr/include/time.h" 3 4
extern time_t timegm (struct tm *__tp) __attribute__ ((__nothrow__ , __leaf__));


extern time_t timelocal (struct tm *__tp) __attribute__ ((__nothrow__ , __leaf__));


extern int dysize (int __year) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__));
# 334 "/usr/include/time.h" 3 4
extern int nanosleep (const struct timespec *__requested_time,
        struct timespec *__remaining);



extern int clock_getres (clockid_t __clock_id, struct timespec *__res) __attribute__ ((__nothrow__ , __leaf__));


extern int clock_gettime (clockid_t __clock_id, struct timespec *__tp) __attribute__ ((__nothrow__ , __leaf__));


extern int clock_settime (clockid_t __clock_id, const struct timespec *__tp)
     __attribute__ ((__nothrow__ , __leaf__));






extern int clock_nanosleep (clockid_t __clock_id, int __flags,
       const struct timespec *__req,
       struct timespec *__rem);


extern int clock_getcpuclockid (pid_t __pid, clockid_t *__clock_id) __attribute__ ((__nothrow__ , __leaf__));




extern int timer_create (clockid_t __clock_id,
    struct sigevent *__restrict __evp,
    timer_t *__restrict __timerid) __attribute__ ((__nothrow__ , __leaf__));


extern int timer_delete (timer_t __timerid) __attribute__ ((__nothrow__ , __leaf__));


extern int timer_settime (timer_t __timerid, int __flags,
     const struct itimerspec *__restrict __value,
     struct itimerspec *__restrict __ovalue) __attribute__ ((__nothrow__ , __leaf__));


extern int timer_gettime (timer_t __timerid, struct itimerspec *__value)
     __attribute__ ((__nothrow__ , __leaf__));


extern int timer_getoverrun (timer_t __timerid) __attribute__ ((__nothrow__ , __leaf__));
# 430 "/usr/include/time.h" 3 4

# 19 "udunits-1.c" 2

# 1 "udunits.h" 1
# 23 "udunits.h"
# 1 "udunits2.h" 1
# 11 "udunits2.h"
# 1 "/usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stdarg.h" 1 3 4
# 40 "/usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stdarg.h" 3 4
typedef __builtin_va_list __gnuc_va_list;
# 98 "/usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stdarg.h" 3 4
typedef __gnuc_va_list va_list;
# 12 "udunits2.h" 2
# 1 "/usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stddef.h" 1 3 4
# 13 "udunits2.h" 2
# 68 "udunits2.h"
# 1 "converter.h" 1
# 15 "converter.h"
# 1 "/usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stddef.h" 1 3 4
# 16 "converter.h" 2
# 25 "converter.h"
typedef union cv_converter cv_converter;
# 34 "converter.h"
 extern cv_converter*
cv_get_trivial(void);
# 44 "converter.h"
 extern cv_converter*
cv_get_inverse(void);
# 54 "converter.h"
 extern cv_converter*
cv_get_scale(
    const double slope);
# 68 "converter.h"
 extern cv_converter*
cv_get_offset(
    const double intercept);
# 83 "converter.h"
 extern cv_converter*
cv_get_galilean(
    const double slope,
    const double intercept);
# 100 "converter.h"
 extern cv_converter*
cv_get_log(
    const double base);
# 116 "converter.h"
 extern cv_converter*
cv_get_pow(
    const double base);
# 134 "converter.h"
 extern cv_converter*
cv_combine(
    cv_converter* const first,
    cv_converter* const second);






 extern void
cv_free(
    cv_converter* const conv);
# 156 "converter.h"
 extern float
cv_convert_float(
    const cv_converter* converter,
    const float value);
# 169 "converter.h"
 extern double
cv_convert_double(
    const cv_converter* converter,
    const double value);
# 186 "converter.h"
 extern float*
cv_convert_floats(
    const cv_converter* converter,
    const float* const in,
    const size_t count,
    float* out);
# 205 "converter.h"
 extern double*
cv_convert_doubles(
    const cv_converter* converter,
    const double* const in,
    const size_t count,
    double* out);
# 224 "converter.h"
 extern int
cv_get_expression(
    const cv_converter* const conv,
    char* const buf,
    size_t max,
    const char* const variable);
# 69 "udunits2.h" 2

typedef struct ut_system ut_system;
typedef union ut_unit ut_unit;

enum utStatus {
    UT_SUCCESS = 0,
    UT_BAD_ARG,
    UT_EXISTS,
    UT_NO_UNIT,
    UT_OS,
    UT_NOT_SAME_SYSTEM,
    UT_MEANINGLESS,
    UT_NO_SECOND,
    UT_VISIT_ERROR,
    UT_CANT_FORMAT,
    UT_SYNTAX,
    UT_UNKNOWN,
    UT_OPEN_ARG,
    UT_OPEN_ENV,
    UT_OPEN_DEFAULT,
    UT_PARSE
};
typedef enum utStatus ut_status;

enum utEncoding {
    UT_ASCII = 0,
    UT_ISO_8859_1 = 1,
    UT_LATIN1 = UT_ISO_8859_1,
    UT_UTF8 = 2
};
typedef enum utEncoding ut_encoding;
# 108 "udunits2.h"
typedef struct ut_visitor {
# 120 "udunits2.h"
    ut_status (*visit_basic)(const ut_unit* unit, void* arg);
# 137 "udunits2.h"
    ut_status (*visit_product)(const ut_unit* unit, int count,
 const ut_unit* const* basicUnits, const int* powers, void* arg);
# 155 "udunits2.h"
    ut_status (*visit_galilean)(const ut_unit* unit, double scale,
 const ut_unit* underlyingUnit, double offset, void* arg);
# 171 "udunits2.h"
    ut_status (*visit_timestamp)(const ut_unit* unit,
 const ut_unit* timeUnit, double origin, void* arg);
# 187 "udunits2.h"
    ut_status (*visit_logarithmic)(const ut_unit* unit, double base,
 const ut_unit* reference, void* arg);
} ut_visitor;


typedef int (*ut_error_message_handler)(const char* fmt, va_list args);
# 216 "udunits2.h"
 extern const char*
ut_get_path_xml(
 const char* path,
 ut_status* status);
# 247 "udunits2.h"
 extern ut_system*
ut_read_xml(
    const char* path);
# 261 "udunits2.h"
 extern ut_system*
ut_new_system(void);
# 273 "udunits2.h"
 extern void
ut_free_system(
    ut_system* system);
# 288 "udunits2.h"
 extern ut_system*
ut_get_system(
    const ut_unit* const unit);
# 306 "udunits2.h"
 extern ut_unit*
ut_get_dimensionless_unit_one(
    const ut_system* const system);
# 327 "udunits2.h"
 extern ut_unit*
ut_get_unit_by_name(
    const ut_system* const system,
    const char* const name);
# 350 "udunits2.h"
 extern ut_unit*
ut_get_unit_by_symbol(
    const ut_system* const system,
    const char* const symbol);
# 369 "udunits2.h"
 extern ut_status
ut_set_second(
    const ut_unit* const second);
# 394 "udunits2.h"
 extern ut_status
ut_add_name_prefix(
    ut_system* const system,
    const char* const name,
    const double value);
# 417 "udunits2.h"
 extern ut_status
ut_add_symbol_prefix(
    ut_system* const system,
    const char* const symbol,
    const double value);
# 443 "udunits2.h"
 extern ut_unit*
ut_new_base_unit(
    ut_system* const system);
# 464 "udunits2.h"
 extern ut_unit*
ut_new_dimensionless_unit(
    ut_system* const system);
# 482 "udunits2.h"
 extern ut_unit*
ut_clone(
    const ut_unit* unit);
# 495 "udunits2.h"
 extern void
ut_free(
    ut_unit* const unit);
# 519 "udunits2.h"
 extern const char*
ut_get_name(
    const ut_unit* const unit,
    const ut_encoding encoding);
# 540 "udunits2.h"
 extern ut_status
ut_map_name_to_unit(
    const char* const name,
    const ut_encoding encoding,
    const ut_unit* const unit);
# 559 "udunits2.h"
 extern ut_status
ut_unmap_name_to_unit(
    ut_system* system,
    const char* const name,
    const ut_encoding encoding);
# 582 "udunits2.h"
 extern ut_status
ut_map_unit_to_name(
    const ut_unit* const unit,
    const char* const name,
    ut_encoding encoding);
# 600 "udunits2.h"
 extern ut_status
ut_unmap_unit_to_name(
    const ut_unit* const unit,
    ut_encoding encoding);
# 625 "udunits2.h"
 extern const char*
ut_get_symbol(
    const ut_unit* const unit,
    const ut_encoding encoding);
# 646 "udunits2.h"
 extern ut_status
ut_map_symbol_to_unit(
    const char* const symbol,
    const ut_encoding encoding,
    const ut_unit* const unit);
# 665 "udunits2.h"
 extern ut_status
ut_unmap_symbol_to_unit(
    ut_system* system,
    const char* const symbol,
    const ut_encoding encoding);
# 687 "udunits2.h"
 extern ut_status
ut_map_unit_to_symbol(
    const ut_unit* unit,
    const char* const symbol,
    ut_encoding encoding);
# 706 "udunits2.h"
 extern ut_status
ut_unmap_unit_to_symbol(
    const ut_unit* const unit,
    ut_encoding encoding);
# 730 "udunits2.h"
 extern int
ut_is_dimensionless(
    const ut_unit* const unit);
# 749 "udunits2.h"
 extern int
ut_same_system(
    const ut_unit* const unit1,
    const ut_unit* const unit2);
# 769 "udunits2.h"
 extern int
ut_compare(
    const ut_unit* const unit1,
    const ut_unit* const unit2);
# 794 "udunits2.h"
 extern int
ut_are_convertible(
    const ut_unit* const unit1,
    const ut_unit* const unit2);
# 823 "udunits2.h"
 extern cv_converter*
ut_get_converter(
    ut_unit* const from,
    ut_unit* const to);
# 852 "udunits2.h"
 extern ut_unit*
ut_scale(
    const double factor,
    const ut_unit* const unit);
# 876 "udunits2.h"
 extern ut_unit*
ut_offset(
    const ut_unit* const unit,
    const double offset);
# 904 "udunits2.h"
 extern ut_unit*
ut_offset_by_time(
    const ut_unit* const unit,
    const double origin);
# 925 "udunits2.h"
 extern ut_unit*
ut_multiply(
    const ut_unit* const unit1,
    const ut_unit* const unit2);
# 944 "udunits2.h"
 extern ut_unit*
ut_invert(
    const ut_unit* const unit);
# 970 "udunits2.h"
 extern ut_unit*
ut_divide(
    const ut_unit* const numer,
    const ut_unit* const denom);
# 990 "udunits2.h"
 extern ut_unit*
ut_raise(
    const ut_unit* const unit,
    const int power);
# 1013 "udunits2.h"
 extern ut_unit*
ut_root(
    const ut_unit* const unit,
    const int root);
# 1058 "udunits2.h"
 extern ut_unit*
ut_log(
    const double base,
    const ut_unit* const reference);
# 1091 "udunits2.h"
 extern ut_unit*
ut_parse(
    const ut_system* const system,
    const char* const string,
    const ut_encoding encoding);
# 1108 "udunits2.h"
 extern char*
ut_trim(
    char* const string,
    const ut_encoding encoding);
# 1150 "udunits2.h"
 extern int
ut_format(
    const ut_unit* const unit,
    char* buf,
    size_t size,
    unsigned opts);
# 1170 "udunits2.h"
 extern ut_status
ut_accept_visitor(
    const ut_unit* const unit,
    const ut_visitor* const visitor,
    void* const arg);
# 1192 "udunits2.h"
 extern double
ut_encode_date(
    int year,
    int month,
    int day);
# 1209 "udunits2.h"
 extern double
ut_encode_clock(
    int hours,
    int minutes,
    double seconds);
# 1231 "udunits2.h"
 extern double
ut_encode_time(
    const int year,
    const int month,
    const int day,
    const int hour,
    const int minute,
    const double second);
# 1255 "udunits2.h"
 extern void
ut_decode_time(
    double value,
    int *year,
    int *month,
    int *day,
    int *hour,
    int *minute,
    double *second,
    double *resolution);
# 1276 "udunits2.h"
 extern ut_status
ut_get_status(void);
# 1287 "udunits2.h"
 extern void
ut_set_status(
    ut_status status);
# 1303 "udunits2.h"
 extern int
ut_handle_error_message(
    const char* const fmt,
    ...);
# 1320 "udunits2.h"
 extern ut_error_message_handler
ut_set_error_message_handler(
    ut_error_message_handler handler);
# 1337 "udunits2.h"
 extern int
ut_write_to_stderr(
    const char* const fmt,
    va_list args);
# 1352 "udunits2.h"
 extern int
ut_ignore(
    const char* const fmt,
    va_list args);
# 24 "udunits.h" 2
# 58 "udunits.h"
typedef double UtOrigin;
typedef double UtFactor;




typedef struct utUnit {
    ut_unit* unit2;
} utUnit;
# 75 "udunits.h"
 extern int utInit ( const char *path )

  ;




 extern int utIsInit (void);




 extern void utIni ( utUnit* const unit )

  ;




 extern int utScan ( const char *spec, utUnit *up )


  ;




 extern int utCalendar ( double value, const utUnit *unit, int *year, int *month, int *day, int *hour, int *minute, float *second )
# 111 "udunits.h"
  ;






 extern int utInvCalendar ( int year, int month, int day, int hour, int minute, double second, const utUnit *unit, double *value )
# 127 "udunits.h"
  ;




 extern int utIsTime ( const utUnit *up )

  ;




 extern int utHasOrigin ( const utUnit *up )

  ;




 extern utUnit* utClear ( utUnit *unit )

  ;




 extern utUnit* utCopy ( const utUnit *source, utUnit *dest )


  ;




 extern utUnit* utMultiply ( const utUnit *term1, const utUnit *term2, utUnit *result )



  ;




 extern utUnit* utDivide ( const utUnit *numer, const utUnit *denom, utUnit *result )



  ;




 extern utUnit* utInvert ( const utUnit *source, utUnit *dest )


  ;




 extern utUnit* utRaise ( const utUnit *source, int power, utUnit *result )



  ;




 extern utUnit* utShift ( const utUnit *source, double amount, utUnit *result )



  ;




 extern utUnit* utScale ( const utUnit *source, double factor, utUnit *result )



  ;




 extern int utConvert ( const utUnit *from, const utUnit *to, double *slope, double *intercept )




  ;




 extern int utPrint ( const utUnit *unit, char **buf )


  ;




 extern int utAdd ( char *name, int HasPlural, const utUnit *unit )



  ;




 extern int utFind ( char *spec, utUnit *up )


  ;




 extern void utFree ( utUnit* const unit )

  ;




 extern void utTerm (void);
# 21 "udunits-1.c" 2

static ut_system* unitSystem = ((void *)0);
static ut_unit* encodedTimeUnit = ((void *)0);
static ut_unit* second = ((void *)0);
static char* buffer;
static int buflen = 80;
static void* unit2s = ((void *)0);




int
utInit(
    const char *path)
{
    int status;

    (void)ut_set_error_message_handler(ut_ignore);
    if (unitSystem != ((void *)0)) {
 ut_free_system(unitSystem);
 unitSystem = ((void *)0);
    }
    unitSystem = ut_read_xml(((void *)0));
    if (unitSystem == ((void *)0)) {
 status = ut_get_status() == UT_PARSE
     ? -2
     : -4;
    }
    else {
 second = ut_get_unit_by_name(unitSystem, "second");
 encodedTimeUnit =
     ut_offset_by_time(second, ut_encode_time(2001, 1, 1, 0, 0, 0.0));
 buffer = malloc(buflen);
 if (buffer == ((void *)0)) {
     buflen = 0;
     status = -8;
 }
 else {
     status = 0;
 }
    }
    return status;
}




int
utIsInit()
{
    return unitSystem != ((void *)0);
}

static int
compare(
    const void* key1,
    const void* key2)
{
    return key1 < key2 ? -1 : key1 == key2 ? 0 : 1;
}

static void
freeIfAllocated(
    utUnit* const unit)
{
    if (tdelete(unit->unit2, &unit2s, compare) != ((void *)0)) {
 ut_free(unit->unit2);
    }
    unit->unit2 = ((void *)0);
}

void
utFree(
    utUnit* const unit)
{
    freeIfAllocated(unit);
}

void
utIni(
    utUnit* const unit)
{
    if (unit != ((void *)0)) {
 unit->unit2 = ((void *)0);
    }
}

static int
setUnit(
    utUnit* const unit,
    ut_unit* const unit2)
{
    int status;

    if (tsearch(unit2, &unit2s, compare) == ((void *)0)) {
 status = -8;
    }
    else {
 freeIfAllocated(unit);
 unit->unit2 = unit2;
 status = 0;
    }
    return status;
}




int
utScan(
    const char *spec,
    utUnit *unit)
{
    int status;

    if (spec == ((void *)0) || unit == ((void *)0)) {
 status = -5;
    }
    else {
 ut_unit* ut_unit = ut_parse(unitSystem, spec, UT_ASCII);

 if (ut_unit != ((void *)0)) {
     status = setUnit(unit, ut_unit);
 }
 else {
     status = ut_get_status();
     if (status == UT_BAD_ARG) {
  status = unitSystem == ((void *)0)
      ? -6
      : -5;
     }
     else if (status == UT_SYNTAX) {
  status = -2;
     }
     else if (status == UT_UNKNOWN) {
  status = -3;
     }
     else {
  status = -8;
     }
 }
    }
    return status;
}

static int isLeapYear(const int year)
{
    return (year % 4 == 0) && !(year % 100 == 0 && year % 400 != 0);
}




static int daysInMonth(
        const int year,
        const int month)
{
    static int daysInMonth[2][12] = {
            {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
            {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
    };
    return isLeapYear(year)
            ? daysInMonth[0][month-1]
            : daysInMonth[1][month-1];
}




int
utCalendar(
    double value,
    const utUnit *unit,
    int *year,
    int *month,
    int *day,
    int *hour,
    int *minute,
    float *second)
{
    int status = 0;

    cv_converter* converter = ut_get_converter(unit->unit2, encodedTimeUnit);
    if (converter == ((void *)0)) {
        status = encodedTimeUnit == ((void *)0) ? -6 : -5;
    }
    else {
        double encodedTime = cv_convert_double(converter, value);
        double sec, res;

        ut_decode_time(encodedTime, year, month, day, hour, minute, &sec, &res);
        *second = (float)sec;
        if (*second > 59) {
            *second = 0;
     *minute += 1;
     if (*minute > 59) {
         *minute = 0;
         *hour += 1;
         if (*hour > 23) {
             *hour = 0;
             *day += 1;
             if (*day > daysInMonth(*year, *month)) {
                 *day = 1;
                 *month += 1;
                 if (*month > 12) {
                     *month = 1;
                     *year += 1;
                 }
             }
         }
     }
 }
 cv_free(converter);
    }
    return status;
}






int
utInvCalendar(
    int year,
    int month,
    int day,
    int hour,
    int minute,
    double second,
    const utUnit *unit,
    double *value)
{
    int status = 0;

    cv_converter* converter = ut_get_converter(encodedTimeUnit, unit->unit2);
    if (converter == ((void *)0)) {
 status = encodedTimeUnit == ((void *)0) ? -6 : -5;
    }
    else {
 double encodedTime =
     ut_encode_time(year, month, day, hour, minute, second);

 *value = cv_convert_double(converter, encodedTime);
 cv_free(converter);
    }
    return status;
}

static ut_status
isTimeVisitBasic(
    const ut_unit* unit,
    void* arg)
{
    return (ut_status)ut_are_convertible(unit, second);
}

static ut_status
isTimeVisitProduct(
    const ut_unit* unit,
    int count,
    const ut_unit* const* basicUnits,
    const int* powers,
    void* arg)
{
    int isTime;
    if (!ut_are_convertible(unit, second)) {
 isTime = 0;
    }
    else {
 int i;

 isTime = 0;
 for (i = 0; i < count; i++) {
     if (ut_are_convertible(basicUnits[i], second) && powers[i] == 1) {
  isTime = 1;
  break;
     }
 }
    }
    return (ut_status)isTime;
}

static ut_status
isTimeVisitGalilean(
    const ut_unit* unit,
    double scale,
    const ut_unit* underlyingUnit,
    double origin,
    void* arg)
{
    return (ut_status)0;
}

static ut_status
isTimeVisitTimestamp(
    const ut_unit* unit,
    const ut_unit* timeUnit,
    double origin,
    void* arg)
{
    return (ut_status)1;
}

static ut_status
isTimeVisitLogarithmic(
    const ut_unit* unit,
    double base,
    const ut_unit* reference,
    void* arg)
{
    return (ut_status)0;
}




int
utIsTime(
    const utUnit *up)
{
    ut_visitor visitor;
    visitor.visit_basic = isTimeVisitBasic;
    visitor.visit_product = isTimeVisitProduct;
    visitor.visit_galilean = isTimeVisitGalilean;
    visitor.visit_timestamp = isTimeVisitTimestamp;
    visitor.visit_logarithmic = isTimeVisitLogarithmic;
    return ut_accept_visitor(up->unit2, &visitor, ((void *)0));
}

static ut_status
hasOriginVisitBasic(
    const ut_unit* unit,
    void* arg)
{
    return (ut_status)0;
}

static ut_status
hasOriginVisitProduct(
    const ut_unit* unit,
    int count,
    const ut_unit* const* basicUnits,
    const int* powers,
    void* arg)
{
    return (ut_status)0;
}

static ut_status
hasOriginVisitGalilean(
    const ut_unit* unit,
    double scale,
    const ut_unit* underlyingUnit,
    double origin,
    void* arg)
{
    return (ut_status)1;
}

static ut_status
hasOriginVisitTimestamp(
    const ut_unit* unit,
    const ut_unit* timeUnit,
    double origin,
    void* arg)
{
    return (ut_status)1;
}

static ut_status
hasOriginVisitLogarithmic(
    const ut_unit* unit,
    double base,
    const ut_unit* reference,
    void* arg)
{
    return (ut_status)0;
}




int
utHasOrigin(
    const utUnit *up)
{
    ut_visitor visitor;
    visitor.visit_basic = hasOriginVisitBasic;
    visitor.visit_product = hasOriginVisitProduct;
    visitor.visit_galilean = hasOriginVisitGalilean;
    visitor.visit_timestamp = hasOriginVisitTimestamp;
    visitor.visit_logarithmic = hasOriginVisitLogarithmic;
    return ut_accept_visitor(up->unit2, &visitor, ((void *)0));
}

static utUnit*
resultingUnit(
    utUnit* result,
    ut_unit* const unit2)
{
    if (unit2 == ((void *)0)) {
 result = ((void *)0);
    }
    else if (result != ((void *)0)) {
 if (setUnit(result, unit2) != 0) {
     result = ((void *)0);
 }
    }
    return result;
}




utUnit*
utClear(
    utUnit *unit)
{
    return resultingUnit(unit, ut_get_dimensionless_unit_one(unitSystem));
}




utUnit*
utCopy(
    const utUnit *source,
    utUnit *dest)
{
    return source == ((void *)0)
 ? ((void *)0)
 : dest == ((void *)0)
     ? ((void *)0)
     : resultingUnit(dest, ut_clone(source->unit2));
}




utUnit*
utMultiply(
    const utUnit *term1,
    const utUnit *term2,
    utUnit *result)
{
    return term1 == ((void *)0) || term2 == ((void *)0)
 ? ((void *)0)
 : resultingUnit(result, ut_multiply(term1->unit2, term2->unit2));
}




utUnit*
utDivide(
    const utUnit *numer,
    const utUnit *denom,
    utUnit *result)
{
    return numer == ((void *)0) || denom == ((void *)0)
 ? ((void *)0)
 : resultingUnit(result, ut_divide(numer->unit2, denom->unit2));
}




utUnit*
utInvert(
    const utUnit *unit,
    utUnit *result)
{
    return unit == ((void *)0) ? ((void *)0) : resultingUnit(result, ut_invert(unit->unit2));
}




utUnit*
utRaise(
    const utUnit *unit,
    int power,
    utUnit *result)
{
    return unit == ((void *)0)
 ? ((void *)0)
 : resultingUnit(result, ut_raise(unit->unit2, power));
}




utUnit*
utShift(
    const utUnit *unit,
    double amount,
    utUnit *result)
{
    return unit == ((void *)0)
 ? ((void *)0)
 : resultingUnit(result, ut_offset(unit->unit2, amount));
}




utUnit*
utScale(
    const utUnit *unit,
    double factor,
    utUnit *result)
{
    return unit == ((void *)0)
 ? ((void *)0)
 : resultingUnit(result, ut_scale(factor, unit->unit2));
}




int
utConvert(
    const utUnit *from,
    const utUnit *to,
    double *slope,
    double *intercept)
{
    int status;
    cv_converter* converter = ut_get_converter(from->unit2, to->unit2);

    if (converter == ((void *)0)) {
 status = ut_get_status();

 if (status == UT_BAD_ARG) {
     status = -5;
 }
 else if (status == UT_NOT_SAME_SYSTEM) {
     status = -6;
 }
 else if (status == UT_MEANINGLESS) {
     status = -7;
 }
 else {
     status = -8;
 }
    }
    else {
 *intercept = cv_convert_double(converter, 0.0);
 *slope = cv_convert_double(converter, 1.0) - *intercept;
 status = 0;
    }
    return status;
}




int
utPrint(
    const utUnit *unit,
    char **buf)
{
    int status;

    for (;;) {
 int len = ut_format(unit->unit2, buffer, buflen, UT_ASCII);
 if (len == -1) {
     status = ut_get_status();

     if (status == UT_BAD_ARG) {
  status = -5;
     }
     else {
  status = -8;
     }
     break;
 }
 if (len < buflen) {
     *buf = buffer;
     status = 0;
     break;
 }
 else {
     int newLen = buflen * 2;
     char* newBuf = malloc(newLen);

     if (newBuf == ((void *)0)) {
  status = -8;
  break;
     }
     buffer = newBuf;
     buflen = newLen;
 }
    }
    return status;
}




int
utAdd(
    char *name,
    int hasPlural,
    const utUnit *unit)
{
    int status = ut_map_name_to_unit(name, UT_ASCII, unit->unit2);

    if (status == UT_SUCCESS) {
 status = ut_map_unit_to_name(unit->unit2, name, UT_ASCII);
 if (status == UT_SUCCESS) {
     if (!hasPlural) {
  status = UT_SUCCESS;
     }
     else {
  extern const char* ut_form_plural(const char*);
  const char* plural = ut_form_plural(name);

  status = ut_map_name_to_unit(plural, UT_ASCII, unit->unit2);
     }
     if (status != UT_SUCCESS) {
  (void)ut_unmap_unit_to_name(unit->unit2, UT_ASCII);
     }
 }
 if (status != UT_SUCCESS) {
     (void)ut_unmap_name_to_unit(unitSystem, name, UT_ASCII);
 }
    }
    return status == UT_SUCCESS
 ? 0
 : status == UT_EXISTS
     ? -11
     : -8;
}





int
utFind(
    char *spec,
    utUnit *up)
{
    int status;
    ut_unit* unit = ut_parse(unitSystem, spec, UT_ASCII);

    if (unit == ((void *)0)) {
 status = ut_get_status();

 if (status == UT_BAD_ARG) {
     status = -5;
 }
 else if (status == UT_SYNTAX) {
     status = -2;
 }
 else if (status == UT_UNKNOWN) {
     status = -3;
 }
 else if (status == UT_OS) {
     status = -8;
 }
    }
    else {
 status = setUnit(up, unit);
    }
    return status;
}




void
utTerm()
{
    ut_free(second);
    second = ((void *)0);
    ut_free(encodedTimeUnit);
    encodedTimeUnit = ((void *)0);
    ut_free_system(unitSystem);
    unitSystem = ((void *)0);
}
