add_executable(p5 "main.cpp;Object.cpp;Block.cpp;Plataform.cpp;Scenery.cpp;Sheep.cpp")
target_link_libraries(p5
LINK_PUBLIC
glfw
glad
glm
${CMAKE_DL_LIBS}
)