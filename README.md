# 🧵 Reader-Writer Synchronization (C++ pthreads)
![License](https://img.shields.io/badge/license-MIT-blue.svg)

A multithreaded C++ console application that simulates the classic **Reader-Writer Problem** using **POSIX threads**, **mutexes**, and **semaphores**. This project was built on **Ubuntu 24.04** to explore thread synchronization techniques in a multi-threaded system.

---

## 💡 Project Overview

This simulation was originally developed as part of an academic group project for the **Operating System Lab (KT24403)** course at Universiti Malaysia Sabah (UMS). It demonstrates synchronization control where multiple reader and writer threads access a shared file concurrently.

---

## 🚀 Features

- 📖 Multiple **reader threads** can read simultaneously
- ✍️ **Writers** gain exclusive access to write
- 🔒 Synchronization using `pthread_mutex_t` and `sem_t`
- 🧮 Custom input: number of readers/writers and cycles
- 📂 Uses actual file I/O with `io_shared_file.txt`
- 🧾 Console log output for each thread’s activity

---

## 🛠️ Built With

- **C++** (G++ Compiler)
- **POSIX Threads (pthreads)**
- **Ubuntu 24.04**
- Terminal-based interaction and file I/O

---

## 📁 Project Structure
```
ReaderWriterSync/
├── main.cpp # Full thread and logic implementation
├── io_shared_file.txt # Shared file for read/write operations
├── README.md # This file
└── LICENSE # MIT License
```

---

## ▶️ How to Run (Ubuntu)

1. Clone this repository:
   ```
   bash
   git clone https://github.com/anake-an/ReaderWriterSync.git
   cd ReaderWriterSync
   ```
2. Compile using g++:
   ```
   g++ -o sync main.cpp -l pthread
   ```
3. Run the program:
   ```
   ./sync
   ```
4. Provide required input (number of threads and cycles) when prompted.

---

## 📊 Example Test Scenarios

These test cases were executed on Ubuntu 24.04 to validate synchronization correctness.

| Test Case | Action                               | Expected Result                                           | Outcome |
|-----------|--------------------------------------|-----------------------------------------------------------|---------|
| TC1       | 2 readers start together             | Both readers can read the file simultaneously             | ✅ PASS |
| TC2       | 1 writer starts while readers active | Writer waits until all readers finish before writing      | ✅ PASS |
| TC3       | 1 reader starts during writer        | Reader waits until writer finishes                        | ✅ PASS |
| TC4       | 2 writers active                     | Only one writer writes at a time, the other waits         | ✅ PASS |
| TC5       | Full simulation with many threads    | No deadlocks, race conditions, or crashes                 | ✅ PASS |
| TC6       | Writer writes → reader reads         | Reader can read new content written by writer             | ✅ PASS |

---

## 👥 Contributors

This project was originally developed as a group assignment for the KT24403 Operating System Lab course.

| Name                                   | Profile Link                                                            |
|----------------------------------------|-------------------------------------------------------------------------|
| **Aniq Najmuddin Bin Sharifuddin**     | [Linkedin](https://www.linkedin.com/in/aniqnaj)                         |
| Muhammad Faris Bin Huzaimi             | [LinkedIn](https://www.linkedin.com/in/farishuz)                        |
| Muhammad Isyraf Ahnaf Bin M. Zamri     | [LinkedIn](https://www.linkedin.com/in/muhammad-isyraf-ahnaf-320557256) |
| Muhammad Saifullah Bin Rosman          | [LinkedIn](https://www.linkedin.com/in/muhammad-saifullah-5a3481272)    |



