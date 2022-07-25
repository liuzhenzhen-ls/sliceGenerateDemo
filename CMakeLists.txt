PROJECT(LvGenerateSliceCLI)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0 FATAL_ERROR)

MESSAGE(STATUS "CLI GenerateWrapper library")

include_directories(./CLI ./Core)

SET(WRAPPER_SOURCE  CLI/
    CLI/Wrapper.cpp
    CLI/AssemblyInfo.cpp
    CLI/Wrapper.h
    CLI/ManagedObject.h
    CLI/pch.h
    CLI/pch.cpp
    )

SET(CORE_HEADER 
   Core/Core.h
   Core/Vector.h
   Core/tgt_math.h
   Core/Volume.h
   Core/SliceHelper.h
   Core/SliceCache.h
   )

SET(CORE_SOURCE 
   Core/Volume.cpp
   Core/SliceHelper.cpp
   Core/SliceCache.cpp)

#include_directories(../boost)

#link_directories(../boost/lib)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /clr")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHa")

#Ugly - is there a better fix to this ....
if(CMAKE_CXX_FLAGS_DEBUG MATCHES "/RTC1")
   string(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
   string(REPLACE "/RTC1" " " CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
endif()  

ADD_LIBRARY(Core STATIC ${CORE_SOURCE} ${CORE_HEADER})

ADD_LIBRARY(LvGenerateSlice SHARED
${WRAPPER_SOURCE})

TARGET_LINK_LIBRARIES(LvGenerateSlice Core)

STRING (REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")   
