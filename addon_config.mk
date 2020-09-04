meta:
	ADDON_NAME = ofxNDI
	ADDON_DESCRIPTION = NewTek NDI SDK Wrapper for openFrameworks. https://www.newtek.com/ndi/
	ADDON_AUTHOR = Nariaki Iwatani
	ADDON_TAGS = "NDI" "Network" "Video"
	ADDON_URL = https://github.com/nariakiiwatani/ofxNDI

common:
	ADDON_INCLUDES = libs/NDI/include
	ADDON_INCLUDES += libs/utils
	ADDON_INCLUDES += src
	ADDON_INCLUDES += src/utils
osx:
	ADDON_LDFLAGS = -Xlinker -rpath -Xlinker @executable_path
vs:
	# x64
	ADDON_DLLS_TO_COPY += windows-copy-to-bin/Processing.NDI.Lib.x64.dll
	# Win32
	ADDON_DLLS_TO_COPY += windows-copy-to-bin/Processing.NDI.Lib.x86.dll

