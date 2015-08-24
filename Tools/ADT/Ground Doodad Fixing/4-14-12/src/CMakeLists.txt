cmake_minimum_required (VERSION 2.6)
project (gillijimproject)

set ( gp_sources
  utilities/Utilities.cpp

  wowfiles/alpha/AdtAlpha.cpp
  wowfiles/alpha/WdtAlpha.cpp
  wowfiles/alpha/McnkAlpha.cpp  
  wowfiles/alpha/McnrAlpha.cpp  
  wowfiles/alpha/McvtAlpha.cpp  
  wowfiles/alpha/Monm.cpp  
  wowfiles/alpha/MphdAlpha.cpp  
  wowfiles/alpha/MainAlpha.cpp  
  
  wowfiles/cataclysm/AdtCata.cpp  
  wowfiles/cataclysm/AdtCataTextures.cpp  
  wowfiles/cataclysm/AdtCataObjects.cpp  
  wowfiles/cataclysm/McnkCata.cpp  
  wowfiles/cataclysm/McnkCataObjects.cpp  
  wowfiles/cataclysm/McnkCataTextures.cpp
  wowfiles/cataclysm/McnrCata.cpp  
  
  wowfiles/lichking/AdtLk.cpp
  wowfiles/lichking/McnkLk.cpp 
  wowfiles/lichking/McnrLk.cpp
  
  wowfiles/Chunk.cpp  
  wowfiles/MainChunk.cpp  
  wowfiles/Mcal.cpp  
  wowfiles/Mcin.cpp 
  wowfiles/Mcnk.cpp 
  wowfiles/Mh2o.cpp
  wowfiles/Mhdr.cpp  
  wowfiles/Mphd.cpp  
  wowfiles/Wdl.cpp  
  wowfiles/Wdt.cpp  
  wowfiles/WowChunkedFormat.cpp  
  
  main.cpp
  )

set ( gp_headers
  utilities/Utilities.h
  
  wowfiles/alpha/AdtAlpha.h
  wowfiles/alpha/WdtAlpha.h
  wowfiles/alpha/McnkAlpha.h
  wowfiles/alpha/McnrAlpha.h
  wowfiles/alpha/McvtAlpha.h
  wowfiles/alpha/Monm.h
  wowfiles/alpha/MphdAlpha.h
  wowfiles/alpha/MainAlpha.h

  wowfiles/cataclysm/AdtCata.h
  wowfiles/cataclysm/AdtCataTextures.h  
  wowfiles/cataclysm/AdtCataObjects.h    
  wowfiles/cataclysm/McnkCata.h
  wowfiles/cataclysm/McnkCataObjects.h
  wowfiles/cataclysm/McnkCataTextures.h
  wowfiles/cataclysm/McnrCata.h

  wowfiles/lichking/AdtLk.h
  wowfiles/lichking/McnkLk.h
  wowfiles/lichking/McnrLk.h

  wowfiles/Chunk.h
  wowfiles/ChunkHeaders.h  
  wowfiles/MainChunk.h
  wowfiles/Mcal.h
  wowfiles/Mcin.h
  wowfiles/Mcnk.h
  wowfiles/Mh2o.h
  wowfiles/Mhdr.h
  wowfiles/Mphd.h
  wowfiles/Wdl.h
  wowfiles/Wdt.h
  wowfiles/WowChunkedFormat.h
  )

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

add_executable( gillijimproject
  ${gp_headers}
  ${gp_sources}  
  )