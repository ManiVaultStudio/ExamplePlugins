# ExamplePlugins

## Requirements
* Git (https://git-scm.com/)
* CMake 3.1+ (https://cmake.org/)
* Qt 5.13 (https://www.qt.io/download)
* Visual Studio 2017 (only for this guide https://visualstudio.microsoft.com/downloads/)

## Setting up
The first step is to pull the repository from github. You can do this in any way you like, but the easiest way is to download a .zip file of the repository from: https://github.com/hdps/ExamplePlugins/archive/master.zip.

After unzipping the folder continue to the next step.

## Building
1. Launch CMake (available here: https://cmake.org/)
2. In the source code field browse to the `ExamplePlugins-master` folder (which contains CMakeLists.txt).
3. In the build field browse to the `ExamplePlugins-master/build` folder. It will create it if it doesn't exist.
4. Press `Configure` and select the `Visual Studio 15 2017` generator with the optional platform of `x64`. If it is not available, be sure to install Visual Studio 2017. Press Finish to configure the project.
5. A lot of red paths should now appear. Check that the ones pointing to Qt directories seem correct and then press `Generate` to generate the Visual Studio solution.
6. Press `Open Project` to launch Visual Studio and the example project.

## Compiling
1. At the top of Visual Studio set the build mode (where it says `Debug`) to `Release`.
2. Right click the project `ExamplePlugins` in the Solution Explorer and select Set as StartUp Project.
3. Right click the project again and select Properties.
4. In the Configuration Properties -> Debugging set the `Command` by browsing to your `HDPS.exe` file. Set the `Working Directory` field by browsing to the folder containing the `HDPS.exe` file.
5. Press Apply and OK and right click the project in the Solution Explorer and press Build.
6. If all is well the build should succeed and if you run the project with Ctrl+F5 it will launch `HDPS.exe` and load the Example project as an available plug-in.
