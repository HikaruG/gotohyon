

mkdir build 
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make -s -j4
cd ..
copy libxml2\lib\libxml2.dll bin\
