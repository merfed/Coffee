#inspired from http://www.cmake.org/pipermail/cmake/2009-July/030788.html

macro(expect test cmd args)
	add_test(${test}
		${CMAKE_COMMAND}
		-Dcmd=${cmd}
		-Dargs:string=${args}
		-Dresult=tests/${test}.res
		-Dexpected=tests/${test}.exp
		-P tests/do_test.cmake)
endmacro(expect)

enable_testing()
file(GLOB UnitTests tests/U*.c)
execute_process(
	COMMAND "tests/make-tests.sh" ${UnitTests}
	OUTPUT_FILE "tests/UAll.c")
add_executable(UAll ${UnitTests} tests/CuTest.c src/conversion.c src/fallback.c src/reading.c src/common.c src/fcaseopen.c)
add_test(UAll UAll)

if (UNIX)
	#Blackbox
	expect(dummy tests/hash.sh dummy)
endif (UNIX)
