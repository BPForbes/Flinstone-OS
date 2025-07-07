# Flintstone Project

A low-level C-based simulation of a file system and shell environment.

## Overview

Demonstrates:
- In-memory directory tree (`cd`, `dir`, `pwd`)
- Disk image creation (`-mkdisk`)
- Disk geometry init (`-init`)
- Directory management (`-D`, `-R`)
- Cluster & hex utilities
- Lightweight thread pool
- Unit testing with CUnit

## Build

```bash
make
./BPForbes_Flinstone_Shell    # interactive
make BPForbes_Flinstone_Tests # run unit tests
```

## Usage Examples

```text
> -mkdisk vol1 8 16
> dir
> -D subdir
> dir
> cd subdir
> pwd
> cd ..
> -R subdir
> exit
```
