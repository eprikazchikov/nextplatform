import qbs

Project {
    id: next
    property stringList srcFiles: [
        "src/*.cpp",
        "src/core/*.cpp",
        "src/math/*.cpp",
        "src/anim/*.cpp",
        "inc/*.h",
        "inc/core/*.h",
        "inc/math/*.h",
        "inc/anim/*.h"
    ]

    property stringList incPaths: [
        "inc",
        "inc/core",
        "inc/math",
        "inc/anim"
    ]

    DynamicLibrary {
        name: "next"
        files: next.srcFiles
        Depends { name: "cpp" }
        Depends { name: "bundle" }
        bundle.isBundle: false

        cpp.defines: ["NEXT_SHARED", "NEXT_LIBRARY"]
        cpp.includePaths: next.incPaths
        cpp.cxxLanguageVersion: "c++14"
        cpp.cxxStandardLibrary: "libc++"
    }

    QtApplication {
        name: "next-tests"
        files: [
            "tests/*.cpp",
            "tests/*.h"
        ]

        Depends { name: "cpp" }
        Depends { name: "next" }
        Depends { name: "Qt"; submodules: ["core", "test"]; }
        bundle.isBundle: false

        consoleApplication: true
        cpp.includePaths: [
            "inc",
            "inc/core",
            "inc/math",
            "inc/anim"
        ]
        cpp.cxxLanguageVersion: "c++14"

    }
}
