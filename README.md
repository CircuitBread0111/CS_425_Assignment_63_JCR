# ✖️ `pth_multiplication.c` – Multithreaded Matrix Multiplication in C

This program that demonstrates how to parallelize matrix multiplication using **POSIX threads (pthreads)** in C. It compares the performance of a parallel implementation against a traditional serial approach and validates that both methods produce the same result.

---

## 📄 File Information

- **File Name:** `pth_multiplication.c`  
- **Language:** C  
- **Author:** Jerrin C. Redmon  
- **Version:** 1.0.0  
- **Date:** April 14, 2025  

---

## 🧠 Description

This program:

- Multiplies two randomly generated 1000x1000 matrices using both:
  - A **serial** nested-loop implementation
  - A **parallel** implementation using `pthread_create`
- Distributes rows across threads for load-balanced matrix multiplication
- Verifies that the serial and parallel results match
- Measures and reports execution times and performance speedup

---

## 🚀 How to Compile and Run

### 🔧 Compile

```bash
gcc -o pth_multiplication pth_multiplication.c -lpthread
```

### ▶️ Run

```bash
./pth_multiplication <num_threads>
```

Example:

```bash
./pth_multiplication 4
```

---

## 📊 Performance Results (4 Threads)

```
Serial Time: 0.5386 seconds
Parallel Time: 0.1908 seconds
Speedup: 2.82x
Result: Match
```

---

## 🧱 Code Structure

- `ThreadData struct`: Contains matrix pointers and row indices for each thread
- `multiply()`: Thread worker function for computing partial matrix products
- `main()`:
  - Initializes random matrices and result buffers
  - Computes product serially and times it
  - Launches threads for row-wise parallel multiplication
  - Joins threads and aggregates results
  - Validates result and computes speedup

---

## 📦 Dependencies

- `pthread.h` — POSIX threading  
- `stdlib.h`, `stdio.h`, `time.h` — Standard C headers  

Ensure to compile with `-lpthread`.

---

## 🪪 License

Released under the MIT License.  
Free for academic, professional, and educational use.

---

## 👨‍💻 Author

**Jerrin C. Redmon**  
For inquiries or collaboration ideas, feel free to connect.

