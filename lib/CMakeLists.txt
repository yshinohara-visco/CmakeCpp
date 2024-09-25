#“®“Iƒ‰ƒCƒuƒ‰ƒŠ‚ğì¬‚·‚é

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(Hello SHARED)

set_target_properties(Hello PROPERTIES
	VERSION {VERSION}
)

target_sources(Hello PRIVATE 
	"hello.cpp"
)

target_include_directories(Hello PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
)

install (TARGETS Hello
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)