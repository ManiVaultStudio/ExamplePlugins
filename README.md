# ManiVault Example Plugins

[ManiVault](https://github.com/ManiVaultStudio/Core) can currently be extended by six types of plugins, each dedicated to a specific functionality. Here, you can find exemplary implementations for each plugins type that can be used as a starting point for new plugins:
- [Analysis](https://github.com/ManiVaultStudio/ExamplePlugins/tree/master/ExampleAnalysis): general analytics methods, e.g., dimensionality reduction
- [Transformation](https://github.com/ManiVaultStudio/ExamplePlugins/tree/master/ExampleTransformation): produces new data in the same shape as the input, e.g., normalizations
- [Data](https://github.com/ManiVaultStudio/ExamplePlugins/tree/master/ExampleData): defines data types
- [Loader](https://github.com/ManiVaultStudio/ExamplePlugins/tree/master/ExampleLoader)/[Writer](https://github.com/ManiVaultStudio/ExamplePlugins/tree/master/ExampleWriter): load data into the system or write them to disk 
- [View](https://github.com/ManiVaultStudio/ExamplePlugins/tree/master/ExampleView): visualize data and provide interaction, e.g., using [OpenGL](https://github.com/ManiVaultStudio/ExamplePlugins/tree/master/ExampleViewOpenGL) or [JavaScript](https://github.com/ManiVaultStudio/ExamplePlugins/tree/master/ExampleViewJS) backends

## Requirements
* ManiVault (https://github.com/ManiVaultStudio/Core)
* CMake 3.17+ (https://cmake.org/)
* Qt 6.3.1+ (https://www.qt.io/download), additional libraries: Qt WebEngine, Qt WebChannel, Qt Positioning and the Qt5 Compatibility Module
* A C++17 compatible compiler
    * For Windows: Visual Studio 2019 or 2022 (https://visualstudio.microsoft.com/downloads/)
* Optionally: Git (https://git-scm.com/)

## Download
Be sure to first have ManiVault installed before continuing to download this repository. You can do the latter in any way you like, for example by downloading a .zip file of this repository (https://github.com/ManiVaultStudio/ExamplePlugins/archive/refs/heads/master.zip) or with 
```
git clone https://github.com/ManiVaultStudio/ExamplePlugins.git
```
After unzipping the download file or navigating to the cloned folder, continue with the next step.

## Project setup
You'll need to define a `MV_INSTALL_DIR` variable and provide it to CMake, as described in the ManiVault install guide. This variable is used to find your local ManiVault installation and defines an install location for the example plugins. 

For Windows, using the CMake GUI:
1. Launch CMake (available here: https://cmake.org/)
2. In the source code field browse to the `ExamplePlugins-master` folder (which contains CMakeLists.txt).
3. In the build field browse to the `ExamplePlugins-master/build` folder. It will create it if it doesn't exist.
4. Press `Configure` and select the `Visual Studio 15 2019` (or 16 2022) generator with the optional platform of `x64`. If it is not available, be sure to install Visual Studio. Press Finish to configure the project.
5. A lot of red paths should now appear. Check that the ones pointing to Qt directories seem correct and then press `Generate` to generate the Visual Studio solution.
6. Press `Open Project` to launch Visual Studio and the example project.

## Compiling
For Windows:
1. At the top of Visual Studio set the build mode (where it says `Debug`) to `Release`.
2. Right click the project `ExamplePlugins` in the Solution Explorer and select Set as StartUp Project.
3. Right click the project again and select Properties.
4. In the Configuration Properties -> Debugging set the `Command` by browsing to your `HDPS.exe` file. Set the `Working Directory` field by browsing to the folder containing the `HDPS.exe` file.
5. Press Apply and OK and right click the project in the Solution Explorer and press Build.
6. If all is well the build should succeed and if you run the project with Ctrl+F5 it will launch `HDPS.exe` and load the Example project as an available plug-in.
