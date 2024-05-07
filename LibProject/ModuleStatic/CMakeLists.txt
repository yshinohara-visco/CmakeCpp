
add_library(${LIB_STATIC} STATIC)

set_target_properties(${LIB_STATIC}
	PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON
)

target_sources(${LIB_STATIC}
	PRIVATE
		ModuleStatic.cpp
)


target_include_directories(${LIB_STATIC} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)