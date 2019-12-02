{
    "targets": [{
        "target_name": "rpserver",
        "cflags!": ["-Wno-psabi", "-Wunused-result", "-fno-exceptions"],
        "cflags_cc!": ["-Wno-psabi", "-Wunused-result", "-fno-exceptions"],
        "sources": [
            "cppFiles/Sources/Exported/mainServer.cpp",
            "cppFiles/Sources/Exported/ServerFunctions.cpp",
            "cppFiles/Headers/Exported/ServerFunctions.h",
            "cppFiles/Headers/Point.h",
            "cppFiles/Sources/Point.cpp",
            "cppFiles/Headers/Exported/PointWrapper.h",
            "cppFiles/Sources/Exported/PointWrapper.cpp",
            "cppFiles/Headers/config.h"
        ],
        'include_dirs': [
            "<!@(node -p \"require('node-addon-api').include\")"
        ],
        'libraries': [],
        'dependencies': [
            "<!(node -p \"require('node-addon-api').gyp\")"
        ],
        'defines': ['NAPI_DISABLE_CPP_EXCEPTIONS']
    }]
}