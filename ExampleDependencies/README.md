# ExampleDependencies

Exemplary setup for automatically installing runtime dependencies.

A plugin developer can use `mv_install_dependencies` to set up automatic plugin runtime dependency gathering and installation with this command:
```cmake
mv_install_dependencies(${PROJECT_NAME} "hwy" "hwy_contrib")
```
Dependencies that are linked to our plugin `${PROJECT_NAME}` and found with `find_package` are automatically resolve (given that their respective cmake export files are nicely setup). This even works with dependency chains, as shown in this example: we link to Faiss, which in turn depends on Lapack and OpenBlas which in turn depends on other libraries.
Otherwise, we need to pass all targets that we build in our project but do not set up with `find_package` to `mv_install_dependencies`, which in this example is the plugin itself and two dependencies, "hwy" and "hwy_contrib". 

The `mv_install_dependencies` is automatically available when using `find_package(ManiVault ... CONFIG)`. The important bit here is **CONFIG**.

There might be cases in which not all dependencies are automatically resolved, i.e. some libraries are located in folders that `mv_install_dependencies` does not search in. We can provide additional search paths like this:
```cmake
set(DEPENDENCIES_FOLDERS "${FREEIMAGE_ROOT_DIR}/bin")
mv_install_dependencies(${PROJECT_NAME})
```
All paths listed in `DEPENDENCIES_FOLDERS` will now also be searched for runtime dependencies.

All installed dependencies are listed like this (current CI output on windows):
```bash
-- Resolved: D:/.conan/bab46b/1/ExampleDependencies/Release/blake3.dll
-- Resolved: D:/.conan/bab46b/1/ExampleDependencies/Release/faiss.dll
-- Resolved: D:/.conan/bab46b/1/_deps/highway-build/Release/hwy.dll
-- Resolved: D:/.conan/bab46b/1/_deps/highway-build/Release/hwy_contrib.dll
-- Resolved: D:/.conan/bab46b/1/ExampleDependencies/Release/libgcc_s_seh-1.dll
-- Resolved: D:/.conan/bab46b/1/ExampleDependencies/Release/libgfortran-5.dll
-- Resolved: D:/.conan/bab46b/1/ExampleDependencies/Release/liblapack.dll
-- Resolved: D:/.conan/bab46b/1/ExampleDependencies/Release/libquadmath-0.dll
-- Resolved: D:/.conan/bab46b/1/ExampleDependencies/Release/libwinpthread-1.dll
-- Resolved: D:/.conan/bab46b/1/ExampleDependencies/Release/openblas.dll
-- Installing: D:\.conan\bab46b\1\package\Release/PluginDependencies/ExampleDependenciesPlugin/blake3.dll
-- Installing: D:\.conan\bab46b\1\package\Release/PluginDependencies/ExampleDependenciesPlugin/faiss.dll
-- Installing: D:\.conan\bab46b\1\package\Release/PluginDependencies/ExampleDependenciesPlugin/hwy.dll
-- Installing: D:\.conan\bab46b\1\package\Release/PluginDependencies/ExampleDependenciesPlugin/hwy_contrib.dll
-- Installing: D:\.conan\bab46b\1\package\Release/PluginDependencies/ExampleDependenciesPlugin/libgcc_s_seh-1.dll
-- Installing: D:\.conan\bab46b\1\package\Release/PluginDependencies/ExampleDependenciesPlugin/libgfortran-5.dll
-- Installing: D:\.conan\bab46b\1\package\Release/PluginDependencies/ExampleDependenciesPlugin/liblapack.dll
-- Installing: D:\.conan\bab46b\1\package\Release/PluginDependencies/ExampleDependenciesPlugin/libquadmath-0.dll
-- Installing: D:\.conan\bab46b\1\package\Release/PluginDependencies/ExampleDependenciesPlugin/libwinpthread-1.dll
-- Installing: D:\.conan\bab46b\1\package\Release/PluginDependencies/ExampleDependenciesPlugin/openblas.dll
```

## Managing dependencies with vcpkg
[vcpkg](https://github.com/microsoft/vcpkg/) will be used in this project if `CMAKE_TOOLCHAIN_FILE` is set to `YOUR_LOCAL_PATH_TO/vcpkg/scripts/buildsystems/vcpkg.cmake`. It is used to install [faiss](https://github.com/facebookresearch/faiss) and [blake3](https://github.com/BLAKE3-team/BLAKE3). If not set, this repository will only build [highway](https://github.com/google/highway).

This plugin also illustrates how you can set up vcpkg on CI runs, see the main `conanfile.py`.

> Important: when building a project using `add_subdirectory` all vcpkg dependencies defined in the manifest file `vcpkg.json` must be uplifted to the main project. vcpkg is not recursively scanning for `vcpkg.json`. In CMake, vcpkg only uses the toplevel `vcpkg.json` since all dependencies need to be installed before the very first project() call.

## ManiVault's runtime dependency handling
ManiVault will load all dynamic libraries located in `PluginDependencies/plugin_a` before loading `plugin_a`. The folder structure has to look like this:
```
ManiVault Studio/
├─ ManiVault Studio.exe
├─ plugin_c_dependency.dll
├─ Plugins/
│  ├─ plugin_a.dll
│  ├─ plugin_b.dll
│  ├─ plugin_c.dll
├─ PluginDependencies/
│  ├─ plugin_a/
│  │  ├─ plugin_a_dependency.dll
│  ├─ plugin_b/
│  │  ├─ plugin_b_dependency_1.dll
│  │  ├─ plugin_b_dependency_2.dll
│  │  ├─ dependency_of_plugin_b_dependency_2.dll
````
