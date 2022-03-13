# Building with CMake

## Dependencies

This project relies on [`cpplocate`](https://github.com/cginternals/cpplocate) to do the heavy lifting for locating the executable/dynamic library at runtime.

This project relies on [`catch2`](https://github.com/catchorg/Catch2) as a testing framework.

All dependencies can be provided by [`vcpkg`](https://github.com/microsoft/vcpkg) which is a submodule for this repo.

Unfortunately, (at the time of writing) `cpplocate` does not have a package in vcpkg upstream. So we provide our own _overlay port_ to do the packaging ourselves. This involves setting some extra CMake options, so we will be taking advantage of [CMake Presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html) to fill these in for us in the following directions. If you do not have the latest CMake version that supports the presets, then it is fairly easy to look at the [CMakePresets.json](CMakePresets.json) file and set the appropriate options yourself.

## Build

Here are the steps for building in release mode with a single-configuration generator, like the Unix Makefiles one:

```sh
cmake --preset=vcpkg-ci -D CMAKE_BUILD_TYPE=Release
cmake --build build
```

Here are the steps for building in release mode with a multi-configuration generator, like the Visual Studio ones:

```sh
cmake --preset=vcpkg-ci
cmake --build build --config Release
```

## Install

This project doesn't require any special command-line flags to install to keep
things simple. As a prerequisite, the project has to be built with the above
commands already.

The below commands require at least CMake 3.15 to run, because that is the
version in which [Install a Project][1] was added.

Here is the command for installing the release mode artifacts with a
single-configuration generator, like the Unix Makefiles one:

```sh
cmake --install build
```

Here is the command for installing the release mode artifacts with a
multi-configuration generator, like the Visual Studio ones:

```sh
cmake --install build --config Release
```

[1]: https://cmake.org/cmake/help/latest/manual/cmake.1.html#install-a-project
