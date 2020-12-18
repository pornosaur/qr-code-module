cmake_minimum_required(VERSION 3.5)
project(qr-code-module)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3")

set(LIBS ZXing)

set(SRC_FILES
        src/qr_encoder.cpp
        src/qr_decoder.cpp
        )

set(INCLUDE include/qrmodule/qr_decoder.h
        include/qrmodule/qr_encoder.h
        include/qrmodule/image_view.h
        )

include_directories(include)

##########ZXING LIB DEPENDENCY###########
add_subdirectory(external/zxing-cpp)
include_directories(external/zxing-cpp/core/src/)
include_directories(external/zxing-cpp/thirdparty/)
#########################################

add_library(${PROJECT_NAME}-lib SHARED ${SRC_FILES})
target_include_directories(${PROJECT_NAME}-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/external/zxing-cpp/thirdparty/stb)
target_link_libraries(${PROJECT_NAME}-lib ${LIBS})
