# Include Qt5 and Qml
include_directories(${Qt5Widgets_INCLUDE_DIRS} ${QtQml_INCLUDE_DIRS})

# set Qt5 compile flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
# Set extra compile flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wpedantic -Wextra -fsanitize=address -fstrict-aliasing")

# Add definitions for Qt widgets and printsupport
add_definitions(${Qt5Widgets_DEFINITIONS} ${Qt5PrintSupport_DEFINITIONS} ${QtQml_DEFINITIONS} ${${Qt5Quick_DEFINITIONS}})

# Auto detect files using GLOB
if(NOT DEFINED HEADERS)
    file(GLOB_RECURSE HEADERS *.hpp)
endif()
if(NOT DEFINED SOURCES)
    file(GLOB_RECURSE SOURCES *.cpp)
endif()
if (NOT DEFINED RESOURCES)
    file(GLOB_RECURSE RESOURCES *.qrc)
endif()

qt5_add_resources(QT_RESOURCES ${RESOURCES})
source_group("Header Files" FILES ${HEADERS})
source_group("Source Files" FILES ${SOURCES})

add_executable(${PROJECT} ${HEADERS} ${SOURCES} ${QT_RESOURCES} ${GUI_TYPE})

target_link_libraries(${PROJECT}
        Qt5::PrintSupport
        Qt5::Widgets
        Qt5::Qml
        Qt5::Quick
)