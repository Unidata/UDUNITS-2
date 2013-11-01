set PATH=C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0
set PATH=%PATH%;C:\Program Files (x86)\Cmake 2.8\bin;C:\MinGW\bin
set PATH=%PATH%;C:\Program Files (x86)\Expat 2.1.0\Bin
set CMAKE_INCLUDE_PATH=C:\Program Files (x86)\Expat 2.1.0\Source\lib;C:\Users\vagrant\include
set CMAKE_LIBRARY_PATH=C:\Program Files (x86)\Expat 2.1.0\Bin;C:\Users\vagrant\lib
cmake -G "MinGW Makefiles" . && cmake --build . -- all test install install_test package