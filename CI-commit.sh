rm -rf build
mkdir build
cd build
cmake ..
cmake --build . -- all test package_source