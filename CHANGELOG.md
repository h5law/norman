# CHANGELOG

The changlog kept for changes made to the norman library over time

## [0.2.0] - 2025-04-14

### Changed
- Transitioned from independent single header-file libraries to a unified
  ond allowing for:
    - Inderdependet pieces of the whole
    - Removing code duplicartion by the use of imports
-   - The use of the dynamic allocator in other components (and later the
      implementations of {ma,re,ca}loc for a fully self-container library
- Header files with guards moved to `./include/*.h`
- Implementations moved into `./src/*c` files separating the single files

### Added
- Relevant `Makefile` targets updated
- Added targets to build library artifcacts, install them any copy them
  to their specific target destinations for usage on the system

<!-- vim: ft=markdown ts=2 sw=2 sts=2 ai si et  -->
