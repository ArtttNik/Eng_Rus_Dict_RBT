cmake_minimum_required(VERSION 3.30)
project(Eng_Rus_Dict_RBT)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXE_LINKER_FLAGS "-static")

if(WIN32)
    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/cmake/icon)

    set(ICON_FILE ${CMAKE_SOURCE_DIR}/cmake/icon/app_icon.ico)
    set(RC_FILE ${CMAKE_SOURCE_DIR}/cmake/icon/app_icon.rc
    )

    file(WRITE ${RC_FILE} "IDI_ICON1 ICON \"${ICON_FILE}\"\n")
endif()

add_executable(Eng_Rus_Dict_RBT
        ${RC_FILE}
        main.cpp
        src/RBT.cpp
        headers/RBT.h
        headers/Exceptions.h
        src/Dict.cpp
        headers/Dict.h
        src/Structs.cpp
        headers/Structs.h
        src/MyJson.cpp
        headers/MyJson.h
        tests/Tests.h
        tests/Tests.cpp
)