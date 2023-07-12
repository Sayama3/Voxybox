mkdir build-bat
cd build-bat
cmake ..
cmake --build . --config Release

copy .\lib\VoxymoreCore\Release\*.dll .\Release\
copy .\lib\VoxymoreCore\Release\*.lib .\Release\
copy .\lib\VoxymoreCore\Release\*.so .\Release\
copy .\lib\VoxymoreCore\Release\*.dylib .\Release\

explorer .\Release\