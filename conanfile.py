import os
from conans import ConanFile, tools

class NpmMasMas(ConanFile):
    name = "asyncply"
    version = "1.0.3"
    license = "Attribution 4.0 International"
    url = "https://github.com/makiolo/asyncply"
    description = "This fast event system allows calls between two interfaces decoupled (sync or async)"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": True}
    generators = "cmake"
    short_paths = True

    def requirements(self):
        self.requires('fast-event-system/1.0.23@npm-mas-mas/testing')
        self.requires('teelogging/1.0.1@npm-mas-mas/testing')

    def source(self):
        self.run("git clone {}".format(self.url))

    def build(self):
        self.run("cd {} && CMAKI_INSTALL={} npm install && npm test".format(self.name, self.package_folder))

    def package(self):
        self.copy("{}/include/*.h".format(self.name), dst=os.path.join('include', self.name), keep_path=False)
        self.copy("{}/bin/{}/*.lib".format(self.name, self.settings.build_type), dst="lib", keep_path=False)
        self.copy("{}/bin/{}/*.dll".format(self.name, self.settings.build_type), dst="bin", keep_path=False)
        self.copy("{}/bin/{}/*_unittest".format(self.name, self.settings.build_type), dst="unittest", keep_path=False)
        self.copy("{}/bin/{}/*.so".format(self.name, self.settings.build_type), dst="lib", keep_path=False)
        self.copy("{}/bin/{}/*.dylib".format(self.name, self.settings.build_type), dst="lib", keep_path=False)
        self.copy("{}/bin/{}/*.a".format(self.name, self.settings.build_type), dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = [lib for lib in tools.collect_libs(self)]
        self.cpp_info.includedirs.append('include')

