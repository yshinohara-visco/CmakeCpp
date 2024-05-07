
#DLLにエクスポート
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(${LIB_SHARED} SHARED)

set_target_properties(${LIB_SHARED}
	PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON
)

target_sources(${LIB_SHARED}
	PRIVATE
		ModuleShared.cpp
)

target_link_libraries(${LIB_SHARED}
	PUBLIC
		${LIB_STATIC}
)

target_include_directories(${LIB_SHARED}
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
)