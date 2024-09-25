#“®“Iƒ‰ƒCƒuƒ‰ƒŠ‚ğì¬‚·‚é

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(CmakeDll SHARED)

set_target_properties(CmakeDll PROPERTIES
	VERSION {PROJECT_VERSION}
)

target_sources(CmakeDll PRIVATE 
	"hello.cpp"
	${CMAKE_BINARY_DIR}/version.rc
)

target_include_directories(CmakeDll PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
)

install (TARGETS CmakeDll
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)