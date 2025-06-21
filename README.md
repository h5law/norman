# norman + rollo
#### WIP - VERY EARLY DEVELOPMENT
## A `libc` implementation powering a from scratch `kernel` for a from scratch `OS`

> The goal in this project is to write every single line of code myself,
> no external libraries, no system headers, or compiler builtins. A fully
> freestanding Operating System, Kernel, Bootloader and Libc.

`norman` is a multi-faceted project focused around building out from scratch a
bootloader, kernel, operating system and a `libc` implementation. In fact it is
the `libc` implementation that will power the rest of the components such as
the kernel who will in turn provide needed interfaces for it in return.

`rollo` is the name of the fledgling kernel and is rather new in development
with only serial IO through basic UART peripherals being supported currently.
While the `libc` implementation supports in theory `aarch64`, `arm` and
`x86_64` the kernel currently only supports `aarch64` architectures due to its
bootloader.

Being implemented fully from scratch is not easy and as such the work on the
`crt` or C Runtime/CSU will be postponed on the `libc` side until the kernel
is futher along read to load manage and execute binaries when the task will be
much clearer.

By targeting `aarch64` and `arm` predominantly, as apposed to `x86/x86_64`
there is unfortunately not such a wide abundance of resources available to pull
from but for this project I have found the  FreeBSD source tree to be welcoming
and very easy to parse, along with numerous entries from the ARM specifications.

**NOTE**: This `libc`, `bootloader` and `kernel` are far from pre-alpha and
should not be used outside of testing and playing around with the code. It is
a hobby project of mine to develop all things from scratch using only my code
so until I am further along in my vision I don't expect anyone to be attempting
to use this software.

https://github.com/user-attachments/assets/85466bff-a87e-4cc2-a2a9-ee30e333802e
