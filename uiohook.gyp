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
		"cflags": [
			"-std=c99"
		],
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
		"include_dirs": [
			'libs/libuiohook/include',
			'libs/libuiohook/src'
		]
	}]
}