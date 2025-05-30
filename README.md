# Philosophers

## Project Overview

**Philosophers** is a concurrency-focused project from the 42 curriculum, designed to simulate the classic **Dining Philosophers Problem**. The goal was to implement a multi-threaded system in C, where several philosophers alternated between eating, sleeping, and thinking — all while sharing limited resources (forks) without causing deadlocks or starvation.

The project challenged me to manage:
- **Thread creation and synchronization** using POSIX threads
- **Fork access control** using mutexes to prevent race conditions
- **Accurate timing** using `gettimeofday` and `usleep`
- **Precise logging** of philosopher actions with millisecond accuracy
- **Graceful termination** when a philosopher died
- Optional **limit on the number of meals** each philosopher must eat, after which the simulation ends cleanly

To comply with the rules of the project:
- Each philosopher acted independently in its own thread
- Forks were protected by mutexes to ensure safe access
- The simulation prioritized both correctness and timing precision (death messages had to be logged within 10ms)

---

## Description

### Simulation Rules:
- Philosophers sit around a table with **one fork between each pair**.
- A philosopher needs **two forks to eat**: one on the left and one on the right.
- After eating, a philosopher **sleeps**, then **thinks**, then repeats the cycle.
- If a philosopher doesnt start eating within a given time (`time_to_die`), he **dies**, ending the simulation.
- Forks are **protected by mutexes** to prevent simultaneous access.

### Behavior Requirements:
Each philosopher operates in its own thread and followed these strict rules:
- Log messages are printed in the format:
```bash
timestamp_in_ms X has taken a fork
timestamp_in_ms X is eating
timestamp_in_ms X is sleeping
timestamp_in_ms X is thinking
timestamp_in_ms X died
```
- Messages **must never overlap**, ensuring clear and atomic state changes.
- The simulation stopped either when:
  - A philosopher **died**
  - All philosophers ate the required number of times (if specified)

### Program Usage:
```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```
- number_of_philosophers: How many philosophers (and forks) sit at the table.
- time_to_die: Time (in ms) a philosopher can go without eating before dying.
- time_to_eat: How long a philosopher spends eating (must hold two forks).
- time_to_sleep: Duration of sleep after eating.
- number_of_times_each_philosopher_must_eat (optional): If set, the simulation ends once all philosophers have eaten at least this many times.
