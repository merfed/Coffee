TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    wowfiles/WowChunkedFormat.cpp \
    wowfiles/Wdt.cpp \
    wowfiles/Wdl.cpp \
    wowfiles/Mphd.cpp \
    wowfiles/Mhdr.cpp \
    wowfiles/Mh2o.cpp \
    wowfiles/Mcnk.cpp \
    wowfiles/Mcin.cpp \
    wowfiles/Mcal.cpp \
    wowfiles/Chunk.cpp \
    utilities/Utilities.cpp \
    wowfiles/alpha/WdtAlpha.cpp \
    wowfiles/alpha/MphdAlpha.cpp \
    wowfiles/alpha/Monm.cpp \
    wowfiles/alpha/McvtAlpha.cpp \
    wowfiles/alpha/McnrAlpha.cpp \
    wowfiles/alpha/McnkAlpha.cpp \
    wowfiles/alpha/MainAlpha.cpp \
    wowfiles/alpha/AdtAlpha.cpp \
    wowfiles/cataclysm/McnrCata.cpp \
    wowfiles/cataclysm/McnkCataTextures.cpp \
    wowfiles/cataclysm/McnkCataObjects.cpp \
    wowfiles/cataclysm/McnkCata.cpp \
    wowfiles/cataclysm/AdtCataTextures.cpp \
    wowfiles/cataclysm/AdtCataObjects.cpp \
    wowfiles/cataclysm/AdtCata.cpp \
    wowfiles/lichking/McnrLk.cpp \
    wowfiles/lichking/McnkLk.cpp \
    wowfiles/lichking/AdtLk.cpp \
    wowfiles/MainChunk.cpp

HEADERS += \
    wowfiles/WowChunkedFormat.h \
    wowfiles/Wdt.h \
    wowfiles/Wdl.h \
    wowfiles/Mphd.h \
    wowfiles/Mhdr.h \
    wowfiles/Mh2o.h \
    wowfiles/Mcnk.h \
    wowfiles/Mcin.h \
    wowfiles/Mcal.h \
    wowfiles/ChunkHeaders.h \
    wowfiles/Chunk.h \
    utilities/Utilities.h \
    wowfiles/alpha/WdtAlpha.h \
    wowfiles/alpha/MphdAlpha.h \
    wowfiles/alpha/Monm.h \
    wowfiles/alpha/McvtAlpha.h \
    wowfiles/alpha/McnrAlpha.h \
    wowfiles/alpha/McnkAlpha.h \
    wowfiles/alpha/MainAlpha.h \
    wowfiles/alpha/AdtAlpha.h \
    wowfiles/cataclysm/McnrCata.h \
    wowfiles/cataclysm/McnkCataTextures.h \
    wowfiles/cataclysm/McnkCataObjects.h \
    wowfiles/cataclysm/McnkCata.h \
    wowfiles/cataclysm/AdtCataTextures.h \
    wowfiles/cataclysm/AdtCataObjects.h \
    wowfiles/cataclysm/AdtCata.h \
    wowfiles/lichking/McnrLk.h \
    wowfiles/lichking/McnkLk.h \
    wowfiles/lichking/AdtLk.h \
    wowfiles/MainChunk.h





