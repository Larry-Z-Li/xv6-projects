# Project 2: Kernel Threads and Mutexes in xv6

This project extends [xv6](https://github.com/mit-pdos/xv6-public), a small Unix-like teaching operating system written in C for 32-bit x86, with kernel-level threads and mutexes. It was built for USC's CSCI 350 (Operating Systems). The course's starting code already reorganized xv6 so that each process holds a fixed array of up to 16 threads. This project adds the system calls that let programs create and manage those threads, makes `exec` and `exit` handle several threads correctly, and adds mutexes that user programs can lock and unlock.

## Thread system calls

| System call | What it does |
| --- | --- |
| `int kthread_create(void *(*start_func)(), void *stack, int stack_size)` | Creates a new thread in the calling process. The new thread gets a copy of the caller's saved registers, with its stack pointer set to the top of the user stack the caller provides and its instruction pointer set to `start_func`. The thread is marked runnable, and the call returns its thread ID, or -1 if no thread could be allocated. |
| `int kthread_id()` | Returns the calling thread's ID. |
| `void kthread_exit()` | Ends the calling thread and wakes any thread waiting to join it. If it is the last thread in the process, the whole process exits. |
| `int kthread_join(int tid)` | Waits until the thread with ID `tid` in the same process has exited, then frees that thread's kernel stack. Returns 0 on success, or -1 if the ID is invalid or is the caller's own ID. |

## Threads and existing system calls

- **`exec`** first marks the process's other threads (those not currently running on a CPU) as finished, so that the thread calling `exec` is the one that continues into the new program.
- **`exit`** uses the same helper and also marks the process as killed before the process becomes a zombie.

## Mutexes

The kernel keeps a global table of 64 mutexes (`MAX_MUTEXES`). Each entry has an ID and a state: unused, unlocked or locked.

| System call | What it does |
| --- | --- |
| `int kthread_mutex_alloc()` | Finds a free entry, gives it a new ID and starts it unlocked. Returns the ID, or -1 if all 64 are in use. |
| `int kthread_mutex_dealloc(int mutex_id)` | Frees the mutex. Returns -1 if it is locked or does not exist. |
| `int kthread_mutex_lock(int mutex_id)` | Locks the mutex. If it is already locked, the calling thread sleeps until the mutex is unlocked, instead of spinning in a loop. |
| `int kthread_mutex_unlock(int mutex_id)` | Unlocks the mutex and wakes the threads waiting for it. Returns -1 if the mutex was not locked. |

As the assignment allowed, a mutex can be unlocked by a different thread from the one that locked it.

## Files changed

| File | Change |
| --- | --- |
| `proc.c` | Implementations of all the thread and mutex system calls, and the helpers `exec` and `exit` use to stop a process's other threads |
| `proc.h` | The mutex structure and its states, and a blocked thread state |
| `kthread.h` | Declarations of the eight system calls |
| `sysproc.c`, `syscall.c` | System call wrappers and their entries in the system call table |
| `exec.c` | Stops the process's other threads before running the new program |
| `main.c`, `defs.h` | Initializes the mutex table at boot, and adds the new declarations |

[`DESIGNDOC.txt`](DESIGNDOC.txt) summarizes these changes, and [`CSCI350Project2.pdf`](CSCI350Project2.pdf) is the assignment.

## Building and running

This project builds with the course's Docker environment, [cs350-docker](https://github.com/camerondurham/cs350-docker):

```bash
./run.sh start
./run.sh shell
```

Then, inside the xv6 source folder (`xv6-kernel-threads/`):

```bash
make clean
make qemu-nox
```

At the xv6 shell prompt, run the course's test programs:

- Threads: `threadtest1`, `threadtest2`, `threadtest3`
- Mutexes: `mutextest1`, `mutextest2`
