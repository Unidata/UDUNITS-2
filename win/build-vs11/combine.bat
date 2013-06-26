call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\vcvarsall.bat" x86

rem Win32\Debug
copy /Y Win32\Debug\lib.lib Win32\Debug\udunits2d.lib
lib Win32\Debug\port.lib /extract:Win32\Debug\search.obj /out:Win32\Debug\search.obj
lib Win32\Debug\udunits2d.lib Win32\Debug\expat.lib Win32\Debug\search.obj

rem Win32\Release
copy /Y Win32\Release\lib.lib Win32\Release\udunits2.lib
lib Win32\Release\port.lib /extract:Win32\Release\search.obj /out:Win32\Release\search.obj
lib /LTCG Win32\Release\udunits2.lib Win32\Release\expat.lib Win32\Release\search.obj

call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\vcvarsall.bat" amd64

rem x64\Debug
copy /Y x64\Debug\lib.lib x64\Debug\udunits2d.lib
lib x64\Debug\port.lib /extract:x64\Debug\search.obj /out:x64\Debug\search.obj
lib x64\Debug\udunits2d.lib x64\Debug\expat.lib x64\Debug\search.obj

rem x64\Release
copy /Y x64\Release\lib.lib x64\Release\udunits2.lib
lib x64\Release\port.lib /extract:x64\Release\search.obj /out:x64\Release\search.obj
lib /LTCG x64\Release\udunits2.lib x64\Release\expat.lib x64\Release\search.obj