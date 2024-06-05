
# 共通ファイル
file(GLOB CMN_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/**_cmn.c*)
# _msw.c*
file(GLOB MSW_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/**_msw.c*)
# _unix.c*
file(GLOB UNIX_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/**_unix.c*)

# ソースファイル追加
target_sources( ${TARGET_LIB} PRIVATE ${CMN_SRC_FILES} )
if(WIN32)
	target_sources( ${TARGET_LIB} PRIVATE
		${MSW_SRC_FILES}
	)
else()
	target_sources( ${TARGET_LIB} PRIVATE
		${UNIX_SRC_FILES}
	)
endif()

# OS毎のサブディレクトリ
if(WIN32)
	add_subdirectory(msw)
else()
	add_subdirectory(unix)
endif()