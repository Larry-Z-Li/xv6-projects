# Project 4: Copy-on-Write `fork` for xv6

This project adds copy-on-write to `fork` in [xv6](https://github.com/mit-pdos/xv6-public), a small Unix-like teaching operating system written in C for 32-bit x86. It was built for USC's CSCI 350 (Operating Systems).

## The problem

In the original xv6, `fork` copies every page of the parent's memory into the child. That work is often wasted: the child may change very little of its memory, or it may call `exec` right away and throw the copy out. With copy-on-write, the parent and child share the same physical pages, and a page is copied only when one of them writes to it.

## What this project adds

### 1. Copy-on-write `fork` (`vm.c`, `proc.c`)

- `fork` calls a new function, `cow()`, instead of `copyuvm()`. `cow()` builds the child's page table so that it points to the same physical pages as the parent's, instead of copying them.
- In both the parent's and the child's page-table entries, `cow()` clears the writable bit (`PTE_W`) and sets a new shared bit, `PTE_S` (`0x800`, one of the page-table bits x86 leaves free for the operating system).
- It then reloads the CR3 register, which makes the CPU discard its cached translations (the TLB), so the parent's old writable entries stop being used.
- A table with one counter per physical page, protected by a spinlock, tracks how many additional processes share each page.

### 2. Page-fault handler (`trap.c`, `vm.c`)

Writing to a read-only page makes the CPU raise a page fault (trap 14). The new `pagefault()` function reads the faulting address from the CR2 register, then:

- **If other processes still share the page,** it allocates a new page, copies the 4,096 bytes into it, maps the copy as writable for this process only, and decrements the page's counter.
- **If this process is the last one using the page,** it simply makes the page writable again.

### 3. Freeing shared pages (`vm.c`)

When a process's memory is freed, `deallocuvm()` frees a physical page only if no other process shares it. Otherwise, it just decrements the page's counter.

### 4. Page-mapping viewer (`proc.c`)

Pressing Ctrl+P (`^P`) in xv6, or calling the new `procdump()` system call, prints every process along with its user pages: each virtual page number, the physical page number it maps to, and whether it is writable (`y` or `n`). The viewer reads each process's first page table, which covers virtual addresses 0 to 4 MB.

## Testing

`testcow`, provided by the course, allocates a buffer, forks, and has the child write to every byte of the buffer. It calls `procdump()` before and after the writes, so the printed mappings can be compared. After the fork, the parent and child map the same physical pages as read-only. After the child writes, the pages it wrote to point to new physical pages and are writable again.

## Files changed

| File | Change |
| --- | --- |
| `vm.c` | The page counters, `cow()`, `pagefault()`, and counter-aware freeing in `deallocuvm()` |
| `trap.c` | Calls `pagefault()` on a page fault |
| `mmu.h` | The `PTE_S` shared flag |
| `proc.c` | `fork` uses `cow()`, and `procdump()` prints page mappings |
| `syscall.c`, `syscall.h`, `sysproc.c`, `user.h`, `usys.S` | The `procdump` system call |
| `defs.h`, `Makefile` | New declarations, and the `testcow` program |

[`DESIGNDOC.txt`](DESIGNDOC.txt) summarizes these changes, and [`CSCI350Project4.pdf`](CSCI350Project4.pdf) is the assignment.

## Building and running

This project builds with the course's Docker environment, [cs350-docker](https://github.com/camerondurham/cs350-docker):

```bash
./run.sh start
./run.sh shell
```

Then, inside the xv6 source folder (`xv6-public-master/`):

```bash
make clean
make qemu-nox
```

At the xv6 shell prompt, run `testcow`.
