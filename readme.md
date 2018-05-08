# ofxNDI

NewTek NDI SDK Wrapper for openFrameworks.

Works on Mac, iOS, and Windows.

## Warning
- This addon wraps all features of original SDK, but some of them are not tested. 
- Receivers on mobile OS won't work because they are not supported by SDK currently.

## Xcode Project setting (osx)

* Build Phases > Copy Files
  * select "Destination" to Executables
  * click `+`
    * add `libndi.3.dylib`
* Build Settings > Linking > Runpath Search Paths
  * set value to  `@executable_path`

