# xv6 Operating System Projects

Three projects from USC's CSCI 350 (Operating Systems). Each one extends [xv6](https://github.com/mit-pdos/xv6-public), a small Unix-like teaching operating system written in C for 32-bit x86.

| Project | What it adds |
| --- | --- |
| [Project 2: Kernel threads and mutexes](project-2-lzli-master/) | System calls to create, join and exit kernel-level threads within a process, thread-aware `exec` and `exit`, and mutexes for user programs |
| [Project 3: Multilevel feedback queue scheduler](project-3-lzli-master/) | A scheduler with three priority queues, per-queue time slices, demotion and priority boosting, plus a `getpinfo` system call that prints each process's scheduling history |
| [Project 4: Copy-on-write `fork`](project-4-lzli-master/) | A `fork` that shares memory pages between parent and child until one of them writes, using read-only page-table entries, per-page counters and a page-fault handler |

Each project's README explains its design, the files it changes, and how to build and test it.
