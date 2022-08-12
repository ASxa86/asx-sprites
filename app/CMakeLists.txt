project(asx-sprites)

add_executable(${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME} PRIVATE -DFONT_FILE="${CMAKE_SOURCE_DIR}/data/roboto/Roboto-Regular.ttf")
target_compile_definitions(${PROJECT_NAME} PRIVATE -DSPRITE_FILE="${CMAKE_SOURCE_DIR}/data/test.png")

target_sources(${PROJECT_NAME} PRIVATE
	main.cpp
)

find_package(SFML CONFIG REQUIRED COMPONENTS graphics window)

target_link_libraries(${PROJECT_NAME} PRIVATE
	sfml-graphics
	sfml-window
)

project_compile_options()
project_install_target()
