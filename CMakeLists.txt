cmake_minimum_required(VERSION 3.7)

project(plotjuggler_sample_plugins)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#-------  Include Qt dependencies -------
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)

find_package(fmt)

find_package(Qt5 REQUIRED COMPONENTS
    Core
    Widgets
    Concurrent
    Xml
    Svg
    OpenGL)

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Xml_INCLUDE_DIRS}
    ${Qt5Svg_INCLUDE_DIRS} )

set(QT_LIBRARIES
    Qt5::Core
    Qt5::Widgets
    Qt5::Xml
    Qt5::Svg )

add_definitions( ${QT_DEFINITIONS} -DQT_PLUGIN )
set( PJ_LIBRARIES ${QT_LIBRARIES} )

find_package(plotjuggler REQUIRED)
message(STATUS "PlotJuggler FOUND")
message(STATUS "plotjuggler_INCLUDE_DIR: ${plotjuggler_INCLUDE_DIR}")
message(STATUS "plotjuggler_LIBRARIES: ${plotjuggler_LIBRARIES}")
link_directories(${plotjuggler_INCLUDE_DIR}/../lib)
include_directories(${plotjuggler_INCLUDE_DIR})
list(APPEND ${PJ_LIBRARIES} ${plotjuggler_LIBRARIES} )
set(PJ_PLUGIN_INSTALL_DIRECTORY bin )

#------- Create the libraries -------

add_library(PlotJugglerGobyJSON SHARED
   DataLoadGobyJSON/nlohmann_parsers.h
   DataLoadGobyJSON/nlohmann_parsers.cpp
   DataLoadGobyJSON/dataload_goby_json.h
   DataLoadGobyJSON/dataload_goby_json.cpp )

target_link_libraries(PlotJugglerGobyJSON
  ${QT_LIBRARIES}  plotjuggler_base plotjuggler_qwt lua5.3 fmt
  )

#------- Install the libraries -------
install(
    TARGETS
        PlotJugglerGobyJSON
    DESTINATION
        ${PJ_PLUGIN_INSTALL_DIRECTORY}  )

