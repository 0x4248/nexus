# x86_64 Linux Syscalls

In this chapter we will be looking at the x86_64 Linux syscalls, which are the
core interface between user-space applications and the kernel. They allow
applications to request services from the kernel, such as file operations, process
management, and network communication.


## Quick Syscall Reference Table

First here is a basic syscall table for x86_64 Linux with the most commonly used
syscalls. I have also had to abbreviate some of the types and parameters to fit 
them in the table, so I have also included a key for the abbreviations used in the table.

*These abbreviations are not used anywhere else in this manual*

- uns = unsigned
- con = const
- fn = filename
- fd = file descriptor
- fp = file path
- buf = buffer
- s_t = size_t
- ch = char
- st = struct
- lg = long
- off = offset
- __old_kernel_stat = __okst
- __kernel_old_stat = _kost
- __kernel_old_itimerval = _koitv
- __kernel_old_timeval = _kotv
- __kernel_old_timespec = _kots
- __kernel_timespec = _kts


| syscall name | rax | 0-rdi | 1-rsi | 2-rdx | 3-r10 | 4-r8 | 5-r9 |
|---|---|---|---|---|---|---|---|
| read | 0x00 | uns int fd | ch *buf | s_t count | - | - | - |
| write | 0x01 | uns int fd | con ch *buf | s_t count | - | - | - |
| open | 0x02 | con ch *fn | int flags | umode_t mode | - | - | - |
| close | 0x03 | uns int fd | - | - | - | - | - |
| stat | 0x04 | con ch *fn | st __okst *statbuf | - | - | - | - |
| fstat | 0x05 | uns int fd | st __okst *statbuf | - | - | - | - |
| lstat | 0x06 | con ch *fn | st __okst *statbuf | - | - | - | - |
| poll | 0x07 | st pollfd *ufds | uns int nfds | int timeout | - | - | - |
| lseek | 0x08 | uns int fd | off_t off | uns int whence | - | - | - |
| mmap | 0x09 | ? | ? | ? | ? | ? | ? |
| mprotect | 0x0a | uns lg start | s_t len | uns lg prot | - | - | - |
| munmap | 0x0b | uns lg addr | s_t len | - | - | - | - |
| brk | 0x0c | uns lg brk | - | - | - | - | - |
| rt_sigaction | 0x0d | int | con st sigaction * | st sigaction * | s_t | - | - |
| rt_sigprocmask | 0x0e | int how | sigset_t *set | sigset_t *oset | s_t sigsetsize | - | - |
| rt_sigreturn | 0x0f | ? | ? | ? | ? | ? | ? |
| ioctl | 0x10 | uns int fd | uns int cmd | uns lg arg | - | - | - |
| pread64 | 0x11 | uns int fd | ch *buf | s_t count | loff_t pos | - | - |
| pwrite64 | 0x12 | uns int fd | con ch *buf | s_t count | loff_t pos | - | - |
| readv | 0x13 | uns lg fd | con st iovec *vec | uns lg vlen | - | - | - |
| writev | 0x14 | uns lg fd | con st iovec *vec | uns lg vlen | - | - | - |
| access | 0x15 | con ch *fn | int mode | - | - | - | - |
| pipe | 0x16 | int *fildes | - | - | - | - | - |
| select | 0x17 | int n | fd_set *inp | fd_set *outp | fd_set *exp | st _kotv *tvp | - |
| sched_yield | 0x18 | - | - | - | - | - | - |
| mremap | 0x19 | uns lg addr | uns lg old_len | uns lg new_len | uns lg flags | uns lg new_addr | - |
| msync | 0x1a | uns lg start | s_t len | int flags | - | - | - |
| mincore | 0x1b | uns lg start | s_t len | uns ch * vec | - | - | - |
| madvise | 0x1c | uns lg start | s_t len | int behavior | - | - | - |
| shmget | 0x1d | key_t key | s_t size | int flag | - | - | - |
| shmat | 0x1e | int shmid | ch *shmaddr | int shmflg | - | - | - |
| shmctl | 0x1f | int shmid | int cmd | st shmid_ds *buf | - | - | - |
| dup | 0x20 | uns int fildes | - | - | - | - | - |
| dup2 | 0x21 | uns int oldfd | uns int newfd | - | - | - | - |
| pause | 0x22 | - | - | - | - | - | - |
| nanosleep | 0x23 | st _kts *rqtp | st _kts *rmtp | - | - | - | - |
| getitimer | 0x24 | int which | st _koitv *value | - | - | - | - |
| alarm | 0x25 | uns int seconds | - | - | - | - | - |
| setitimer | 0x26 | int which | st _koitv *value | st _koitv *ovalue | - | - | - |
| getpid | 0x27 | - | - | - | - | - | - |
| sendfile | 0x28 | int out_fd | int in_fd | off_t *off | s_t count | - | - |
| socket | 0x29 | int | int | int | - | - | - |
| connect | 0x2a | int | st sockaddr * | int | - | - | - |
| accept | 0x2b | int | st sockaddr * | int * | - | - | - |
| sendto | 0x2c | int | void * | s_t | uns | st sockaddr * | int |
| recvfrom | 0x2d | int | void * | s_t | uns | st sockaddr * | int * |
| sendmsg | 0x2e | int fd | st user_msghdr *msg | uns flags | - | - | - |
| recvmsg | 0x2f | int fd | st user_msghdr *msg | uns flags | - | - | - |
| shutdown | 0x30 | int | int | - | - | - | - |
| bind | 0x31 | int | st sockaddr * | int | - | - | - |
| listen | 0x32 | int | int | - | - | - | - |
| getsockname | 0x33 | int | st sockaddr * | int * | - | - | - |
| getpeername | 0x34 | int | st sockaddr * | int * | - | - | - |
| socketpair | 0x35 | int | int | int | int * | - | - |
| setsockopt | 0x36 | int fd | int level | int optname | ch *optval | int optlen | - |
| getsockopt | 0x37 | int fd | int level | int optname | ch *optval | int *optlen | - |
| clone | 0x38 | uns lg | uns lg | int * | int * | uns lg | - |
| fork | 0x39 | - | - | - | - | - | - |
| vfork | 0x3a | - | - | - | - | - | - |
| execve | 0x3b | con ch *fn | con ch *con *argv | con ch *con *envp | - | - | - |
| exit | 0x3c | int error_code | - | - | - | - | - |
| wait4 | 0x3d | pid_t pid | int *stat_addr | int options | st rusage *ru | - | - |
| kill | 0x3e | pid_t pid | int sig | - | - | - | - |
| uname | 0x3f | st old_utsname * | - | - | - | - | - |
| semget | 0x40 | key_t key | int nsems | int semflg | - | - | - |
| semop | 0x41 | int semid | st sembuf *sops | uns nsops | - | - | - |
| semctl | 0x42 | int semid | int semnum | int cmd | uns lg arg | - | - |
| shmdt | 0x43 | ch *shmaddr | - | - | - | - | - |
| msgget | 0x44 | key_t key | int msgflg | - | - | - | - |
| msgsnd | 0x45 | int msqid | st msgbuf *msgp | s_t msgsz | int msgflg | - | - |
| msgrcv | 0x46 | int msqid | st msgbuf *msgp | s_t msgsz | lg msgtyp | int msgflg | - |
| msgctl | 0x47 | int msqid | int cmd | st msqid_ds *buf | - | - | - |
| fcntl | 0x48 | uns int fd | uns int cmd | uns lg arg | - | - | - |
| flock | 0x49 | uns int fd | uns int cmd | - | - | - | - |
| fsync | 0x4a | uns int fd | - | - | - | - | - |
| fdatasync | 0x4b | uns int fd | - | - | - | - | - |
| truncate | 0x4c | con ch *path | lg length | - | - | - | - |
| ftruncate | 0x4d | uns int fd | off_t length | - | - | - | - |
| getdents | 0x4e | uns int fd | st linux_dirent *dirent | uns int count | - | - | - |
| getcwd | 0x4f | ch *buf | uns lg size | - | - | - | - |
| chdir | 0x50 | con ch *fn | - | - | - | - | - |
| fchdir | 0x51 | uns int fd | - | - | - | - | - |
| rename | 0x52 | con ch *oldname | con ch *newname | - | - | - | - |
| mkdir | 0x53 | con ch *pathname | umode_t mode | - | - | - | - |
| rmdir | 0x54 | con ch *pathname | - | - | - | - | - |
| creat | 0x55 | con ch *pathname | umode_t mode | - | - | - | - |
| link | 0x56 | con ch *oldname | con ch *newname | - | - | - | - |
| unlink | 0x57 | con ch *pathname | - | - | - | - | - |
| symlink | 0x58 | con ch *old | con ch *new | - | - | - | - |
| readlink | 0x59 | con ch *path | ch *buf | int bufsiz | - | - | - |
| chmod | 0x5a | con ch *fn | umode_t mode | - | - | - | - |
| fchmod | 0x5b | uns int fd | umode_t mode | - | - | - | - |
| chown | 0x5c | con ch *fn | uid_t user | gid_t group | - | - | - |
| fchown | 0x5d | uns int fd | uid_t user | gid_t group | - | - | - |
| lchown | 0x5e | con ch *fn | uid_t user | gid_t group | - | - | - |
| umask | 0x5f | int mask | - | - | - | - | - |
| gettimeofday | 0x60 | st _kotv *tv | st timezone *tz | - | - | - | - |
| getrlimit | 0x61 | uns int resource | st rlimit *rlim | - | - | - | - |
| getrusage | 0x62 | int who | st rusage *ru | - | - | - | - |
| sysinfo | 0x63 | st sysinfo *info | - | - | - | - | - |
| times | 0x64 | st tms *tbuf | - | - | - | - | - |
| ptrace | 0x65 | lg request | lg pid | uns lg addr | uns lg data | - | - |
| getuid | 0x66 | - | - | - | - | - | - |
| syslog | 0x67 | int type | ch *buf | int len | - | - | - |
| getgid | 0x68 | - | - | - | - | - | - |
| setuid | 0x69 | uid_t uid | - | - | - | - | - |
| setgid | 0x6a | gid_t gid | - | - | - | - | - |
| geteuid | 0x6b | - | - | - | - | - | - |
| getegid | 0x6c | - | - | - | - | - | - |
| setpgid | 0x6d | pid_t pid | pid_t pgid | - | - | - | - |
| getppid | 0x6e | - | - | - | - | - | - |
| getpgrp | 0x6f | - | - | - | - | - | - |
| setsid | 0x70 | - | - | - | - | - | - |
| setreuid | 0x71 | uid_t ruid | uid_t euid | - | - | - | - |
| setregid | 0x72 | gid_t rgid | gid_t egid | - | - | - | - |
| getgroups | 0x73 | int gidsetsize | gid_t *grouplist | - | - | - | - |
| setgroups | 0x74 | int gidsetsize | gid_t *grouplist | - | - | - | - |
| setresuid | 0x75 | uid_t ruid | uid_t euid | uid_t suid | - | - | - |

## Syscall Details

Now we are going to go over some of the details of the syscalls and document 
the parameters and return values and its behavior. This is not going to be an 
exhaustive list of all the syscalls, but I am slowly adding to this section.

### 0x00 - read
The `read` syscall reads data from a file descriptor into a buffer.

#### Synopsis

```c
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t count);
```


#### Example

```c
int fd = open("example.txt", O_RDONLY);
char buf[100];
ssize_t bytes_read = read(fd, buf, sizeof(buf));
if (bytes_read < 0) {
    // handle error
}
```

#### Parameters
- `fd`: The file descriptor to read from.
- `buf`: The buffer to read the data into.
- `count`: The number of bytes to read.

#### Return Value

The number of bytes read on success, or -1 on error. `errno` is set to indicate 
the error.

#### Error values

- `EAGAIN`: The file descriptor is non-blocking and no data is available to read.
- `EBADF`: The file descriptor is not valid or is not open for reading.
- `EFAULT`: The buffer is outside the accessible address space.
- `EINTR`: The call was interrupted by a signal before any data was read.
- `EINVAL`: The file descriptor cannot be read from, or the buffer is not valid.
- `EIO`: An I/O error occurred while reading from the file descriptor.
- `EISDIR`: The file descriptor refers to a directory.

### 0x01 - write
The `write` syscall writes data from a buffer to a file descriptor.

#### Synopsis

```c
#include <unistd.h>
ssize_t write(int fd, const void *buf, size_t count);
```

#### Example

```c
int fd = open("example.txt", O_WRONLY | O_CREAT, 0644);
const char *buf = "Hello, world!";
ssize_t bytes_written = write(fd, buf, strlen(buf));
if (bytes_written < 0) {
    // handle error
}
```

#### Parameters
- `fd`: The file descriptor to write to.
- `buf`: The buffer containing the data to write.
- `count`: The number of bytes to write.

#### Return Value
