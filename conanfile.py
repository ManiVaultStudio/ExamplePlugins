from conans import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain
from conans.tools import save, load
from rules_support import PluginBranchInfo
import os
import pathlib
import subprocess

class ExamplePluginsConan(ConanFile):
    """Class to package ExamplePlugins using conan

    Packages both RELEASE and DEBUG.
    Uses rules_support (github.com/ManiVaultStudio/rulessupport) to derive
    versioninfo based on the branch naming convention
    as described in https://github.com/ManiVaultStudio/core/wiki/Branch-naming-rules
    """

    name = "ExamplePlugins"
    description = (
        "A collection of examples including analys, data, loader and view plugins."
    )
    topics = ("hdps", "plugin", "examples", "various")
    url = "https://github.com/ManiVaultStudio/ExamplePlugins"
    author = "B. van Lew b.van_lew@lumc.nl"  # conan recipe author
    license = "MIT"

    # Options may need to change depending on the packaged library
    settings = {"os": None, "build_type": None, "compiler": None, "arch": None}
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": True, "fPIC": True}

    scm = {
        "type": "git",
        "subfolder": "hdps/ExamplePlugins",
        "url": "auto",
        "revision": "auto",
    }

    def __get_git_path(self):
        path = load(
            pathlib.Path(pathlib.Path(__file__).parent.resolve(), "__gitpath.txt")
        )
        print(f"git info from {path}")
        return path

    def export(self):
        # save the original source path to the directory used to build the package
        save(
            pathlib.Path(self.export_folder, "__gitpath.txt"),
            str(pathlib.Path(__file__).parent.resolve()),
        )

    def set_version(self):
        # Assign a version from the branch name
        branch_info = PluginBranchInfo(self.recipe_folder)
        self.version = branch_info.version

    def requirements(self):
        branch_info = PluginBranchInfo(self.__get_git_path())
        print(f"Core requirement {branch_info.core_requirement}")
        self.requires(branch_info.core_requirement)

    # Remove runtime and use always default (MD/MDd)
    def configure(self):
        pass

    def system_requirements(self):
        #  May be needed for macOS or Linux
        pass

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def generate(self):
        generator = None
        if self.settings.os == "Macos":
            generator = "Xcode"
        if self.settings.os == "Linux":
            generator = "Ninja Multi-Config"

        tc = CMakeToolchain(self, generator=generator)

        if self.settings.os == "Windows" and self.options.shared:
            tc.variables["CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS"] = True
        if self.settings.os == "Linux" or self.settings.os == "Macos":
            tc.variables["CMAKE_CXX_STANDARD_REQUIRED"] = "ON"

        # Use the Qt provided .cmake files
        qt_path = pathlib.Path(self.deps_cpp_info["qt"].rootpath)
        qt_cfg = list(qt_path.glob("**/Qt6Config.cmake"))[0]
        qt_root = qt_cfg.parents[0].as_posix()
        qt_dir = qt_cfg.parents[3].as_posix()

        # for Qt >= 6.4.2
        #print("Qt6_DIR: ", qt_root)
        #tc.variables["Qt6_DIR"] = qt_root

        # for Qt < 6.4.2
        tc.variables["CMAKE_PREFIX_PATH"] = qt_dir

        # Use the ManiVault .cmake files
        mv_core_root = self.deps_cpp_info["hdps-core"].rootpath
        self.manivault_dir = pathlib.Path(mv_core_root, "cmake", "mv").as_posix()

        # Find ManiVault with find_package
        tc.variables["ManiVault_DIR"] = self.manivault_dir
        print("ManiVault_DIR: ", self.manivault_dir)

        # Set some build options
        #tc.variables["MV_UNITY_BUILD"] = "ON"

        # Install vcpkg dependencies
        if os.environ.get("VCPKG_ROOT", None):
            vcpkg_dir = pathlib.Path(os.environ["VCPKG_ROOT"])
            vcpkg_exe = vcpkg_dir / "vcpkg.exe" if self.settings.os == "Windows" else vcpkg_dir / "vcpkg" 
            vcpkg_tc  = vcpkg_dir / "scripts" / "buildsystems" / "vcpkg.cmake"

            vcpkg_triplet = "x64-windows"
            if self.settings.os == "Macos":
                vcpkg_triplet = "x64-osx"
            if self.settings.os == "Linux":
                vcpkg_triplet = "x64-linux"

            print("vcpkg_dir: ", vcpkg_dir)
            print("vcpkg_exe: ", vcpkg_exe)
            print("vcpkg_tc: ", vcpkg_tc)
            print("vcpkg_triplet: ", vcpkg_triplet)

            tc.variables["VCPKG_LIBRARY_LINKAGE"]   = "dynamic"
            tc.variables["VCPKG_TARGET_TRIPLET"]    = vcpkg_triplet
            tc.variables["VCPKG_HOST_TRIPLET"]      = vcpkg_triplet
            tc.variables["VCPKG_ROOT"]              = vcpkg_dir.as_posix()

            tc.variables["CMAKE_PROJECT_INCLUDE"] = vcpkg_tc.as_posix()

        tc.generate()

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.configure(build_script_folder="hdps/ExamplePlugins")
        cmake.verbose = True
        return cmake

    def build(self):
        print("Build OS is: ", self.settings.os)

        cmake = self._configure_cmake()
        cmake.build(build_type="Debug")
        cmake.build(build_type="Release")

    def package(self):
        package_dir = pathlib.Path(self.build_folder, "package").as_posix()
        print("Packaging install dir: ", package_dir)
        subprocess.run(
            [
                "cmake",
                "--install",
                self.build_folder,
                "--config",
                "Debug",
                "--prefix",
                pathlib.Path(package_dir, "Debug").as_posix(),
            ]
        )
        subprocess.run(
            [
                "cmake",
                "--install",
                self.build_folder,
                "--config",
                "Release",
                "--prefix",
                pathlib.Path(package_dir, "Release").as_posix(),
            ]
        )
        self.copy(pattern="*", src=package_dir)

    def package_info(self):
        self.cpp_info.debug.libdirs = ["Debug/lib"]
        self.cpp_info.debug.bindirs = ["Debug/Plugins", "Debug"]
        self.cpp_info.debug.includedirs = ["Debug/include", "Debug"]
        self.cpp_info.release.libdirs = ["Release/lib"]
        self.cpp_info.release.bindirs = ["Release/Plugins", "Release"]
        self.cpp_info.release.includedirs = ["Release/include", "Release"]
