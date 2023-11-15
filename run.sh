rm -rf build
mkdir -p build
cd build
cmake ..
#cmake -DCMAKE_BUILD_TYPE=Debug ..
make
cd ..
cd test
bash testing.sh