cmake_minimum_required(VERSION 3.20)
project(MIPT_CPP_Homeworks)

set(CMAKE_CXX_STANDARD 23)
set(SUBDIRS 
    1_usr_sys 
    2_list 
    3_type_map 
    4_mix_in 
    5_singleton 
    6_kp 
    7_bridge 
    8_comp
)

foreach(DIR ${SUBDIRS})
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/main.cpp)
        add_executable(${DIR} ${DIR}/main.cpp)
        target_include_directories(${DIR} PRIVATE ${DIR})
    endif()
endforeach()

target_include_directories(3_type_map PRIVATE 2_list)
