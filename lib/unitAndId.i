# 1 "unitAndId.c"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "unitAndId.c"
# 13 "unitAndId.c"
# 1 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/stdlib.h" 1 3 4
# 25 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/stdlib.h" 3 4
#pragma ident "@(#)stdlib.h	1.48	00/04/13 SMI"

# 1 "/usr/include/iso/stdlib_iso.h" 1 3 4
# 28 "/usr/include/iso/stdlib_iso.h" 3 4
#pragma ident "@(#)stdlib_iso.h	1.3	02/10/10 SMI"

# 1 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/feature_tests.h" 1 3 4
# 22 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/feature_tests.h" 3 4
#pragma ident "@(#)feature_tests.h	1.18	99/07/26 SMI"

# 1 "/usr/include/sys/isa_defs.h" 1 3 4
# 9 "/usr/include/sys/isa_defs.h" 3 4
#pragma ident "@(#)isa_defs.h	1.20	99/05/04 SMI"
# 25 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/feature_tests.h" 2 3 4
# 31 "/usr/include/iso/stdlib_iso.h" 2 3 4






extern unsigned char __ctype[];
# 48 "/usr/include/iso/stdlib_iso.h" 3 4
typedef struct {
 int quot;
 int rem;
} div_t;

typedef struct {
 long quot;
 long rem;
} ldiv_t;






typedef unsigned int size_t;
# 84 "/usr/include/iso/stdlib_iso.h" 3 4
typedef long wchar_t;





extern void abort(void);
extern int abs(int);
extern int atexit(void (*)(void));
extern double atof(const char *);
extern int atoi(const char *);
extern long int atol(const char *);
extern void *bsearch(const void *, const void *, size_t, size_t,
 int (*)(const void *, const void *));
extern void *calloc(size_t, size_t);
extern div_t div(int, int);
extern void exit(int);
extern void free(void *);
extern char *getenv(const char *);
extern long int labs(long);
extern ldiv_t ldiv(long, long);
extern void *malloc(size_t);
extern int mblen(const char *, size_t);
extern size_t mbstowcs(wchar_t *, const char *, size_t);
extern int mbtowc(wchar_t *, const char *, size_t);
extern void qsort(void *, size_t, size_t,
 int (*)(const void *, const void *));
extern int rand(void);
extern void *realloc(void *, size_t);
extern void srand(unsigned int);
extern double strtod(const char *, char **);
extern long int strtol(const char *, char **, int);
extern unsigned long int strtoul(const char *, char **, int);
extern int system(const char *);
extern int wctomb(char *, wchar_t);
extern size_t wcstombs(char *, const wchar_t *, size_t);
# 28 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/stdlib.h" 2 3 4



# 1 "/usr/include/sys/wait.h" 1 3 4
# 16 "/usr/include/sys/wait.h" 3 4
#pragma ident "@(#)wait.h	1.21	97/04/08 SMI"



# 1 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/types.h" 1 3 4
# 25 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/types.h" 3 4
#pragma ident "@(#)types.h	1.75	04/01/06 SMI"







# 1 "/usr/include/sys/machtypes.h" 1 3 4
# 16 "/usr/include/sys/machtypes.h" 3 4
#pragma ident "@(#)machtypes.h	1.13	99/05/04 SMI"
# 37 "/usr/include/sys/machtypes.h" 3 4
typedef unsigned char lock_t;
# 34 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/types.h" 2 3 4
# 45 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/types.h" 3 4
# 1 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/int_types.h" 1 3 4
# 18 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/int_types.h" 3 4
#pragma ident "@(#)int_types.h	1.6	97/08/20 SMI"
# 71 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/int_types.h" 3 4
typedef char int8_t;





typedef short int16_t;
typedef int int32_t;




typedef long long int64_t;



typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;




typedef unsigned long long uint64_t;
# 103 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/int_types.h" 3 4
typedef int64_t intmax_t;
typedef uint64_t uintmax_t;
# 119 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/int_types.h" 3 4
typedef int intptr_t;
typedef unsigned int uintptr_t;







typedef char int_least8_t;





typedef short int_least16_t;
typedef int int_least32_t;




typedef long long int_least64_t;



typedef unsigned char uint_least8_t;
typedef unsigned short uint_least16_t;
typedef unsigned int uint_least32_t;




typedef unsigned long long uint_least64_t;
# 46 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/types.h" 2 3 4
# 62 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/types.h" 3 4
typedef long long longlong_t;
typedef unsigned long long u_longlong_t;
# 85 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/types.h" 3 4
typedef long t_scalar_t;
typedef unsigned long t_uscalar_t;





typedef unsigned char uchar_t;
typedef unsigned short ushort_t;
typedef unsigned int uint_t;
typedef unsigned long ulong_t;

typedef char *caddr_t;
typedef long daddr_t;
typedef short cnt_t;
# 112 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/types.h" 3 4
typedef int ptrdiff_t;
# 121 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/types.h" 3 4
typedef ulong_t pfn_t;
typedef ulong_t pgcnt_t;
typedef long spgcnt_t;

typedef uchar_t use_t;
typedef short sysid_t;
typedef short index_t;
typedef void *timeout_id_t;
typedef void *bufcall_id_t;
# 143 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/types.h" 3 4
typedef long off_t;
# 159 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/types.h" 3 4
typedef ulong_t ino_t;
typedef long blkcnt_t;
typedef ulong_t fsblkcnt_t;
typedef ulong_t fsfilcnt_t;
# 187 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/types.h" 3 4
typedef long blksize_t;



typedef enum { _B_FALSE, _B_TRUE } boolean_t;
# 209 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/types.h" 3 4
typedef int64_t pad64_t;
typedef uint64_t upad64_t;
# 222 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/types.h" 3 4
typedef union {
 long double _q;
 int32_t _l[4];
} pad128_t;

typedef union {
 long double _q;
 uint32_t _l[4];
} upad128_t;

typedef longlong_t offset_t;
typedef u_longlong_t u_offset_t;
typedef u_longlong_t len_t;
typedef u_longlong_t diskaddr_t;
# 253 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/types.h" 3 4
typedef union {
 offset_t _f;
 struct {
  int32_t _u;
  int32_t _l;
 } _p;
} lloff_t;
# 273 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/types.h" 3 4
typedef union {
 longlong_t _f;
 struct {
  int32_t _u;
  int32_t _l;
 } _p;
} lldaddr_t;


typedef uint_t k_fltset_t;
# 296 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/types.h" 3 4
typedef long id_t;






typedef uint_t useconds_t;



typedef long suseconds_t;
# 317 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/types.h" 3 4
typedef ulong_t major_t;
typedef ulong_t minor_t;





typedef short pri_t;
# 337 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/types.h" 3 4
typedef ushort_t o_mode_t;
typedef short o_dev_t;
typedef ushort_t o_uid_t;
typedef o_uid_t o_gid_t;
typedef short o_nlink_t;
typedef short o_pid_t;
typedef ushort_t o_ino_t;





typedef int key_t;



typedef ulong_t mode_t;







typedef long uid_t;



typedef uid_t gid_t;

typedef id_t taskid_t;
typedef id_t projid_t;






typedef uint_t pthread_t;
typedef uint_t pthread_key_t;







typedef struct _pthread_mutex {
 struct {
  uint16_t __pthread_mutex_flag1;
  uint8_t __pthread_mutex_flag2;
  uint8_t __pthread_mutex_ceiling;
  uint16_t __pthread_mutex_type;
  uint16_t __pthread_mutex_magic;
 } __pthread_mutex_flags;
 union {
  struct {
   uint8_t __pthread_mutex_pad[8];
  } __pthread_mutex_lock64;
  struct {
   uint32_t __pthread_ownerpid;
   uint32_t __pthread_lockword;
  } __pthread_mutex_lock32;
  upad64_t __pthread_mutex_owner64;
 } __pthread_mutex_lock;
 upad64_t __pthread_mutex_data;
} pthread_mutex_t;

typedef struct _pthread_cond {
 struct {
  uint8_t __pthread_cond_flag[4];
  uint16_t __pthread_cond_type;
  uint16_t __pthread_cond_magic;
 } __pthread_cond_flags;
 upad64_t __pthread_cond_data;
} pthread_cond_t;




typedef struct _pthread_rwlock {
 int32_t __pthread_rwlock_readers;
 uint16_t __pthread_rwlock_type;
 uint16_t __pthread_rwlock_magic;
 pthread_mutex_t __pthread_rwlock_mutex;
 pthread_cond_t __pthread_rwlock_readercv;
 pthread_cond_t __pthread_rwlock_writercv;
} pthread_rwlock_t;




typedef struct _pthread_attr {
 void *__pthread_attrp;
} pthread_attr_t;




typedef struct _pthread_mutexattr {
 void *__pthread_mutexattrp;
} pthread_mutexattr_t;




typedef struct _pthread_condattr {
 void *__pthread_condattrp;
} pthread_condattr_t;




typedef struct _once {
 upad64_t __pthread_once_pad[4];
} pthread_once_t;





typedef struct _pthread_rwlockattr {
 void *__pthread_rwlockattrp;
} pthread_rwlockattr_t;

typedef ulong_t dev_t;





typedef ulong_t nlink_t;
typedef long pid_t;
# 494 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/sys/types.h" 3 4
typedef int ssize_t;





typedef long time_t;




typedef long clock_t;




typedef int clockid_t;




typedef int timer_t;
# 21 "/usr/include/sys/wait.h" 2 3 4



# 1 "/usr/include/sys/resource.h" 1 3 4
# 16 "/usr/include/sys/resource.h" 3 4
#pragma ident "@(#)resource.h	1.25	98/06/30 SMI"




# 1 "/usr/include/sys/time.h" 1 3 4
# 23 "/usr/include/sys/time.h" 3 4
#pragma ident "@(#)time.h	2.67	03/08/07 SMI"
# 50 "/usr/include/sys/time.h" 3 4
struct timeval {
 time_t tv_sec;
 suseconds_t tv_usec;
};
# 156 "/usr/include/sys/time.h" 3 4
struct itimerval {
 struct timeval it_interval;
 struct timeval it_value;
};
# 204 "/usr/include/sys/time.h" 3 4
typedef longlong_t hrtime_t;
# 332 "/usr/include/sys/time.h" 3 4
int getitimer(int, struct itimerval *);
int utimes(const char *, const struct timeval *);

int setitimer(int, const struct itimerval *, struct itimerval *);
# 390 "/usr/include/sys/time.h" 3 4
int gettimeofday(struct timeval *, void *);
# 424 "/usr/include/sys/time.h" 3 4
# 1 "/usr/include/sys/select.h" 1 3 4
# 12 "/usr/include/sys/select.h" 3 4
#pragma ident "@(#)select.h	1.16	98/04/27 SMI"




# 1 "/usr/include/sys/time.h" 1 3 4
# 18 "/usr/include/sys/select.h" 2 3 4
# 47 "/usr/include/sys/select.h" 3 4
typedef long fds_mask;
# 75 "/usr/include/sys/select.h" 3 4
typedef struct __fd_set {

 long fds_bits[(((1024)+(((sizeof (fds_mask) * 8))-1))/((sizeof (fds_mask) * 8)))];
} fd_set;
# 97 "/usr/include/sys/select.h" 3 4
extern int select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
# 425 "/usr/include/sys/time.h" 2 3 4
# 22 "/usr/include/sys/resource.h" 2 3 4
# 67 "/usr/include/sys/resource.h" 3 4
typedef unsigned long rlim_t;
# 102 "/usr/include/sys/resource.h" 3 4
struct rlimit {
 rlim_t rlim_cur;
 rlim_t rlim_max;
};
# 143 "/usr/include/sys/resource.h" 3 4
struct rusage {
 struct timeval ru_utime;
 struct timeval ru_stime;
 long ru_maxrss;
 long ru_ixrss;
 long ru_idrss;
 long ru_isrss;
 long ru_minflt;
 long ru_majflt;
 long ru_nswap;
 long ru_inblock;
 long ru_oublock;
 long ru_msgsnd;
 long ru_msgrcv;
 long ru_nsignals;
 long ru_nvcsw;
 long ru_nivcsw;
};
# 193 "/usr/include/sys/resource.h" 3 4
extern int setrlimit(int, const struct rlimit *);
extern int getrlimit(int, struct rlimit *);
# 203 "/usr/include/sys/resource.h" 3 4
extern int getpriority(int, id_t);
extern int setpriority(int, id_t, int);
extern int getrusage(int, struct rusage *);
# 25 "/usr/include/sys/wait.h" 2 3 4
# 1 "/usr/include/sys/siginfo.h" 1 3 4
# 16 "/usr/include/sys/siginfo.h" 3 4
#pragma ident "@(#)siginfo.h	1.54	98/03/27 SMI"
# 34 "/usr/include/sys/siginfo.h" 3 4
union sigval {
 int sival_int;
 void *sival_ptr;
};
# 69 "/usr/include/sys/siginfo.h" 3 4
struct sigevent {
 int sigev_notify;
 int sigev_signo;
 union sigval sigev_value;
 void (*sigev_notify_function)(union sigval);
 pthread_attr_t *sigev_notify_attributes;
 int __sigev_pad2;
};
# 126 "/usr/include/sys/siginfo.h" 3 4
# 1 "/usr/include/sys/machsig.h" 1 3 4
# 16 "/usr/include/sys/machsig.h" 3 4
#pragma ident "@(#)machsig.h	1.15	99/08/15 SMI"
# 127 "/usr/include/sys/siginfo.h" 2 3 4
# 208 "/usr/include/sys/siginfo.h" 3 4
# 1 "/usr/include/sys/time_std_impl.h" 1 3 4
# 19 "/usr/include/sys/time_std_impl.h" 3 4
#pragma ident "@(#)time_std_impl.h	1.2	98/02/13 SMI"
# 32 "/usr/include/sys/time_std_impl.h" 3 4
typedef struct _timespec {
 time_t __tv_sec;
 long __tv_nsec;
} _timespec_t;

typedef struct _timespec _timestruc_t;
# 209 "/usr/include/sys/siginfo.h" 2 3 4
# 223 "/usr/include/sys/siginfo.h" 3 4
typedef struct {

 int si_signo;
 int si_code;
 int si_errno;



 union {

  int __pad[((128 / sizeof (int)) - 3)];

  struct {
   pid_t __pid;
   union {
    struct {
     uid_t __uid;


     union sigval __value;



    } __kill;
    struct {
     clock_t __utime;
     int __status;
     clock_t __stime;
    } __cld;
   } __pdata;
  } __proc;

  struct {
   void *__addr;
   int __trapno;
   caddr_t __pc;
  } __fault;

  struct {

   int __fd;
   long __band;
  } __file;

  struct {
   caddr_t __faddr;




   _timestruc_t __tstamp;

   short __syscall;
   char __nsysarg;
   char __fault;
   long __sysarg[8];
   int __mstate[10];
  } __prof;

 } __data;

} siginfo_t;
# 26 "/usr/include/sys/wait.h" 2 3 4
# 1 "/usr/include/sys/procset.h" 1 3 4
# 16 "/usr/include/sys/procset.h" 3 4
#pragma ident "@(#)procset.h	1.20	00/02/14 SMI"
# 41 "/usr/include/sys/procset.h" 3 4
typedef enum



  {
 P_PID,
 P_PPID,
 P_PGID,

 P_SID,
 P_CID,
 P_UID,
 P_GID,
 P_ALL,
 P_LWPID,
 P_TASKID,
 P_PROJID
} idtype_t;
# 27 "/usr/include/sys/wait.h" 2 3 4
# 86 "/usr/include/sys/wait.h" 3 4
extern pid_t wait(int *);
extern pid_t waitpid(pid_t, int *, int);


extern int waitid(idtype_t, id_t, siginfo_t *, int);
extern pid_t wait3(int *, int, struct rusage *);
# 32 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/stdlib.h" 2 3 4
# 77 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/stdlib.h" 3 4
typedef struct {
 long long quot;
 long long rem;
} lldiv_t;
# 118 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/stdlib.h" 3 4
extern int rand_r(unsigned int *);


extern void _exithandle(void);




extern double drand48(void);
extern double erand48(unsigned short *);
extern long jrand48(unsigned short *);
extern void lcong48(unsigned short *);
extern long lrand48(void);
extern long mrand48(void);
extern long nrand48(unsigned short *);
extern unsigned short *seed48(unsigned short *);
extern void srand48(long);
extern int putenv(char *);
extern void setkey(const char *);
# 159 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/stdlib.h" 3 4
extern int mkstemp(char *);
# 170 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/stdlib.h" 3 4
extern long a64l(const char *);
extern char *ecvt(double, int, int *, int *);
extern char *fcvt(double, int, int *, int *);
extern char *gcvt(double, int, char *);
extern int getsubopt(char **, char *const *, char **);
extern int grantpt(int);
extern char *initstate(unsigned, char *, size_t);
extern char *l64a(long);
extern char *mktemp(char *);
extern char *ptsname(int);
extern long random(void);
extern char *realpath(const char *, char *);
extern char *setstate(const char *);
extern void srandom(unsigned);
extern int ttyslot(void);
extern int unlockpt(int);
extern void *valloc(size_t);
# 14 "unitAndId.c" 2
# 1 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/string.h" 1 3 4
# 25 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/string.h" 3 4
#pragma ident "@(#)string.h	1.24	99/08/10 SMI"

# 1 "/usr/include/iso/string_iso.h" 1 3 4
# 28 "/usr/include/iso/string_iso.h" 3 4
#pragma ident "@(#)string_iso.h	1.2	99/11/09 SMI"
# 60 "/usr/include/iso/string_iso.h" 3 4
extern int memcmp(const void *, const void *, size_t);
extern void *memcpy(void *, const void *, size_t);
extern void *memmove(void *, const void *, size_t);
extern void *memset(void *, int, size_t);
extern char *strcat(char *, const char *);
extern int strcmp(const char *, const char *);
extern char *strcpy(char *, const char *);
extern int strcoll(const char *, const char *);
extern size_t strcspn(const char *, const char *);
extern char *strerror(int);
extern size_t strlen(const char *);
extern char *strncat(char *, const char *, size_t);
extern int strncmp(const char *, const char *, size_t);
extern char *strncpy(char *, const char *, size_t);
extern size_t strspn(const char *, const char *);
extern char *strtok(char *, const char *);
extern size_t strxfrm(char *, const char *, size_t);
# 112 "/usr/include/iso/string_iso.h" 3 4
extern void *memchr(const void *, int, size_t);
extern char *strchr(const char *, int);
extern char *strpbrk(const char *, const char *);
extern char *strrchr(const char *, int);
extern char *strstr(const char *, const char *);
# 28 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/string.h" 2 3 4
# 67 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/string.h" 3 4
extern char *strtok_r(char *, const char *, char **);




extern void *memccpy(void *, const void *, int, size_t);
# 88 "/opt/csw/gcc4/lib/gcc/sparc-sun-solaris2.8/4.0.2/include/string.h" 3 4
extern char *strdup(const char *);
# 15 "unitAndId.c" 2

# 1 "unitAndId.h" 1



# 1 "units.h" 1



# 1 "converter.h" 1
# 14 "converter.h"
typedef union cvConverter cvConverter;
# 23 "converter.h"
cvConverter*
cvGetTrivial();
# 33 "converter.h"
cvConverter*
cvGetInverse();
# 43 "converter.h"
cvConverter*
cvGetScale(
    const double slope);
# 57 "converter.h"
cvConverter*
cvGetOffset(
    const double intercept);
# 72 "converter.h"
cvConverter*
cvGetGalilean(
    const double slope,
    const double intercept);
# 89 "converter.h"
cvConverter*
cvGetLog(
    const double logE);
# 106 "converter.h"
cvConverter*
cvGetExp(
    const double logE);
# 124 "converter.h"
cvConverter*
cvCombine(
    cvConverter* const first,
    cvConverter* const second);
# 143 "converter.h"
cvConverter*
cvGetComposite(
    cvConverter* const first,
    cvConverter* const second);






void
cvFree(
    const cvConverter* const conv);
# 165 "converter.h"
float
cvConvertFloat(
    const cvConverter* converter,
    const float value);
# 178 "converter.h"
double
cvConvertDouble(
    const cvConverter* converter,
    const double value);
# 198 "converter.h"
float*
cvConvertFloats(
    const cvConverter* converter,
    const float* const in,
    const size_t count,
    float* out);
# 220 "converter.h"
double*
cvConvertDoubles(
    const cvConverter* converter,
    const double* const in,
    const size_t count,
    double* out);
# 239 "converter.h"
int
utGetExpression(
    const cvConverter* const conv,
    char* const buf,
    size_t max,
    const char* const variable);
# 5 "units.h" 2

typedef struct utSystem utSystem;
typedef union utUnit utUnit;


typedef enum utStatus {
    UT_SUCCESS = 0,
    UT_BADARG,
    UT_BADSYSTEM,
    UT_BADID,
    UT_BADVALUE,
    UT_EXISTS,
    UT_BADUNIT,
    UT_NOUNIT,
    UT_OS,
    UT_NOTSAMESYSTEM,
    UT_MEANINGLESS,
    UT_NOSECOND,
    UT_BADBUF,
    UT_BADVISITOR,
    UT_VISIT_ERROR,
    UT_CANT_FORMAT,
    UT_INTERNAL,
} utStatus;

extern enum utStatus unitStatus;


typedef enum {
    UT_ASCII = 0,
    UT_LATIN1 = 1,
    UT_UTF8 = 2
} utEncoding;
# 46 "units.h"
typedef struct {
    utStatus (*visitBasic)(const utUnit*, void*);
    utStatus (*visitProduct)(const utUnit*, int count,
 const utUnit* const* basicUnits, const int* powers, void*);
    utStatus (*visitGalilean)(const utUnit*, double scale,
 const utUnit* unit, double offset, void*);
    utStatus (*visitTimestamp)(const utUnit*, const utUnit* timeUnit,
 int year, int month, int day, int hour, int minute, double second,
 double resolution, void*);
    utStatus (*visitLogarithmic)(const utUnit*, double logE,
 const utUnit* reference, void*);
} utVisitor;
# 79 "units.h"
utStatus
utGetStatus();
# 91 "units.h"
utSystem*
utNewSystem();
# 104 "units.h"
utSystem*
utGetSystem(
    const utUnit* const unit);
# 120 "units.h"
utUnit*
utGetDimensionlessUnitOne(
    utSystem* const system);
# 138 "units.h"
utUnit*
utNewBaseUnit(
    utSystem* const system);
# 160 "units.h"
utUnit*
utNewDimensionlessUnit(
    utSystem* const system);
# 177 "units.h"
utStatus
utMapNameToUnit(
    const char* const name,
    utUnit* const unit);
# 196 "units.h"
utStatus
utMapSymbolToUnit(
    const char* const symbol,
    utUnit* const unit);
# 217 "units.h"
utStatus
utAddNamePrefix(
    utSystem* const system,
    const char* const name,
    const double value);
# 239 "units.h"
utStatus
utAddSymbolPrefix(
    utSystem* const system,
    const char* const symbol,
    const double value);
# 255 "units.h"
int
utSize(
    const utSystem* const system);
# 275 "units.h"
utUnit*
utGetUnitByName(
    utSystem* const system,
    const char* const name);
# 296 "units.h"
utUnit*
utGetUnitBySymbol(
    utSystem* const system,
    const char* const symbol);
# 315 "units.h"
utUnit*
utGetBaseUnit(
    const char* const name,
    const char* const symbol);
# 335 "units.h"
utStatus
utSetSecond(
    utSystem* const system,
    utUnit* const second);
# 351 "units.h"
int
utSameSystem(
    const utUnit* const unit1,
    const utUnit* const unit2);
# 369 "units.h"
int
utCompare(
    const utUnit* const unit1,
    const utUnit* const unit2);
# 390 "units.h"
utUnit*
utScale(
    const double factor,
    utUnit* const unit);
# 411 "units.h"
utUnit*
utOffset(
    utUnit* const unit,
    const double offset);
# 429 "units.h"
utUnit*
utMultiply(
    utUnit* const unit1,
    utUnit* const unit2);
# 446 "units.h"
utUnit*
utInvert(
    utUnit* const unit);
# 463 "units.h"
utUnit*
utDivide(
    utUnit* const numer,
    utUnit* const denom);
# 483 "units.h"
utUnit*
utRaise(
    utUnit* const unit,
    const int power);
# 513 "units.h"
utUnit*
utLog(
    const double logE,
    utUnit* const reference);
# 542 "units.h"
utUnit*
utOffsetByTime(
    utUnit* const unit,
    const int year,
    const int month,
    const int day,
    const int hour,
    const int minute,
    const double second);
# 563 "units.h"
utUnit*
utClone(
    const utUnit* unit);
# 577 "units.h"
void
utFree(
    const utUnit* const unit);
# 600 "units.h"
int
utAreConvertible(
    utUnit* const unit1,
    utUnit* const unit2);
# 620 "units.h"
cvConverter*
utGetConverter(
    utUnit* const from,
    utUnit* const to);
# 641 "units.h"
utUnit*
utParse(
    utSystem* const system,
    const char* const string,
    int* nchar);
# 661 "units.h"
utStatus
utAcceptVisitor(
    const utUnit* const unit,
    const utVisitor* const visitor,
    void* const arg);
# 694 "units.h"
int
utFormat(
    const utUnit* const unit,
    char* buf,
    size_t size,
    unsigned opts);
# 5 "unitAndId.h" 2

typedef struct {
    char* id;
    utUnit* unit;
} UnitAndId;
# 27 "unitAndId.h"
UnitAndId*
uaiNew(
    const utUnit* const unit,
    const char* const id);


void
uaiFree(
    UnitAndId* const node);
# 17 "unitAndId.c" 2


extern utStatus unitStatus;
# 32 "unitAndId.c"
UnitAndId*
uaiNew(
    const utUnit* const unit,
    const char* const id)
{
    UnitAndId* entry = 0;

    if (id == 0 || unit == 0) {
 unitStatus = UT_BADARG;
    }
    else {
 entry = malloc(sizeof(UnitAndId));

 if (entry == 0) {
     unitStatus = UT_OS;
 }
 else {
     unitStatus = UT_SUCCESS;

     entry->id = strdup(id);

     if (entry->id == 0) {
  unitStatus = UT_OS;
     }
     else {
  entry->unit = utClone(unit);

  if (entry->unit == 0) {
      assert(unitStatus != UT_SUCCESS);
      free(entry->id);
  }
     }

     if (unitStatus != UT_SUCCESS) {
  free(entry);
  entry = 0;
     }
 }
    }

    return entry;
}
# 82 "unitAndId.c"
void
uaiFree(
    UnitAndId* const entry)
{
    if (entry != 0) {
 free(entry->id);
 utFree(entry->unit);
 free(entry);
    }
}
