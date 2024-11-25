import os

from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.files import copy


class ImGuiExample(ConanFile):
    description = "ConanFile for IRC"
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires("spdlog/1.13.0")
        self.requires("boost/1.86.0")

    def generate(self):
        toolchain = CMakeToolchain(self)
        dependencies = CMakeDeps(self)
        toolchain.generate()
        dependencies.generate()

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
