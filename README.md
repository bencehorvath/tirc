# TIRC
## Twl's C++ IRC library

### Requirements

* Boost (>= 1.50.0) - http://www.boost.org
* C++11-compatible compiler and standard library.
    * GCC >= 4.7 with libstdc++ will be fine.
    * Clang >= 3.2 with libc++ will also work.
* CMake (http://www.cmake.org)


### How to build

It is extremely simple. You have to do an out-of-source build though.

* You clone the repository:

```bash
git clone https://github.com/Twl/tirc.git tirc
```

* You create a build directory:

```bash
mkdir tirc-build
cd tirc-build
```


* Finally, you build the source using CMake and make.

```bash
cmake ../tirc -DCMAKE_INSTALL_PREFIX=<desired_install_location> -DWITH_TESTER=0
make
make install
```

Done!

The library will be found in the prefix/lib directory, with include files in prefix/include (and optionally a not supported
lib tester in prefix/bin).
