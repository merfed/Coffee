#inspired from http://www.cmake.org/pipermail/cmake/2009-July/030788.html
#arguments checking is useless since a macro in test.cmake is used.

#convert the space-separated string to a list
separate_arguments(args)

execute_process(
	COMMAND ${cmd} ${args}
	OUTPUT_FILE ${result})
execute_process(
	COMMAND ${CMAKE_COMMAND} -E compare_files ${expected} ${result}
	RESULT_VARIABLE nope)

if (nope)
	message(SEND_ERROR "${result} does not match ${expected}!")
endif (nope)
