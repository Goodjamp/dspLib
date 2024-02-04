cmake_minimum_required(VERSION 3.0.0)
project(DspLib)

set(LIB_SRC
    dspAPI/FirFilter.c
    dspAPI/FirFilter.h
    dspAPI/RollingAverage.c
    dspAPI/RollingAverage.h
    main.c
)

set (LIB_SRC_PATH 
    dspApi
)




include_directories(${LIB_SRC_PATH})

add_executable(${PROJECT_NAME} ${LIB_SRC})