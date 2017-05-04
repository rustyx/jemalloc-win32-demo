# jemalloc Windows demo

This is a demo project for redirecting C++ `new` and `delete` operators to jemalloc, bypassing the Windows heap.

It demonstrates that:
1. jemalloc on Windows 64-bit is faster than the Windows heap.
2. jemalloc on Windows 32-bit is NOT faster and runs out of memory, so is probably a bad idea to use on 32-bit.

Build prerequisites:
* Cygwin with autoconf and autogen (cygwin/bin folder must be on the PATH)
* Git (must be on the PATH)

Run build-jemalloc.cmd to build the jemalloc (static) library.

Once built, compare the performance of `Release | x64` with `Release-jemalloc | x64` (i.e. with and without jemalloc).
