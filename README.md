<div align="center">

```
                                          Philosopher

```

> *"The philosophers keep thinking, the forks keep waiting — and the only true enemy is time."*

![42 School](https://img.shields.io/badge/42-School-black?style=for-the-badge&logo=42&logoColor=white)
![Language](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Threads](https://img.shields.io/badge/pthreads-Concurrency-blueviolet?style=for-the-badge)
![Mutexes](https://img.shields.io/badge/Mutex-Synchronization-orange?style=for-the-badge)
![License](https://img.shields.io/badge/license-MIT-green?style=for-the-badge)

</div>

---

## 📖 Table of Contents

1. [The Problem](#-the-dining-philosophers-problem)
2. [What is a Thread?](#-what-is-a-thread)
3. [Thread vs Process](#-thread-vs-process)
4. [Thread Control Block (TCB)](#-thread-control-block-tcb)
5. [Thread Lifecycle & Creation](#-thread-lifecycle--creation)
6. [Concurrency vs Parallelism](#-concurrency-vs-parallelism)
7. [Race Conditions](#-race-conditions)
8. [Mutex — The Gatekeeper](#-mutex--the-gatekeeper)
9. [Semaphore — The Counter Guard](#-semaphore--the-counter-guard)
10. [POSIX & pthreads](#-posix--pthreads)
11. [pthread_join — Waiting Gracefully](#-pthread_join--waiting-gracefully)
12. [Project Structure](#-project-structure)
13. [Usage & Compilation](#-usage--compilation)
14. [Mandatory vs Bonus](#-mandatory-vs-bonus)

---

## 🍽️ The Dining Philosophers Problem

> A classic concurrency problem posed by **Edsger W. Dijkstra** in 1965.

```
        [Philosopher 1]
           🤔  🍴
      🍴              🍴
  [Philo 5]  🍝🍝🍝  [Philo 2]
      🍴              🍴
           🍴  🤔
        [Philosopher 4]
              🤔
          [Philosopher 3]
```

**Scenario:**
- `N` philosophers sit around a circular table with a shared bowl of spaghetti.
- Each philosopher alternates between **thinking**, **eating**, and **sleeping**.
- Between each pair of philosophers lies a **single fork**.
- To eat, a philosopher needs **two forks** (left and right).
- A philosopher **dies** if they go too long without eating.

**The challenge:** Coordinate fork access to prevent:
- 💀 **Starvation** — a philosopher never gets to eat.
- 🔒 **Deadlock** — all philosophers hold one fork and wait forever.
- 🏁 **Race conditions** — unpredictable behavior from unsynchronized access.

---

## 🧵 What is a Thread?

A **thread** is the smallest unit of execution within a program.

```
┌─────────────────────────────────────────────────────────┐
│                        PROCESS                          │
│                                                         │
│  ┌──────────────────────────────────────────────────┐  │
│  │            SHARED MEMORY SPACE                   │  │
│  │  • Code Segment (text)                           │  │
│  │  • Global Variables                              │  │
│  │  • Heap Memory                                   │  │
│  │  • File Descriptors                              │  │
│  └──────────────────────────────────────────────────┘  │
│                                                         │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐    │
│  │  Thread 1   │  │  Thread 2   │  │  Thread 3   │    │
│  │  (main)     │  │  (philo 1)  │  │  (philo 2)  │    │
│  │  own stack  │  │  own stack  │  │  own stack  │    │
│  │  own PC     │  │  own PC     │  │  own PC     │    │
│  │  own regs   │  │  own regs   │  │  own regs   │    │
│  └─────────────┘  └─────────────┘  └─────────────┘    │
└─────────────────────────────────────────────────────────┘
```

### Key Properties

| Property           | Thread                          | Process                        |
|--------------------|----------------------------------|-------------------------------|
| Memory Space       | Shared with parent process       | Isolated, private              |
| Stack              | Private per thread               | Private per process            |
| Program Counter    | Private (own instruction pointer)| Private                        |
| Creation Cost      | Low (fast)                       | High (slow)                    |
| Communication      | Direct (shared memory)           | IPC (pipes, sockets, etc.)     |
| Context Switch     | Fast                             | Slow                           |

> 🔑 A single thread **cannot** be executed by multiple cores simultaneously.  
> 🔑 Each thread **can** be executed by a single CPU core.

---

## 💡 Thread vs Process

### Why Threads Switch Faster Than Processes

Context switching between threads is significantly faster than between processes due to three key reasons:

```
PROCESS context switch:
┌────────────────────────────────────────────────┐
│  Save ALL registers + memory maps + page tables│
│  + file descriptors + signal handlers ...      │
│  ─────────── HEAVY OVERHEAD ───────────────   │
└────────────────────────────────────────────────┘

THREAD context switch:
┌────────────────────────────────────────────────┐
│  Save: stack pointer + program counter         │
│               + registers                      │
│  ─────────── LIGHT OVERHEAD ───────────────   │
└────────────────────────────────────────────────┘
```

1. **Shared Memory Space** — No need to swap page tables or memory maps.
2. **Lower Overhead** — Thread state (TCB) is a fraction of process state (PCB).
3. **Reduced State Info** — Only stack, PC, and registers need saving/restoring.

---

## 🗂️ Thread Control Block (TCB)

Every thread is represented in the OS by a **Thread Control Block (TCB)** — a data structure storing all thread-specific state.

```
┌─────────────────────────────────────────────────┐
│               THREAD CONTROL BLOCK              │
├─────────────────────────────────────────────────┤
│  1. Thread ID (TID)         → Unique identifier │
│  2. Program Counter (PC)    → Next instruction  │
│  3. CPU Registers           → Current reg state │
│  4. Stack Pointer           → Top of the stack  │
│  5. State                   → Running/Ready/... │
│  6. Priority                → Scheduling weight │
│  7. Thread-Local Storage    → Private variables │
│  8. Resource References     → Memory, file hdls │
│  9. Parent PCB pointer      → Owning process    │
│ 10. Scheduling Info         → CPU time, affinity│
└─────────────────────────────────────────────────┘
```

### Thread States

```
              pthread_create()
                    │
                    ▼
              ┌──────────┐
              │  READY   │◄──────────────────────┐
              └────┬─────┘                       │
                   │ scheduler picks thread      │ time-slice expires /
                   ▼                             │ preempted
              ┌──────────┐                       │
              │ RUNNING  │───────────────────────┘
              └────┬─────┘
                   │ waiting for resource / I/O
                   ▼
              ┌──────────┐
              │ BLOCKED  │
              └────┬─────┘
                   │ resource available
                   └──────────► READY (again)
                   │
              ┌────▼─────┐
              │TERMINATED│  ← pthread_exit() / return from thread fn
              └──────────┘
```

---

## 🔧 Thread Lifecycle & Creation

### How a Thread is Born

```
Program starts
     │
     ▼
OS creates Process Control Block (PCB)
     │
     └─── Initial thread = main thread
                │
                ▼
        pthread_create() called
                │
                ▼
   ┌────────────────────────────┐
   │  OS allocates new TCB      │
   │  OS initializes TCB with:  │
   │   • entry function address │
   │   • function arguments     │
   │   • initial priority       │
   │   • stack allocation       │
   └─────────────┬──────────────┘
                 │
                 ▼
         Thread state = READY
                 │
                 ▼
   Scheduler picks thread → CPU loads TCB
                 │
                 ▼
         Thread RUNNING 🚀
```

### In Code (pthreads)

```c
// Thread entry function
void *routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    // philosopher logic here
    return (NULL);
}

// Creating a thread
pthread_create(&philo->thread,   // thread identifier
               NULL,             // default attributes
               routine,          // entry point function
               &philo);          // argument passed to function
```

### When Creating N Philosophers

```c
void create_threads(t_main *main, int philo_nbr)
{
    int i = 0;
    main->start_time = get_time_in_ms();
    while (i < philo_nbr)
    {
        main->philos[i].main = main;
        main->philos[i].last_meal = main->start_time;
        pthread_create(&main->philos[i].thread, NULL,
                       routine, &main->philos[i]);
        i++;
    }
}
```

---

## ⚡ Concurrency vs Parallelism

```
SEQUENTIAL:
─────────────────────────────────────────────►  time
  [Task A ████████][Task B ████████][Task C ████████]


CONCURRENT (1 core):
─────────────────────────────────────────────►  time
  [A██][B██][A██][C██][B██][A██][C██][B██]
   ↑ rapid context switches create the ILLUSION of simultaneity


PARALLEL (multiple cores):
  Core 1: [Task A ████████████████████████]
  Core 2: [Task B ████████████████████████]
  Core 3: [Task C ████████████████████████]
          ─────────────────────────────►  time
```

### The Illusion of Concurrency

On a **single-core CPU**, only **one thread truly executes at a time**. Context switching happens so fast (in **nanoseconds**) that it appears as if all threads run simultaneously — but it's an illusion created because:

- Human perception of time is too slow to notice the rapid switches.
- The OS context-switches between threads thousands of times per second.

> 🔑 A core is considered by the computer as **its own CPU**.

| Model         | Description                                              | Example                             |
|---------------|----------------------------------------------------------|-------------------------------------|
| Sequential    | Tasks run one after another                              | Reading a file then writing it      |
| Concurrent    | Tasks progress independently, may interleave             | Web server handling requests        |
| Parallel      | Tasks execute truly simultaneously on multiple cores     | Splitting a big computation         |

---

## ⚠️ Race Conditions

A **race condition** arises when two or more threads access and modify a **shared resource** at the same time, and the outcome depends on the **unpredictable timing** of these accesses.

```
Thread 1 reads:   counter = 5
Thread 2 reads:   counter = 5   ← reads BEFORE Thread 1 writes back!
Thread 1 writes:  counter = 6
Thread 2 writes:  counter = 6   ← WRONG! Should be 7
```

### In the Philosophers Problem

```
Fork = shared resource
│
├── Philosopher 1 grabs fork (reads state: available)
│
├── Context switch happens here  ← danger zone!
│
└── Philosopher 2 ALSO grabs the same fork (also reads: available)
        ↑ Both think they have exclusive access — DATA RACE!
```

**Solution:** Use **synchronization primitives** — mutexes or semaphores.

---

## 🔐 Mutex — The Gatekeeper

A **mutex** (Mutually Exclusive flag) is the security guard of concurrent programming. It ensures that only **one thread at a time** can access a shared resource.

```
┌─────────────────────────────────────────────┐
│                  MUTEX                      │
│                                             │
│  🔒 LOCKED   → only one thread inside      │
│  🔓 UNLOCKED → any thread may enter        │
│                                             │
│  Components:                                │
│  • Lock State     (locked / unlocked)       │
│  • Owner Thread   (which thread holds it)   │
│  • Wait Queue     (threads waiting to enter)│
│  • Condition Vars (signaling between threads)│
└─────────────────────────────────────────────┘
```

### Mutex Flow

```
Thread 1                       Thread 2
────────                       ────────
pthread_mutex_lock(&fork)      pthread_mutex_lock(&fork)
   ↓ (acquires lock 🔒)            ↓ (BLOCKS — mutex is locked)
   [critical section]              [waiting...]
   [use shared resource]           [waiting...]
pthread_mutex_unlock(&fork)        ↓ (wakes up, acquires lock 🔒)
                               [critical section]
                               pthread_mutex_unlock(&fork)
```

### In Code

```c
// Protect a shared message print
pthread_mutex_lock(&philo->main->message_lock);
printf("%d  %d is eating\n", time, philo->id);
pthread_mutex_unlock(&philo->main->message_lock);
```

---

## 🚦 Semaphore — The Counter Guard

A **semaphore** is a signaling mechanism represented by an **integer value** shared between threads. Its purpose is to protect a **critical section** from being entered by more threads than allowed.

```
Semaphore value = N  →  up to N threads may enter simultaneously

sem_wait(sem)  ── decrements value; blocks if value == 0
sem_post(sem)  ── increments value; wakes a waiting thread
```

```
Semaphore (value = 2):

Thread 1 → sem_wait() → value = 1 → enters
Thread 2 → sem_wait() → value = 0 → enters
Thread 3 → sem_wait() → value = -1 → BLOCKED (waits)
Thread 1 → sem_post() → value = 0 → Thread 3 unblocked → enters
```

| Feature       | Mutex                       | Semaphore                        |
|---------------|-----------------------------|----------------------------------|
| Value         | Binary (locked/unlocked)    | Integer (0 to N)                 |
| Ownership     | Owned by locking thread     | No ownership concept             |
| Use Case      | Mutual exclusion            | Resource counting / signaling    |
| Signals       | No                          | Yes (can signal other threads)   |

> Used in `philo_bonus` where **processes** replace threads, requiring semaphores instead of mutexes.

---

## 🌐 POSIX & pthreads

**POSIX** (Portable Operating System Interface) is a set of standards defining how software should interact with the OS, ensuring portability across Unix-like systems (Linux, macOS, BSD…).

```
POSIX defines:
  • Standard CLI tools:     cd, ls, grep, cat…
  • Standard system calls:  open(), read(), write(), close()…
  • Standard thread API:    pthread_create(), pthread_join(), pthread_mutex_*…
```

### Why POSIX Matters

| Goal                | Description                                                         |
|---------------------|---------------------------------------------------------------------|
| **Portability**     | Write once, run on any POSIX-compliant OS without modification      |
| **Interoperability**| Different systems communicate using standardized interfaces         |
| **Consistency**     | Predictable behavior reduces bugs and development time              |
| **Ecosystem**       | Common interface encourages broader software development            |

### POSIX Thread (pthread) API Used in This Project

```c
pthread_create()     // Create a new thread
pthread_join()       // Wait for a thread to finish
pthread_mutex_init() // Initialize a mutex
pthread_mutex_lock() // Lock a mutex (blocking)
pthread_mutex_unlock()// Release a mutex
pthread_mutex_destroy()// Clean up a mutex
```

---

## ⏳ pthread_join — Waiting Gracefully

`pthread_join()` makes the calling thread **wait** until the specified thread terminates.

```
Without pthread_join():                With pthread_join():
─────────────────────────────         ─────────────────────────────
main()                                main()
  pthread_create(philo1)                pthread_create(philo1)
  pthread_create(philo2)                pthread_create(philo2)
  main exits immediately ❌             pthread_join(philo1) ──┐
  philos become zombies ☠️              pthread_join(philo2) ──┤
                                        main exits cleanly ✅  │
                                                               │
                                   waits until all threads end ┘
```

### Consequences of NOT Joining

1. **Thread Creation still works** — `pthread_create()` creates threads regardless.
2. **Main exits early** — Without joining, main may terminate before philosophers finish.
3. **Zombie Threads** — Threads still running after main exits → resource leaks, undefined behavior.

### In This Project

```c
// After creating all philosopher threads:
void *join_threads(t_philo *philos, int philo_nbr)
{
    int i = 0;
    while (i < philo_nbr)
    {
        pthread_join(philos[i].thread, NULL);
        i++;
    }
    return (NULL);
}
```

---

## 📁 Project Structure

```
Philosopher/
├── philo/                      ← Mandatory (threads + mutexes)
│   ├── main.c                  → Entry point, thread creation, struct init
│   ├── philo.c                 → Philosopher routine (eat/sleep/think)
│   ├── simulation.c            → Monitor thread, death detection
│   ├── philo_utils.c           → Time utilities, sleep, reader/writer
│   ├── parse.c                 → Argument validation
│   ├── ft_atoi.c               → String to integer conversion
│   └── philo.h                 → Structs, defines, prototypes
│
└── philo_bonus/                ← Bonus (processes + semaphores)
    ├── main.c                  → Entry point, process creation
    ├── simulation.c            → Philosopher process logic
    ├── bonus_utils.c           → Utility functions
    ├── clean.c                 → Resource cleanup
    └── philo_bonus.h           → Structs, defines, prototypes
```

### Core Data Structures

```c
typedef struct main_struct
{
    pthread_mutex_t  message_lock;   // Protects console output
    pthread_mutex_t  dead_flag;      // Protects death flag
    pthread_mutex_t  meal_flag;      // Protects meal count
    pthread_mutex_t  *forks;         // One mutex per fork
    pthread_t        monitor;        // Monitor thread
    t_philo          *philos;        // Array of philosophers
    int              start_time;     // Simulation start (ms)
    int              time_eat;       // ms to eat
    int              time_die;       // ms before dying of hunger
    int              time_sleep;     // ms to sleep
    int              philo_nbr;      // Number of philosophers
    int              meals;          // Optional meal limit
    int              dead_sign;      // 0 if someone died
}   t_main;

typedef struct philo
{
    pthread_t        thread;         // This philosopher's thread
    pthread_mutex_t  *right_fork;    // Fork on the right
    pthread_mutex_t  *left_fork;     // Fork on the left
    t_main           *main;          // Pointer to shared state
    int              id;             // Philosopher number (1-based)
    int              last_meal;      // Timestamp of last meal
    int              meals_nbr;      // How many meals eaten
}   t_philo;
```

---

## 🚀 Usage & Compilation

### Building

```bash
# Mandatory version (threads + mutexes)
cd philo
make

# Bonus version (processes + semaphores)
cd philo_bonus
make
```

### Running

```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

| Argument                                  | Description                                         |
|-------------------------------------------|-----------------------------------------------------|
| `number_of_philosophers`                  | Also the number of forks on the table               |
| `time_to_die` (ms)                        | A philosopher dies if they haven't eaten in this time |
| `time_to_eat` (ms)                        | Time it takes to eat (holding two forks)            |
| `time_to_sleep` (ms)                      | Time spent sleeping after eating                    |
| `number_of_times_each_philosopher_must_eat` | Optional: simulation stops when all reach this count |

### Examples

```bash
# 5 philosophers, die at 800ms, eat in 200ms, sleep 200ms
./philo 5 800 200 200

# Same but stop after each philosopher eats 7 times
./philo 5 800 200 200 7

# Edge case: 1 philosopher (should die — only 1 fork)
./philo 1 800 200 200
```

### Expected Output Format

```
timestamp_in_ms  philosopher_id  action

Example:
0      1 has taken a fork
0      1 has taken a fork
10     1 is eating
210    1 is sleeping
410    1 is thinking
800    2 died
```

---

## 🆚 Mandatory vs Bonus

|                        | **Mandatory** (`philo/`)       | **Bonus** (`philo_bonus/`)       |
|------------------------|--------------------------------|-----------------------------------|
| Concurrency Model      | Threads (`pthread`)            | Processes (`fork`)                |
| Synchronization        | Mutexes (`pthread_mutex_t`)    | Semaphores (`sem_t`)              |
| Memory Sharing         | Shared (same process)          | Separate per process              |
| IPC Mechanism          | Shared variables (mutex-guarded)| Semaphores (POSIX named sems)    |
| Death Detection        | Monitor thread                 | Monitor process / signal          |

---

<div align="center">

### 🧠 Key Takeaways

```
┌──────────────────────────────────────────────────────────────────┐
│                                                                  │
│  Threads share memory → fast communication, but dangerous.       │
│  Mutexes prevent race conditions → only one thread at a time.    │
│  Semaphores control access to N resources simultaneously.        │
│  Context switching is fast (ns) → concurrency feels like magic.  │
│  pthread_join ensures clean shutdown — never leave zombies.      │
│                                                                  │
└──────────────────────────────────────────────────────────────────┘
```

*Made with 🧠 by [Samia-Hb](https://github.com/Samia-Hb) — 1337 school*

</div>
