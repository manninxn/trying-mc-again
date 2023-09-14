include_directories(${minecraftc_SOURCE_DIR}/include)
# I assume you want to use LibProject as a library in MainProject.
# include_directories(${MyProject_SOURCE_DIR}/LibProject/inc)
# link_directories(${MyProject_SOURCE_DIR}/LibProject/src)

include_directories(${minecraftc_SOURCE_DIR}/lib/glfw/include)
link_directories(${minecraftc_SOURCE_DIR}/lib/glfw/src)

include_directories(${minecraftc_SOURCE_DIR}/lib/glad/include)
link_directories(${minecraftc_SOURCE_DIR}/lib/glad/src)

include_directories(${minecraftc_SOURCE_DIR}/lib/cglm/include)
link_directories(${minecraftc_SOURCE_DIR}/lib/cglm/src)

add_executable(minecraftc main.c "vbo.c" "vao.c" io.c "shader.c" "camera.c" "chunk.c" "texture.c" "blocks/grass.c" "block.c")

add_custom_command(TARGET minecraftc PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/res/ $<TARGET_FILE_DIR:minecraftc>/res/)
#target_link_libraries(MainProject LibProject)

target_link_libraries(minecraftc glfw)
target_link_libraries(minecraftc glad)
target_link_libraries(minecraftc cglm)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET minecraftc PROPERTY CXX_STANDARD 20)
endif()