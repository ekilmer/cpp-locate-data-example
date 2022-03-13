# locate-example

This is the locate-example project. This example shows how to create a CMake project that can load data files at runtime in a way that supports relocation of the installed artifacts (like in vcpkg).

This project attempts to follow best practices for CMake first and C++ second. If you find anything that you think should be changed, please open an issue or (even better) a pull request, and we can discuss a resolution.

# Building and installing

See the [BUILDING](BUILDING.md) document.

# Contributing

See the [CONTRIBUTING](CONTRIBUTING.md) document.

# Licensing

This project was initialized with the great [`cmake-init`](https://github.com/friendlyanon/cmake-init) project (see commit messages for version info) for a base CMake setup, where the generated files are in the Public Domain.

This project makes use of the [`cpplocate`](https://github.com/cginternals/cpplocate) library, which is under the MIT License, to do the heavy lifting for cross-platform binary self-location.

In the hope that this project's CMake files serve as a best-practice example for a CMake project that includes runtime data, the modified CMake content of this repo is also Public Domain. Everyone is free to use, modify, republish, sell or give away these files without prior consent from anybody. These files are provided on an "as is" basis, without warranty of any kind. Use at your own risk! Under no circumstances shall the author(s) or contributor(s) be liable for damages resulting directly or indirectly from the use or non-use of these files. 
