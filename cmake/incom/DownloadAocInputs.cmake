include_guard(GLOBAL)

function(_aoc_read_session_cookie out_cookie session_cookie_file)
	if(NOT EXISTS "${session_cookie_file}")
		message(FATAL_ERROR
			"AoC session cookie file not found: ${session_cookie_file}. "
			"Create this file (for example at .session_cookie) and put your session token in it."
		)
	endif()

	file(READ "${session_cookie_file}" _cookie_raw)
	string(STRIP "${_cookie_raw}" _cookie)

	if(_cookie MATCHES "^Cookie:[ ]*session=(.+)$")
		set(_cookie "${CMAKE_MATCH_1}")
	elseif(_cookie MATCHES "^session=(.+)$")
		set(_cookie "${CMAKE_MATCH_1}")
	endif()

	string(STRIP "${_cookie}" _cookie)

	if(_cookie STREQUAL "")
		message(FATAL_ERROR
			"AoC session cookie file is empty: ${session_cookie_file}"
		)
	endif()

	set(${out_cookie} "${_cookie}" PARENT_SCOPE)
endfunction()

function(aoc_download_available_inputs)
	set(options)
	set(oneValueArgs PROBLEMS_ROOT OUTPUT_ROOT SESSION_COOKIE_FILE STRICT)
	set(multiValueArgs)
	cmake_parse_arguments(AOC_DL
		"${options}"
		"${oneValueArgs}"
		"${multiValueArgs}"
		${ARGN}
	)

	if(NOT AOC_DL_PROBLEMS_ROOT)
		set(AOC_DL_PROBLEMS_ROOT "${CMAKE_SOURCE_DIR}/src/problems")
	endif()

	if(NOT AOC_DL_OUTPUT_ROOT)
		set(AOC_DL_OUTPUT_ROOT "${CMAKE_SOURCE_DIR}/input")
	endif()

	if(NOT AOC_DL_SESSION_COOKIE_FILE)
		set(AOC_DL_SESSION_COOKIE_FILE "${CMAKE_SOURCE_DIR}/.session_cookie")
	endif()

	if(NOT DEFINED AOC_DL_STRICT)
		set(AOC_DL_STRICT OFF)
	endif()

	if(AOC_DL_STRICT)
		set(_severity FATAL_ERROR)
	else()
		set(_severity WARNING)
	endif()

	file(GLOB_RECURSE _problem_sources
		CONFIGURE_DEPENDS
		"${AOC_DL_PROBLEMS_ROOT}/AOC_*/AOC_*_day_*.cpp"
	)

	if(NOT _problem_sources)
		message(WARNING
			"AoC input download: no problem source files found under ${AOC_DL_PROBLEMS_ROOT}"
		)
		message(WARNING
			"AoC input download: this is unusual and probably an error in how the project is setup"
		)
		return()
	endif()

	set(_download_enabled OFF)
	if(EXISTS "${AOC_DL_SESSION_COOKIE_FILE}")
		_aoc_read_session_cookie(_aoc_session_cookie "${AOC_DL_SESSION_COOKIE_FILE}")
		set(_download_enabled ON)
	else()
		message(WARNING
			"AoC session cookie file not found: ${AOC_DL_SESSION_COOKIE_FILE}. "
			"Download is disabled; expected inputs will be checked and reported if missing."
		)
	endif()

	set(_downloaded_count 0)
	set(_skipped_count 0)
	set(_failed_count 0)
	set(_missing_count 0)

	foreach(_problem_file IN LISTS _problem_sources)
		get_filename_component(_problem_name "${_problem_file}" NAME)

		if(NOT
			_problem_name
			MATCHES
			"^AOC_([0-9][0-9][0-9][0-9])_day_([0-9]|[0-9][0-9])\\.cpp$"
		)
			continue()
		endif()

		set(_year "${CMAKE_MATCH_1}")
		set(_day "${CMAKE_MATCH_2}")

		set(_year_dir "${AOC_DL_OUTPUT_ROOT}/AOC_${_year}")
		file(MAKE_DIRECTORY "${_year_dir}")

		set(_output_file "${_year_dir}/${_year}_${_day}_1.txt")
		if(EXISTS "${_output_file}")
			math(EXPR _skipped_count "${_skipped_count} + 1")
			continue()
		endif()

		if(NOT _download_enabled)
			math(EXPR _missing_count "${_missing_count} + 1")
			message(WARNING
				"AoC input missing and cannot be downloaded (no session cookie): ${_output_file}"
			)
			continue()
		endif()

		set(_url "https://adventofcode.com/${_year}/day/${_day}/input")

		file(DOWNLOAD "${_url}"
			"${_output_file}"
			HTTPHEADER "Cookie: session=${_aoc_session_cookie}"
			STATUS _status
			LOG _log
			SHOW_PROGRESS
		)

		list(GET _status 0 _status_code)
		list(GET _status 1 _status_message)

		if(NOT _status_code EQUAL 0)
			file(REMOVE "${_output_file}")
			math(EXPR _failed_count "${_failed_count} + 1")

			if(_log)
				string(REPLACE "\n" " " _log_single_line "${_log}")
			else()
				set(_log_single_line "<no downloader log>")
			endif()

			message(${_severity}
				"AoC input download failed for ${_year} day ${_day}: ${_status_message}. "
				"URL: ${_url}. "
				"Downloader log: ${_log_single_line}"
			)
			continue()
		endif()

		math(EXPR _downloaded_count "${_downloaded_count} + 1")
		message(STATUS "AoC input downloaded: ${_output_file}")
	endforeach()

	message(STATUS
		"AoC input download summary - downloaded: ${_downloaded_count}, "
		"skipped(existing): ${_skipped_count}, missing(no-cookie): ${_missing_count}, failed: ${_failed_count}"
	)
endfunction()
