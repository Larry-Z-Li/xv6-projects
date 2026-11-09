# Project 3: Multilevel Feedback Queue Scheduler for xv6

This project replaces the round-robin scheduler in [xv6](https://github.com/mit-pdos/xv6-public), a small Unix-like teaching operating system written in C for 32-bit x86, with a three-level multilevel feedback queue (MLFQ) scheduler. It also adds a system call that prints each process's scheduling history. It was built for USC's CSCI 350 (Operating Systems).

## How the scheduler works

- **Three priority queues.** `q0` has the highest priority, then `q1`, then `q2`. Every new process starts in `q0`.
- **Picking a process.** Each time the scheduler runs, it chooses the first runnable process in `q0`. If `q0` has none, it looks in `q1`, and then in `q2`.
- **Time slices.** A process may run for 1 timer tick at a time in `q0`, 2 ticks in `q1` and 8 ticks in `q2`. When the timer interrupt finds that the running process has used up its slice, it makes the process give up the CPU.
- **Demotion.** Once a process has used 1 tick in total in `q0`, it moves down to `q1`. Once it has used 2 ticks in total in `q1`, it moves down to `q2`. In `q2`, a process that has used 8 ticks moves to the back of the queue, so `q2` works as round robin.
- **Boosting.** Each time a process finishes a turn on the CPU, the length of that turn is added to the wait time of every process in `q2`. A process that has waited more than 50 ticks is moved back up to `q0`, so processes in the lowest queue don't starve.

## Scheduling statistics: `getpinfo(pid)`

Each time a process is scheduled, the scheduler records the tick it started, how many ticks it ran and which queue it was in. Each process can hold up to 1,500 of these records (`NSCHEDSTATS` in `pstat.h`). `getpinfo(pid)` prints that process's records, along with its wait time, how many times it was scheduled in each queue, and how long its most recent turn in each queue lasted.

## Tests

`test1`, `test2` and `test3` start processes with different workloads, such as printing in a loop or sleeping repeatedly, and call `getpinfo` at the end to print their scheduling history. [`output.pdf`](output.pdf), [`output2.pdf`](output2.pdf) and [`output3.pdf`](output3.pdf) graph the results, and [`workload.txt`](workload.txt) explains them.

## Files changed

| File | Change |
| --- | --- |
| `proc.c` | The new `scheduler()`, the `boost()` function, placing new processes in `q0` (`allocproc()`, `userinit()`), and `getpinfo()` |
| `proc.h` | The three queues and the per-process fields for ticks, wait time and statistics |
| `pstat.h` | The `sched_stat_t` record type |
| `trap.c` | Makes the running process give up the CPU when its time slice is used up |
| `syscall.c`, `syscall.h`, `sysproc.c`, `user.h`, `usys.S` | The `getpinfo` system call |
| `defs.h` | Declarations for `boost()` and `getpinfo()` |
| `Makefile`, `test1.c`, `test2.c`, `test3.c` | The test programs |

[`DESIGNDOC.txt`](DESIGNDOC.txt) summarizes these changes.

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

At the xv6 shell prompt, run `test1`, `test2` or `test3`.
