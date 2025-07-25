# http
HTTP server

- [ ] level 1
- [ ] level 2
- [ ] level 3
- [ ] level 4
- [ ] level 5

Here are **five levels** of a C++ socket-based HTTP server project, from simplest to most sophisticated. Each level incrementally introduces more complexity and realism in terms of networking, threading, HTTP protocol handling, and architecture:

---

### **Level 1: Naive Single-Request Server (Educational Hello World)**

* **Design**:

  * Uses `socket`, `bind`, `listen`, `accept`, and `recv/send` once.
  * Handles **one client** at a time, blocking IO.
  * Parses HTTP manually with minimal robustness (e.g., assumes request is one line, always `GET`).

* **Features**:

  * Responds with a fixed HTML string (`HTTP/1.0 200 OK\r\n...`).
  * No concurrency, no proper HTTP header parsing.
  * Barely any error checking.

* **Use case**: Teaching socket API and basic HTTP structure.

---

### **Level 2: Basic Iterative Server with Minimal Parsing**

* **Design**:

  * Handles multiple clients **sequentially**, in a `while(true)` loop.
  * Can serve multiple `GET` requests with minimal HTTP header parsing.
  * All IO is **blocking**.

* **Features**:

  * Can serve files (e.g., from `./www`) by mapping paths to file reads.
  * Detects malformed requests, returns `404` or `400`.
  * Uses `Content-Length`, supports `Connection: close`.

* **Use case**: Hobby server, static file server prototype.

---

### **Level 3: Multithreaded Server (Thread-per-Connection)**

* **Design**:

  * Spawns a new `std::thread` or `pthread` for each accepted connection.
  * Thread handles request lifecycle: parse, dispatch, respond.
  * Shared logging, global state requires locking (`std::mutex`).

* **Features**:

  * Thread-safe logging, basic error recovery.
  * Improved HTTP compliance: `Host` header, MIME types, proper status codes.
  * Simple routing via if-else or `std::map<std::string, handler>`.

* **Use case**: Educational for threading models, testbed for middleware/routing logic.

---

### **Level 4: Event-Driven with `epoll`/`select` + Thread Pool**

* **Design**:

  * Uses **epoll** (Linux) or **select** (portable) for non-blocking IO multiplexing.
  * Employs a **fixed-size thread pool** for concurrency.
  * Separation of IO and logic layers; worker threads handle requests from a queue.

* **Features**:

  * Efficient under load compared to thread-per-connection.
  * Connection reuse (`keep-alive`), chunked transfer encoding.
  * Supports `GET`, `POST`, maybe HTTP/1.1 with pipelining.

* **Use case**: Lightweight production-grade server, embedded systems, real-time monitoring.

---

### **Level 5: Fully Modular HTTP Server Framework**

* **Design**:

  * Fully asynchronous architecture (`epoll`/`io_uring`).
  * Pluggable **router**, **middleware**, and **handlers**.
  * Abstracts transport (TCP, Unix domain sockets), supports TLS.

* **Features**:

  * Full HTTP/1.1 compliance, partial HTTP/2 support.
  * Optional features: WebSocket support, reverse proxy, static+dynamic routing, file caching.
  * Integrates with JSON libraries, logging frameworks, plugin architecture.
  * Build system with CMake, testing with `gtest`, metrics.

* **Use case**: Foundation for web frameworks or microservices in C++. Competitive with libraries like Boost.Beast or Crow.

---

### Summary Table

| Level | Architecture          | Concurrency    | Protocol Support      | Intended For                           |
| ----- | --------------------- | -------------- | --------------------- | -------------------------------------- |
| 1     | Single request        | None           | Hardcoded HTTP 1.0    | Learning sockets & HTTP basics         |
| 2     | Iterative loop        | Sequential     | Basic HTTP 1.0        | File-serving demos                     |
| 3     | Thread-per-connection | `std::thread`  | HTTP 1.0, some 1.1    | Exploring concurrency models           |
| 4     | `epoll` + thread pool | Pooled threads | HTTP 1.1, keep-alive  | Efficient real-world use               |
| 5     | Asynchronous, modular | Fully async    | HTTP 1.1+, WebSockets | Frameworks, high-performance use cases |

---

Let me know if you'd like code examples or a roadmap to move from one level to the next.
