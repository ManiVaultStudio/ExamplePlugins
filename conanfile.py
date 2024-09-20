from conans import ConanFile
from conan.tools.cmake import CMakeDeps, CMake, CMakeToolchain
from conans.tools import save, load
import os
import shutil
import pathlib
import subprocess
from rules_support import PluginBranchInfo
import re

def compatibility(os, compiler, compiler_version):
    # On macos fallback to zlib apple-clang 13
    if os == "Macos" and compiler == "apple-clang" and bool(re.match("14.*", compiler_version)):  
        print("Compatibility match")
        return ["zlib/1.3:compiler.version=13"]
    return None

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

    short_paths = True
    generators = "CMakeDeps"

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
        print("In export")
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

        tc.variables["CMAKE_CXX_STANDARD_REQUIRED"] = "ON"

        # Use the Qt provided .cmake files
        qtpath = pathlib.Path(self.deps_cpp_info["qt"].rootpath)
        qt_root = str(list(qtpath.glob("**/Qt6Config.cmake"))[0].parents[3].as_posix())
        tc.variables["CMAKE_PREFIX_PATH"] = qt_root

        # Set the installation directory for ManiVault based on the MV_INSTALL_DIR environment variable
        # or if none is specified, set it to the build/install dir.
        if not os.environ.get("MV_INSTALL_DIR", None):
            os.environ["MV_INSTALL_DIR"] = os.path.join(self.build_folder, "install")
        print("MV_INSTALL_DIR: ", os.environ["MV_INSTALL_DIR"])
        self.install_dir = pathlib.Path(os.environ["MV_INSTALL_DIR"]).as_posix()
        
        # Find ManiVault with find_package
        self.manivault_dir = self.install_dir + '/cmake/mv/'
        tc.variables["ManiVault_DIR"] = self.manivault_dir

        # Set some build options
        tc.variables["MV_UNITY_BUILD"] = "ON"
        
        tc.variables["CMAKE_TOOLCHAIN_FILE"] = os.environ.get("VCPKG_ROOT", None) + '/scripts/buildsystems/vcpkg.cmake/'
        tc.variables["CMAKE_PROJECT_INCLUDE"] = "build/conan_toolchain.cmake"
        
        tc.generate()

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.configure(build_script_folder="hdps/ExamplePlugins")
        cmake.verbose = True
        return cmake

    def build(self):
        print("Build OS is : ", self.settings.os)

        # The ExamplePlugins build expects the ManiVaultStudio package to be in this install dir
        hdps_pkg_root = self.deps_cpp_info["hdps-core"].rootpath
        print("Install dir type: ", self.install_dir)
        shutil.copytree(hdps_pkg_root, self.install_dir)

        cmake = self._configure_cmake()
        cmake.build(build_type="Debug")
        cmake.install(build_type="Debug")

        # cmake_release = self._configure_cmake()
        cmake.build(build_type="Release")
        cmake.install(build_type="Release")

    def package(self):
        package_dir = os.path.join(self.build_folder, "package")
        print("Packaging install dir: ", package_dir)
        subprocess.run(
            [
                "cmake",
                "--install",
                self.build_folder,
                "--config",
                "Debug",
                "--prefix",
                os.path.join(package_dir, "Debug"),
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
                os.path.join(package_dir, "Release"),
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
