{
  "targets": [
    {
      "target_name": "iohook",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "sources": [
        "src/index.cc",
        "src/index.h",
      ],
      "dependencies": [
        "./uiohook.gyp:uiohook"
      ],
      "include_dirs": [
        "libs/libuiohook/include",
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
          },
        }]
      ]
    }
  ]
}
