# ManiVault Example Plugins

[ManiVault](https://github.com/ManiVaultStudio/Core) can currently be extended by six types of plugins, each dedicated to a specific functionality. Here, you can find exemplary implementations for each plugins type that can be used as a starting point for new plugins:
- [Analysis](https://github.com/ManiVaultStudio/ExamplePlugins/tree/master/ExampleAnalysis): general analytics methods, e.g., dimensionality reduction
- [Transformation](https://github.com/ManiVaultStudio/ExamplePlugins/tree/master/ExampleTransformation): produces new data in the same shape as the input, e.g., normalizations
- [Data](https://github.com/ManiVaultStudio/ExamplePlugins/tree/master/ExampleData): defines data types
- [Loader](https://github.com/ManiVaultStudio/ExamplePlugins/tree/master/ExampleLoader)/[Writer](https://github.com/ManiVaultStudio/ExamplePlugins/tree/master/ExampleWriter): load data into the system or write them to disk 
- [View](https://github.com/ManiVaultStudio/ExamplePlugins/tree/master/ExampleView): visualize data and provide interaction, e.g., using [OpenGL](https://github.com/ManiVaultStudio/ExamplePlugins/tree/master/ExampleViewOpenGL) or [JavaScript](https://github.com/ManiVaultStudio/ExamplePlugins/tree/master/ExampleViewJS) backends

## Installation & Building
<details open>

<summary>Manual</summary>

## Requirements
* ManiVault Studio (https://github.com/ManiVaultStudio/Core)
* CMake 3.22+ (https://cmake.org/)
* Qt 6.8+ (https://www.qt.io/download), additional libraries: Qt WebEngine, Qt WebChannel, Qt Positioning and the Qt5 Compatibility Module
* A C++20 compatible compiler
    * For Windows: Visual Studio 2022 (https://visualstudio.microsoft.com/downloads/)
* Optionally: Git (https://git-scm.com/)

## Download
Be sure to first have ManiVault installed before continuing to download this repository. You can do the latter in any way you like, for example by downloading a .zip file of this repository (https://github.com/ManiVaultStudio/ExamplePlugins/archive/refs/heads/master.zip) or with 
```
git clone https://github.com/ManiVaultStudio/ExamplePlugins.git
```
With SSH this would be:
```
git clone git@github.com:ManiVaultStudio/ExamplePlugins.git
```
After unzipping you should have an `ExamplePlugins-master`, after cloning you should have an `ExamplePlugins` folder, after navigating to this folder, continue with the next step.

## Project setup
You'll need to define a `ManiVault_DIR` and setup CMake such that it can fin Qt. The `ManiVault_DIR` variable is used to find your local ManiVault installation and defines an install location for the example plugins. 

For Windows, using the CMake GUI:
1. Launch CMake (available here: https://cmake.org/)
2. In the source code field browse to the `ExamplePlugins-master` or `ExamplePlugins` folder (which contains CMakeLists.txt).
3. In the build field browse to the `ExamplePlugins-master/build` or `ExamplePlugins/build` folder. It will create it if it doesn't exist.

   The source and build fields in your cmake should now roughly look like this:

   `Source: C:/YourOwnFolder/ExamplePlugins-master`  
   `Build:  C:/YourOwnFolder/ExamplePlugins-master/build`
   
4. Press `Configure` and select the `Visual Studio 17 2022` generator with the optional platform of `x64`. If it is not available, be sure to install Visual Studio. Press Finish to configure the project.
![Example_cmake](https://github.com/user-attachments/assets/b86e6e19-0fc1-40ec-8397-04e5ee8a7f87)
5. A lot of red paths should now appear. Set the `ManiVault_DIR` to the subfolder of your ManiVault install directory (which should hold `Debug`, `Release` and a `cmake` folder) that contains a `ManiVaultConfig.cmake` file. That should be `YOUR_MANIVAULT_INSTALL_DIR/cmake/mv`. Check that the entries relating to Qt seem correct and then press `Generate` to generate the Visual Studio solution. If cmake is asking you for a Qt6_DIR please refer to [CMake cannot find my Qt Installation](https://github.com/ManiVaultStudio/PublicWiki/wiki/FAQ#cmake-cannot-find-my-qt-installation).
6. Press `Open Project` to launch Visual Studio and the example project.

## Compiling
For Windows:
1. At the top of Visual Studio set the build mode (where it says `Debug`) to `Release`.
2. Right click the project `ALL_BUILD` in the Solution Explorer and select Properties.
3. In the Configuration Properties -> Debugging set the `Command` by browsing to your `ManiVault Studio.exe` file. Set the `Working Directory` field by browsing to the folder containing the `ManiVault Studio.exe` file.
4. Press Apply and OK and right click the project in the Solution Explorer and press Build.
5. If all is well the build should succeed and if you run the project with Ctrl+F5 it will launch `ManiVault Studio.exe` and load the Example project as an available plug-in.

</details>

<details>
   
<summary>Automated using DevBundle (available for BioVault team members)</summary>

The preferred approach to building the example plugins is by using our [DevBundle](https://github.com/ManiVaultStudio/DevBundle) tool. This cross-platform tool creates self-contained development environments using build configurations in JSON format. Since the build environments are self-contained, multiple build environments can exist side-by-side. The major advantage of using DevBundle is that it will remove much of the configuration overhead by:
- Cloning repositories from the build configuration (with the branch specified in the build configuration)
- Downloading related binary dependencies from our [Artifactory](https://lkeb-artifactory.lumc.nl/ui/login/) server (and adding/configuring paths in the `CMakeLists.txt`)
- Setting up an umbrella CMakeLists.txt which consists of all projects from the build configuration

Note: the `allmain` build config in the DevBundle [config.json](https://github.com/ManiVaultStudio/DevBundle/blob/master/config.json) contains an example of how to add the example plugins to a build configuration.

</details>
