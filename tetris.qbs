import "../solid/solid.qbs" as solid

solid {
    Application {
        name: "Tetris"

        files: [
            "data/bg.frag",
            "data/bg.vert",
            "data/cubemap.frag",
            "data/cubemap.vert",
            "data/gouraud.frag",
            "data/gouraud.vert",
            "data/app.rc",
            "data/app.res",
            "data/blender/bishop.blend",
            "data/blender/bishop.blend1",
            "data/blender/board.blend",
            "data/blender/boardandpieces.blend",
            "data/blender/boardandpieces.blend1",
            "data/blender/king.blend",
            "data/blender/king.blend1",
            "data/blender/knight.blend",
            "data/blender/knight.blend1",
            "data/blender/pawn.blend",
            "data/blender/pawn.blend1",
            "data/blender/queen.blend",
            "data/blender/queen.blend1",
            "data/blender/rook.blend",
            "data/blender/rook.blend1",
            "data/blender/shogi.blend",
            "data/blender/shogi.blend1",
            "data/gui.frag",
            "data/gui.vert",
            "data/icon.png",
            "data/icon_1024x1024.png",
            "data/icon_128x128.png",
            "data/icon_16x16.png",
            "data/icon_256x256.png",
            "data/icon_32x32.png",
            "data/icon_512x512.png",
            "data/icon_64x64.png",
            "data/icon_96x96.png",
            "data/logo_inv_jpg.jpg",
            "data/phong.frag",
            "data/phong.vert",
            "data/project.json",
            "data/simple.frag",
            "data/simple.vert",
            "data/solid.ico",
            "data/vertexanimation.frag",
            "data/vertexanimation.vert",
            "source/main.cpp",
            "source/main.h",
        ]

        Depends { name: "core"  }
        Depends { name: "nullrenderer"  }
        Depends { name: "nullphysics"  }
        Depends { name: "nullaudio"  }
        Depends { name: "nullphysics"  }
        Depends { name: "nullfilesystem"  }
        Depends { name: "gles2renderer"  }
        Depends { name: "stdfilesystem"  }
        Depends { name: "portaudioaudio"  }

        property stringList includePaths: "../solid/source"

        Properties {
            condition: qbs.targetOS.contains("macos")

            cpp.frameworks: macosFrameworks

            cpp.dynamicLibraries: macosSharedLibs
            cpp.staticLibraries: staticLibs.concat("SDL2")

            cpp.libraryPaths: [project.buildDirectory, "../solid/lib/debug/darwin/x86_64"]
            cpp.includePaths: includePaths.concat("../solid/include/darwin")
            cpp.defines: project.defines.concat(project.sdlDefines)
        }

        Properties {
            condition: qbs.targetOS.contains("linux")

            //cpp.dynamicLibraries: linuxSharedLibs
            cpp.staticLibraries: staticLibs.concat("SDL2")

            cpp.libraryPaths: [project.buildDirectory, "../solid/lib/debug/linux/x86_64"]
            cpp.includePaths: includePaths.concat("../solid/include/linux")
            cpp.defines: project.defines.concat(project.sdlDefines)
        }

        Properties {
            condition: qbs.targetOS.contains("windows")

            cpp.dynamicLibraries: windowsSharedLibs
            cpp.staticLibraries: staticLibs

            cpp.libraryPaths: [project.buildDirectory, "../solid/lib/debug/mingw32/x86"]
            cpp.includePaths: includePaths.concat("../solid/include/mingw32")
            cpp.defines: project.defines.concat(project.windowsDefines)
        }

        Depends { name: "cpp" }
        Depends { name: "core" }
        Depends { name: "nullphysics" }
        Depends { name: "bulletphysics" }
    }
}
