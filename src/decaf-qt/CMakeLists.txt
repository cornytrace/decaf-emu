project(decaf-qt)

include_directories(".")

file(GLOB_RECURSE SOURCE_FILES
	*.cpp 
	../decaf-sdl/config.cpp
	../decaf-sdl/decafsdl_graphics.cpp
	../decaf-sdl/decafsdl_opengl.cpp
	../decaf-sdl/decafsdl_vulkan.cpp)
file(GLOB_RECURSE HEADER_FILES *.h)
file(GLOB_RECURSE UI_FILES *.ui)

qt5_wrap_ui(UIS_HDRS ${UI_FILES})

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Widgets finds its own dependencies.
find_package(Qt5Widgets REQUIRED)

add_executable(decaf-qt
   ${SOURCE_FILES}
   ${HEADER_FILES}
   ${UIS_HDRS})

target_link_libraries(decaf-qt
    common
    libconfig
    libdecaf
	Qt5::WinMain
	Qt5::Widgets
    ${CPPTOML_LIBRARIES}
    ${EXCMD_LIBRARIES}
	${SDL2_LINK})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(decaf-sdl X11)
endif()

if(MSVC)
    set_target_properties(decaf-qt PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:WINDOWS")
	
	# Copy required QT files to install directory
	get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
	get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
	find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
	
	add_custom_command(TARGET decaf-qt POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E
        env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
            "$<TARGET_FILE:decaf-qt>"
    COMMENT "Running windeployqt...")
endif()

qt5_use_modules(decaf-qt Widgets)

install(TARGETS decaf-qt RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}")