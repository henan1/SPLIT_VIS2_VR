cmake_minimum_required(VERSION 2.8.12)
project(SPLIT_VIS2)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})

include(ExternalProject)
set(BUILD_SHARED_LIBS OFF) # force static libraries

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
message("OPENMP FOUND")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)


set(genVTK_SOURCES
    preprocessing/generateVTK/genVTK.cpp
    preprocessing/generateVTK/genVTKfromOrg.cpp
    preprocessing/generateVTK/genVTKfromOrg.h)

add_executable(genVTK ${genVTK_SOURCES})
install(TARGETS genVTK RUNTIME DESTINATION bin)

set(Contour_SOURCES
    preprocessing/generatecontour/main.cpp
    preprocessing/generatecontour/svContour.cpp
    preprocessing/generatecontour/svContour.h)

add_executable(Contour ${Contour_SOURCES})
target_link_libraries(Contour ${VTK_LIBRARIES})
install(TARGETS Contour RUNTIME DESTINATION bin)

ExternalProject_Add(glui
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/glui-2.35
    CONFIGURE_COMMAND ""
    BUILD_COMMAND cd src && make
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND "")
ExternalProject_Get_Property(glui SOURCE_DIR)
set(GLUI_LIBRARIES ${SOURCE_DIR}/src/lib/libglui.a CACHE STRING "" FORCE)


ExternalProject_Add(kmlocal
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/kmlocal-1.7.2
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/bin/kmlsample
        ${CMAKE_CURRENT_BINARY_DIR}/../bin/)

ExternalProject_Add(jcUtil
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/jcUtil
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND "")
ExternalProject_Get_Property(jcUtil SOURCE_DIR)
set(JCUTIL_LIBRARIES ${SOURCE_DIR}/libjcUtil-1.0.a CACHE STRING "" FORCE)
set(JCUTIL_INCLUDE_DIR ${SOURCE_DIR} CACHE STRING "" FORCE)

ExternalProject_Add(LU
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/LU
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND "")
ExternalProject_Get_Property(LU SOURCE_DIR)
set(LU_LIBRARIES ${SOURCE_DIR}/libLU.a CACHE STRING "" FORCE)
set(svl_SOURCES
    libs/quanVisLib/svUtil.cpp
    libs/quanVisLib/svLut.cpp
    libs/quanVisLib/svParticle.cpp
    libs/quanVisLib/ivview.cpp
    libs/quanVisLib/ivTrackball.cpp
    libs/quanVisLib/svOutline.cpp
    libs/quanVisLib/svDistance.cpp
    libs/quanVisLib/svArrowGlyph.cpp
    libs/quanVisLib/svArrowSelection.cpp
    libs/quanVisLib/svSlice.cpp
    libs/quanVisLib/svQDOTdistancemap.cpp
    libs/quanVisLib/svQDOTmagnitudemap.cpp
    libs/quanVisLib/CIEColor.cpp
    libs/quanVisLib/svColors.cpp
    libs/quanVisLib/svMap.cpp
    libs/quanVisLib/svDirectArrow.cpp
    libs/quanVisLib/svMesh.cpp
    libs/quanVisLib/svSplitArrow.cpp
    libs/quanVisLib/svVectorField.cpp
    libs/quanVisLib/svQDOT.cpp
    libs/quanVisLib/svPickingray.cpp
    libs/quanVisLib/svDisc.cpp
    libs/quanVisLib/svAnalyticField.cpp
    libs/quanVisLib/svPrimitive.cpp
    libs/quanVisLib/svGlyph.cpp
    libs/quanVisLib/svSummaryGlyph.cpp
    libs/quanVisLib/svBarGlyph.cpp
    libs/quanVisLib/svBendBar.cpp
    libs/quanVisLib/svSplitVectorsBar.cpp
    libs/quanVisLib/svDirectBar.cpp
    libs/quanVisLib/svImage.cpp
    libs/quanVisLib/svQDOTImage.cpp
    libs/quanVisLib/svLegend.cpp
    libs/quanVisLib/svUtil.h
    libs/quanVisLib/svLut.h
    libs/quanVisLib/svParticle.h
    libs/quanVisLib/ivview.h
    libs/quanVisLib/ivTrackball.h
    libs/quanVisLib/svOutline.h
    libs/quanVisLib/svDistance.h
    libs/quanVisLib/svArrowGlyph.h
    libs/quanVisLib/svArrowSelection.h
    libs/quanVisLib/svSlice.h
    libs/quanVisLib/svQDOTdistancemap.h
    libs/quanVisLib/svQDOTmagnitudemap.h
    libs/quanVisLib/CIEColor.h
    libs/quanVisLib/svColors.h
    libs/quanVisLib/svMap.h
    libs/quanVisLib/svDirectArrow.h
    libs/quanVisLib/svMesh.h
    libs/quanVisLib/svSplitArrow.h
    libs/quanVisLib/svVectorField.h
    libs/quanVisLib/svQDOT.h
    libs/quanVisLib/svPickingray.h
    libs/quanVisLib/svDisc.h
    libs/quanVisLib/svAnalyticField.h
    libs/quanVisLib/svPrimitive.h
    libs/quanVisLib/svGlyph.h
    libs/quanVisLib/svSummaryGlyph.h
    libs/quanVisLib/svBarGlyph.h
    libs/quanVisLib/svBendBar.h
    libs/quanVisLib/svSplitVectorsBar.h
    libs/quanVisLib/svDirectBar.h
    libs/quanVisLib/svImage.h
    libs/quanVisLib/svQDOTImage.h
    libs/quanVisLib/svLegend.h
    )

configure_file(libs/quanVisLib/svConfig.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/svConfig.h)

add_library(svl ${svl_SOURCES}  ${CMAKE_CURRENT_BINARY_DIR}/svConfig.h)
target_include_directories(svl PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
set(SVL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/quanVisLib CACHE STRING "" FORCE)

add_executable(task1sample1m1 SplitTool/task1sample1m1.cpp)
target_link_libraries(task1sample1m1 svl ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} omega pthread glut)
target_include_directories(task1sample1m1 PRIVATE ${SVL_INCLUDE_DIR}
${JCUTIL_INCLUDE_DIR})
install(TARGETS task1sample1m1 RUNTIME DESTINATION bin)

add_executable(task1sample1m2 SplitTool/task1sample1m2.cpp)
target_link_libraries(task1sample1m2 svl ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} omega pthread glut)
target_include_directories(task1sample1m2 PRIVATE ${SVL_INCLUDE_DIR}
${JCUTIL_INCLUDE_DIR})
install(TARGETS task1sample1m2 RUNTIME DESTINATION bin)

add_executable(task1sample2m1 SplitTool/task1sample2m1.cpp)
target_link_libraries(task1sample2m1 svl ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} omega pthread glut)
target_include_directories(task1sample2m1 PRIVATE ${SVL_INCLUDE_DIR}
${JCUTIL_INCLUDE_DIR})
install(TARGETS task1sample2m1 RUNTIME DESTINATION bin)

add_executable(task1sample2m2 SplitTool/task1sample2m2.cpp)
target_link_libraries(task1sample2m2 svl ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} omega pthread glut)
target_include_directories(task1sample2m2 PRIVATE ${SVL_INCLUDE_DIR}
${JCUTIL_INCLUDE_DIR})
install(TARGETS task1sample2m2 RUNTIME DESTINATION bin)


add_executable(task2sample1m1 SplitTool/task2sample1m1.cpp)
target_link_libraries(task2sample1m1 svl ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} omega pthread glut)
target_include_directories(task2sample1m1 PRIVATE ${SVL_INCLUDE_DIR}
${JCUTIL_INCLUDE_DIR})
install(TARGETS task2sample1m1 RUNTIME DESTINATION bin)

add_executable(task2sample1m2 SplitTool/task2sample1m2.cpp)
target_link_libraries(task2sample1m2 svl ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} omega pthread glut)
target_include_directories(task2sample1m2 PRIVATE ${SVL_INCLUDE_DIR}
${JCUTIL_INCLUDE_DIR})
install(TARGETS task2sample1m2 RUNTIME DESTINATION bin)

add_executable(task2sample2m1 SplitTool/task2sample2m1.cpp)
target_link_libraries(task2sample2m1 svl ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} omega pthread glut)
target_include_directories(task2sample2m1 PRIVATE ${SVL_INCLUDE_DIR}
${JCUTIL_INCLUDE_DIR})
install(TARGETS task2sample2m1 RUNTIME DESTINATION bin)

add_executable(task2sample2m2 SplitTool/task2sample2m2.cpp)
target_link_libraries(task2sample2m2 svl ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} omega pthread glut)
target_include_directories(task2sample2m2 PRIVATE ${SVL_INCLUDE_DIR}
${JCUTIL_INCLUDE_DIR})
install(TARGETS task2sample2m2 RUNTIME DESTINATION bin)


add_executable(task3sample1m1 SplitTool/task3sample1m1.cpp)
target_link_libraries(task3sample1m1 svl ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} omega pthread glut)
target_include_directories(task3sample1m1 PRIVATE ${SVL_INCLUDE_DIR}
${JCUTIL_INCLUDE_DIR})
install(TARGETS task3sample1m1 RUNTIME DESTINATION bin)

add_executable(task3sample1m2 SplitTool/task3sample1m2.cpp)
target_link_libraries(task3sample1m2 svl ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} omega pthread glut)
target_include_directories(task3sample1m2 PRIVATE ${SVL_INCLUDE_DIR}
${JCUTIL_INCLUDE_DIR})
install(TARGETS task3sample1m2 RUNTIME DESTINATION bin)

add_executable(task3sample2m1 SplitTool/task3sample2m1.cpp)
target_link_libraries(task3sample2m1 svl ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} omega pthread glut)
target_include_directories(task3sample2m1 PRIVATE ${SVL_INCLUDE_DIR}
${JCUTIL_INCLUDE_DIR})
install(TARGETS task3sample2m1 RUNTIME DESTINATION bin)

add_executable(task3sample2m2 SplitTool/task3sample2m2.cpp)
target_link_libraries(task3sample2m2 svl ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} omega pthread glut)
target_include_directories(task3sample2m2 PRIVATE ${SVL_INCLUDE_DIR}
${JCUTIL_INCLUDE_DIR})
install(TARGETS task3sample2m2 RUNTIME DESTINATION bin)

add_executable(test11 SplitTool/test11.cpp)
target_link_libraries(test11 svl ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} omega pthread glut)
target_include_directories(test11 PRIVATE ${SVL_INCLUDE_DIR}
${JCUTIL_INCLUDE_DIR})
install(TARGETS test11 RUNTIME DESTINATION bin)

add_executable(test12 SplitTool/test12.cpp)
target_link_libraries(test12 svl ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} omega pthread glut)
target_include_directories(test12 PRIVATE ${SVL_INCLUDE_DIR}
${JCUTIL_INCLUDE_DIR})
install(TARGETS test12 RUNTIME DESTINATION bin)

#add_executable(splitToolkit_z10z SplitTool/splitToolkit_z10z.cpp)
#target_link_libraries(splitToolkit_z10z svl ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} omega pthread glut)
#target_include_directories(splitToolkit_z10z PRIVATE ${SVL_INCLUDE_DIR}
#${JCUTIL_INCLUDE_DIR})
#install(TARGETS splitToolkit_z10z RUNTIME DESTINATION bin)

#add_executable(ccStudy SplitTool/ccStudy.cpp)
#target_link_libraries(ccStudy svl ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
#target_include_directories(ccStudy PRIVATE ${SVL_INCLUDE_DIR}
#${JCUTIL_INCLUDE_DIR} ${PEELLING_COMMON_INCLUDE_DIR}
#${PEELING_NVMODEL_INCLUDE_DIR} ${PEELING_GLEW_INCLUDE_DIR})
#install(TARGETS ccStudy RUNTIME DESTINATION bin)

