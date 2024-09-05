if not exist "./build" mkdir "./build"
cl /std:c++20 /permissive /J /O2 /LD /EHsc /Fo./build\ /Fe:./build/ljd.dll ./*.cpp ./ast/*.cpp ./bytecode/*.cpp ./lua/*.cpp
