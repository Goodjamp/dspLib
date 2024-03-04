cmake_minimum_required(VERSION 3.0.0)
project(DspLib)

set(LIB_SRC
    DspApi/FirFilter.c
    DspApi/FirFilter.h
    DspApi/RollingAverage.c
    DspApi/RollingAverage.h
    DspApi/Fft.c
    DspApi/Fft.h
    DspApi/FftRotateCoeff.c
    #DspApi/FftRotateCoeff.h
    main.c
)

set (LIB_SRC_PATH 
    DspApi
)

include_directories(${LIB_SRC_PATH})

add_custom_command (
    OUTPUT ${CMAKE_SOURCE_DIR}/DspApi/FftRotateCoeff.c
    COMMENT "Calculate rotate coeff"
    COMMAND python.exe ${CMAKE_SOURCE_DIR}/RotateCoefGen.py ${FFT_COEFF_NUMBER} ${CMAKE_SOURCE_DIR}/DspApi/
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(${PROJECT_NAME} ${LIB_SRC})