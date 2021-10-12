{
	"targets": [{
		"target_name": "uiohook",
		"type": "shared_library",
		"sources": [
			"libs/libuiohook/include/uiohook.h",
			"libs/libuiohook/src/logger.c",
			"libs/libuiohook/src/logger.h",
			"libs/libuiohook/src/darwin/input_helper.h",
			"libs/libuiohook/src/darwin/input_helper.c",
			"libs/libuiohook/src/darwin/input_hook.c",
			"libs/libuiohook/src/darwin/post_event.c",
			"libs/libuiohook/src/darwin/system_properties.c"
		],
		"include_dirs": [
			'libs/libuiohook/include',
			'libs/libuiohook/src'
		],
		"conditions": [
			['OS=="mac"', {
				"defines": [
					'USE_IOKIT=1',
					'USE_OBJC=1'
				],
				"link_settings": {
					"libraries": [
						"-framework IOKit",
						"-framework Carbon",
						"-framework ApplicationServices",
						"-lobjc",
						"-Wl,-rpath,@executable_path/.",
						"-Wl,-rpath,@loader_path/.",
						"-Wl,-rpath,<!(pwd)/build/Release/"
					]
				},
			}],
            ['OS=="linux"', {
                "link_settings": {
                    "libraries": [
                        "-Wl,-rpath,<!(node -e \"console.log('builds/' + process.env.gyp_iohook_runtime + '-v' + process.env.gyp_iohook_abi + '-' + process.env.gyp_iohook_platform + '-' + process.env.gyp_iohook_arch + '/build/Release')\")",
                        "-Wl,-rpath,<!(pwd)/build/Release/",
                        "-lX11",
                        "-lX11-xcb",
                        "-lxkbcommon-x11",
                        "-lxkbcommon",
                        "-lXtst"
                    ]
                },
                "defines": [
                    "USE_XKBCOMMON"
                ],
            }]
		]
	}]
}