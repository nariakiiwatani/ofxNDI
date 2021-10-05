meta:
	ADDON_NAME = ofxNDI
	ADDON_DESCRIPTION = NewTek NDI SDK Wrapper for openFrameworks. https://www.newtek.com/ndi/
	ADDON_AUTHOR = Nariaki Iwatani
	ADDON_TAGS = "NDI" "Network" "Video"
	ADDON_URL = https://github.com/nariakiiwatani/ofxNDI

osx:
	ADDON_LDFLAGS = -Xlinker -rpath -Xlinker @executable_path

vs:
	ADDON_DLLS_TO_COPY += windows-copy-to-bin/Processing.NDI.Lib.Advanced.x64.dll
	ADDON_DLLS_TO_COPY += windows-copy-to-bin/Processing.NDI.Lib.Advanced.x86.dll