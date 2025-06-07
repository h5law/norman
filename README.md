# norman

`norman` is a pre-alpha implementation of a C standard library and eventually
fully compliant `libc` implementation. It is written from scratch with no
external dependencies in plain C99.

## Features

- ARM64 support
- Basic syscall wrapper implementations
- WIP vector, hashmap, allocator implementations
  - Being ported to use 0 dependencies by utilising the `libc` implementation
- Assembly entrypoint and syscall implementation for use in `C` code
  - Currently only targeting `arm64`/`aarch64` architectures

## TODO

- Replace usage of `newlib` `libc`'s [`crt0.o`](./external/)
- Implement basics of `malloc` family of functions
- Implement memory operations from `string.h`
  - `memset`, `mem(n)cpy`, `memmove`, ...
- Implement `fdopen` and other `fd_` `stdio.h` functions
- Port `mallogator.h` allocator to use these base methods
- Port `dynamic.h` implementations to use base methods and `mallogator.h`
- Add more demos and tests
- Improve build system
