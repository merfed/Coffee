cmake_minimum_required (VERSION 2.6)
project (gruulme)

find_package (Qt4 4.8 COMPONENTS QtCore QtGui QtOpenGL REQUIRED)

include (${QT_USE_FILE})

qt4_wrap_cpp (moced ${headers_to_moc} ${headers_to_moc})
qt4_automoc (${moced})

set ( gruulme_cpp
  Utilities.cpp

  Chunk.cpp
  
  adtarray.cpp
  adtarrayscene.cpp
  
  Wdt.cpp
  
  main.cpp
  )

set ( gruulme_h
  Utilities.h

  Chunk.h
  
  adtarray.h
  adtarrayscene.h
  
  Wdt.h
  )

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

add_executable( gruulme
  ${gruulme_h}
  ${gruulme_cpp}  
  )