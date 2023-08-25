cmake_minimum_required(VERSION 3.27)

include(CMakePrintHelpers)

function(GroupSourcesByFolder TARGET)
	set(SOURCE_GROUP_DELIMITER "/")
	set(LAST_DIR "")
	set(FILES "")

	get_target_property(SOURCES ${TARGET} SOURCES)

	foreach(FILE ${SOURCES})
		file(RELATIVE_PATH RELATIVE_FILE "${CMAKE_CURRENT_SOURCE_DIR}" ${FILE})
		get_filename_component(DIR "${RELATIVE_FILE}" PATH)

		if(NOT "${DIR}" STREQUAL "${LAST_DIR}")
			if(FILES)
				source_group("${LAST_DIR}" FILES ${FILES})
			endif()
			set(FILES "")
		endif()

		set(FILES ${FILES} ${FILE})
		set(LAST_DIR "${DIR}")
	endforeach()

	if(FILES)
		source_group("${LAST_DIR}" FILES ${FILES})
	endif()
endfunction()