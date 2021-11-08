call mkdir build
call cd build
call cmake .. -DBENCHMARK_ENABLE_TESTING=OFF -DBENCHMARK_ENABLE_LTO=ON
PAUSE