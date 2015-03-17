# ypsilon
Automatically exported from code.google.com/p/ypsilon

Ypsilon is the implementation of Scheme Programming Language, which conforms to the latest standard R6RS. It achieves a remarkably short GC pause time and the best performance in parallel execution as it implements "mostly concurrent garbage collection", which is optimized for the multi-core CPU system.

Ypsilon is easy to use as well as good for applications of any kind that require quick, reliable, and interactive data processing. It implements full features of R6RS and R6RS standard libraries including:

* arbitrary precision integer arithmetic
* rational number
* exact and inexact complex number
* implicitly phased library
* top-level program
* proper tail recursion
* call/cc and dynamic wind
* unicode
* bytevectors
* records
* exceptions and conditions
* i/o
* syntax-case
* hashtables
* enumerations
