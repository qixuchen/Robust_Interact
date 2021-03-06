cmake_minimum_required(VERSION 3.15)
project(RI_user_study)

set(CMAKE_CXX_STANDARD 14)
set(INC_DIR /usr/local/Cellar/glpk/4.65/include)
set(LINK_DIR /usr/local/Cellar/glpk/4.65/lib)
enable_testing()

set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true)

if(COMMAND cmake_policy)
       cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

#if(NOT DEFINED BOOST)
#  message(FATAL_ERROR "This program requires the boost library, and will not be compiled.")
#else()
#  message(STATUS "BOOST is DEFINED")
#endif()

find_library(LP_SOLVE NAMES liblpsolve55.so PATHS /usr/lib/lp_solve)

if (NOT LP_SOLVE)

  message(FATAL_ERROR "This program requires the lp_solve library, and will not be compiled.")

else ()

  message(STATUS "Library lp_solve found: ${LP_SOLVE}")

  set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")


  set(CMAKE_CXX_STANDARD 14)
  set(INC_DIR /usr/local/Cellar/glpk/4.65/include)
  set(LINK_DIR /usr/local/Cellar/glpk/4.65/lib)




  include_directories(.)
  include_directories(2DPI)
  include_directories(Others)
  include_directories(Qhull)

  include_directories(${INC_DIR})
  link_directories(${LINK_DIR})
  link_libraries(glpk)


  include_directories (BEFORE ./external/Eigen)
  include_directories (BEFORE ./external)
  include_directories (BEFORE ./external/minimum_ellipsoid)
  #include_directories (BEFORE ../include/cartesian_geom)
  #include_directories (BEFORE ../include/convex_bodies)
  include_directories (BEFORE ./external/LPsolve_src/run_headers)
  include_directories (BEFORE ./external/boost)
  #include_directories (BEFORE BOOST)
  include_directories (BEFORE ./include/generators)
  include_directories (BEFORE ./include/volume)
  include_directories (BEFORE ./include)
  include_directories (BEFORE ./include/convex_bodies)
  include_directories (BEFORE ./include/annealing)
  include_directories (BEFORE ./include/samplers)
  include_directories (BEFORE ./include/lp_oracles)
  include_directories (BEFORE ./include/misc)

  #for Eigen
  if (${CMAKE_VERSION} VERSION_LESS "3.12.0") 
    add_compile_options(-D "EIGEN_NO_DEBUG")
  else ()
    add_compile_definitions("EIGEN_NO_DEBUG")
  endif ()


  add_definitions(${CMAKE_CXX_FLAGS} "-std=c++11")  # enable C++11 standard
  add_definitions(${CMAKE_CXX_FLAGS} "-g")  # enable debuger
  #add_definitions(${CMAKE_CXX_FLAGS} "-Wint-in-bool-context")
  #add_definitions(${CMAKE_CXX_FLAGS} "-Wall")
 
  #add_definitions(${CMAKE_CXX_FLAGS} "-O3")  # optimization of the compiler
  #add_definitions(${CXX_COVERAGE_COMPILE_FLAGS} "-lgsl")
  add_definitions(${CXX_COVERAGE_COMPILE_FLAGS} "-lm")
  add_definitions(${CXX_COVERAGE_COMPILE_FLAGS} "-ldl")
  add_definitions(${CXX_COVERAGE_COMPILE_FLAGS} "-DBOOST_NO_AUTO_PTR")
  #add_definitions(${CXX_COVERAGE_COMPILE_FLAGS} "-lgslcblas")
  #add_definitions( "-O3 -lgsl -lm -ldl -lgslcblas" )


  add_definitions(${CMAKE_CXX_FLAGS} "-g")

add_executable(User_study
        RI/PointPrune.cpp
        RI/PointPrune.h
        RI/SamplePrune.cpp
        RI/SamplePrune.h
        RI/STMD.cpp
        RI/STMD.h
        RI/partition_func.cpp
        RI/partition_func.h
        active_ranking.cpp
        active_ranking.h
        RH.cpp
        RH.h
        HDPI.cpp
        HDPI.h
        Others/Array.cc
        Others/Array.hh
        Others/data_struct.h
        Others/data_utility.cpp
        Others/data_utility.h
        Others/operation.cpp
        Others/operation.h
        Others/pruning.cpp
        Others/pruning.h
        Others/qhull_build.cpp
        Others/qhull_build.h
        Others/read_write.cpp
        Others/read_write.h
        Others/rtree.cpp
        Others/rtree.h
        Others/used_function.cpp
        Others/used_function.h
        Others/user_mode.cpp
        Others/user_mode.h
        Others/user_study_func.cpp
        Others/user_study_func.h
        Qhull/frame.cpp
        Qhull/frame.h
        Qhull/geom.cpp
        Qhull/geom.h
        Qhull/geom2.cpp
        Qhull/global.cpp
        Qhull/io.cpp
        Qhull/io.h
        Qhull/libqhull.cpp
        Qhull/libqhull.h
        lp.cpp
        lp.h
        RI_user_study.cpp
        RI_user_study.h
        maxUtility.cpp
        maxUtility.h
        Qhull/mem.cpp
        Qhull/mem.h
        Qhull/merge.cpp
        Qhull/merge.h
        Qhull/poly.cpp
        Qhull/poly.h
        Qhull/poly2.cpp
        preferLearn.cpp
        preferLearn.h
        Qhull/qhull_a.h
        Qhull/qset.cpp
        Qhull/qset.h
        Others/QuadProg++.cc
        Others/QuadProg++.hh
        Qhull/random.cpp
        Qhull/random.h
        Qhull/rboxlib.cpp
        Qhull/stat.cpp
        Qhull/stat.h
        Qhull/user.cpp
        Qhull/user.h
        Qhull/usermem.cpp
        Qhull/userprintf.cpp
        Qhull/userprintf_rbox.cpp
        main.cpp)

target_link_libraries(User_study glpk)

TARGET_LINK_LIBRARIES(User_study ${LP_SOLVE})

endif ()