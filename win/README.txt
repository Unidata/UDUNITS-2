This is an attempt to port UDUNITS2 on MS Windows.

*Building with Visual Studio 2012 solution*

Simply open the solution supplied in the build-vs11 subdirectory and build all. The solution gather 6 projects :
1. The expat library 
2. The udunits library 
3. The windows port library, including search, regex, getopt and glog POSIX specific port
4. The utility executable
5. The CUnit testing framework
6. The test suite

Note - The C runtime is set to (multithreaded) :
 * Shared dll for debug builds (/MDd)
 * Static for release builds (/MT)