copy /Y "%RECIPE_DIR%\CMakeLists.txt" CMakeLists.txt
if errorlevel 1 exit 1

cmake -G Ninja -B build -S . ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_INSTALL_PREFIX="%LIBRARY_PREFIX%" ^
    -DBUILD_SHARED_LIBS=ON
if errorlevel 1 exit 1

cmake --build build --target install
if errorlevel 1 exit 1
