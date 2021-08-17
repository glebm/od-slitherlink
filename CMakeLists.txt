cmake_minimum_required(VERSION 3.10) # CMP0083 NEW

project(Slitherlink
  VERSION 0.0.1
  LANGUAGES C CXX)

option(ASAN "Enable address sanitizer" ON)
option(UBSAN "Enable undefined behaviour sanitizer" ON)

if(CMAKE_BUILD_TYPE MATCHES "Release")

  # Enable link-time optimization if supported.
  include(CheckIPOSupported)
  check_ipo_supported(RESULT ipo_supported OUTPUT ipo_check_output)
  if(ipo_supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
  else()
    message(WARNING "LTO is not supported: ${ipo_check_output}\n")
  endif()

  set(ASAN OFF)
  set(UBSAN OFF)
endif()

set(Slitherlink_SRCS
  cell.cpp
  cursor.cpp
  draw.cpp
  help.cpp
  level.cpp
  main.cpp
  menuPause.cpp
  resourceManager.cpp
  resourceManager.cpp
  save.cpp
  sdlutils.cpp
  title.cpp
)

set(BIN_TARGET slitherlink)
add_executable(${BIN_TARGET} ${Slitherlink_SRCS})

find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_ttf REQUIRED)
find_package(SDL_mixer REQUIRED)

set_target_properties(${BIN_TARGET} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)

target_include_directories(${BIN_TARGET} PUBLIC
  ${SDL_INCLUDE_DIR}
  ${SDL_IMAGE_INCLUDE_DIRS}
  ${SDL_TTF_INCLUDE_DIRS}
  ${SDL_MIXER_INCLUDE_DIRS}
)
target_link_libraries(${BIN_TARGET} PRIVATE
  ${SDL_LIBRARY}
  ${SDL_IMAGE_LIBRARIES}
  ${SDL_TTF_LIBRARIES}
  ${SDL_MIXER_LIBRARIES}
)

set(TARGET_PLATFORM host CACHE STRING "Target platform")
set_property(CACHE TARGET_PLATFORM PROPERTY STRINGS host retrofw gcw0 rg350 miyoo)

if(TARGET_PLATFORM STREQUAL "retrofw")
  include(CMake/retrofw_defs.cmake)
elseif(TARGET_PLATFORM STREQUAL "gcw0")
  include(CMake/rg350_defs.cmake)
elseif(TARGET_PLATFORM STREQUAL "rg350")
  include(CMake/rg350_defs.cmake)
elseif(TARGET_PLATFORM STREQUAL "miyoo")
  include(CMake/miyoo_defs.cmake)
  target_link_libraries(${BIN_TARGET} PRIVATE
  mpg123
  mikmod
  vorbisfile
  vorbis
  ogg
  freetype
  bz2
  z
  png
  jpeg
  asound
)
endif()

foreach(
  def_name
  SCREEN_WIDTH
  SCREEN_HEIGHT
  RES_DIR
  LOCAL_STORAGE_DIR
  MYKEY_UP
  MYKEY_RIGHT
  MYKEY_DOWN
  MYKEY_LEFT
  MYKEY_LINEUP
  MYKEY_LINERIGHT
  MYKEY_LINEDOWN
  MYKEY_LINELEFT
  MYKEY_QUIT
  MYKEY_START
  MYKEY_MOD
  MYKEY_UNDO
)
  if(DEFINED ${def_name})
    list(APPEND def_list ${def_name}=${${def_name}})
  endif()
endforeach(def_name)
target_compile_definitions(${BIN_TARGET} PRIVATE ${def_list})

if(ASAN)
target_compile_options(${BIN_TARGET} PUBLIC -fsanitize=address -fsanitize-recover=address)
target_link_libraries(${BIN_TARGET} PUBLIC -fsanitize=address -fsanitize-recover=address)
endif()

if(UBSAN)
target_compile_options(${BIN_TARGET} PUBLIC -fsanitize=undefined)
target_link_libraries(${BIN_TARGET} PUBLIC -fsanitize=undefined)
endif()
