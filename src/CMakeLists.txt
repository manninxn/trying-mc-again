include_directories(${GLProject_SOURCE_DIR}/include)
# I assume you want to use LibProject as a library in MainProject.
# include_directories(${MyProject_SOURCE_DIR}/LibProject/inc)
# link_directories(${MyProject_SOURCE_DIR}/LibProject/src)

include_directories(${GLProject_SOURCE_DIR}/lib/glfw/include)
link_directories(${GLProject_SOURCE_DIR}/lib/glfw/src)

include_directories(${GLProject_SOURCE_DIR}/lib/glad/include)
link_directories(${GLProject_SOURCE_DIR}/lib/glad/src)

include_directories(${GLProject_SOURCE_DIR}/lib/cglm/include)
link_directories(${GLProject_SOURCE_DIR}/lib/cglm/src)

add_executable(GLProject main.c "vbo.c")
#target_link_libraries(MainProject LibProject)

target_link_libraries(GLProject glfw)
target_link_libraries(GLProject glad)
target_link_libraries(GLProject cglm)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET GLProject PROPERTY CXX_STANDARD 20)
endif()