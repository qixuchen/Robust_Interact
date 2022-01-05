cmake_minimum_required(VERSION 3.15)
project(2D)

set(CMAKE_CXX_STANDARD 14)
set(INC_DIR /usr/local/Cellar/glpk/4.65/include)
set(LINK_DIR /usr/local/Cellar/glpk/4.65/lib)

include_directories(.)
include_directories(Others)
include_directories(Qhull)

include_directories(${INC_DIR})
link_directories(${LINK_DIR})
link_libraries(glpk)

add_definitions(${CMAKE_CXX_FLAGS} "-g")

add_executable(2D
        ST2D/ST2D.h
        ST2D/ST2D.cpp
        Median_Hull/medianhull.cpp
        Median_Hull/medianhull.h
        Others/user_mode.cpp
        Others/user_mode.h
        Others/data_struct.h
        Others/data_utility.cpp
        Others/data_utility.h
        Others/frame.cpp
        Others/frame.h
        Others/operation.cpp
        Others/operation.h
        Others/pruning.cpp
        Others/pruning.h
        Others/qhull_build.cpp
        Others/qhull_build.h
        Others/read_write.cpp
        Others/read_write.h
        Others/used_function.cpp
        Others/used_function.h
        Qhull/geom.cpp
        Qhull/geom.h
        Qhull/geom2.cpp
        Qhull/global.cpp
        Qhull/io.cpp
        Qhull/io.h
        Qhull/libqhull.cpp
        Qhull/libqhull.h
        Qhull/mem.cpp
        Qhull/mem.h
        Qhull/merge.cpp
        Qhull/merge.h
        Qhull/poly.cpp
        Qhull/poly.h
        Qhull/poly2.cpp
        Qhull/qhull_a.h
        Qhull/qset.cpp
        Qhull/qset.h
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
        lp.cpp
        lp.h
        main.cpp)

target_link_libraries(2D glpk)