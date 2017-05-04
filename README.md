# jemalloc Windows demo

This is a demo project for redirecting C++ `new` and `delete` operators to jemalloc, bypassing the Windows heap.

Build prerequisites:
* Cygwin with autoconf and autogen (cygwin/bin folder must be on the PATH)
* Git (must be on the PATH)

Run build-jemalloc.cmd to build the jemalloc (static) library.

Once built, compare the performance of `Release | x64` with `Release-jemalloc | x64` (i.e. with and without jemalloc).
