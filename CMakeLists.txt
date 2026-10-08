cmake_minimum_required(VERSION 3.16)

project(EC_Master_Lite VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (QT_AUTO_SCREEN_SCALE_FACTOR 1)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools PrintSupport)

# Configurable sizes
set(EC_BUFSIZE EC_MAXECATFRAME CACHE STRING "standard frame buffer size in bytes")
set(EC_MAXBUF 16 CACHE STRING "number of frame buffers per channel (tx, rx1 rx2)")
set(EC_MAXEEPBITMAP 128 CACHE STRING "size of EEPROM bitmap cache")
set(EC_MAXEEPBUF "EC_MAXEEPBITMAP << 5" CACHE STRING "size of EEPROM cache buffer")
set(EC_LOGGROUPOFFSET 16 CACHE STRING "default group size in 2^x")
set(EC_MAXELIST 64 CACHE STRING "max. entries in EtherCAT error list")
set(EC_MAXNAME 40 CACHE STRING "max. length of readable name in slavelist and Object Description List")
set(EC_MAXSLAVE 200 CACHE STRING "max. number of slaves in array")
set(EC_MAXGROUP 2 CACHE STRING "max. number of groups")
set(EC_MAXIOSEGMENTS 64 CACHE STRING "max. number of IO segments per group")
set(EC_MAXMBX 1486 CACHE STRING "max. mailbox size")
set(EC_MBXPOOLSIZE 32 CACHE STRING "number of mailboxes in pool")
set(EC_MAXEEPDO 0x200 CACHE STRING "max. eeprom PDO entries")
set(EC_MAXSM 8 CACHE STRING "max. SM used")
set(EC_MAXFMMU 4 CACHE STRING "max. FMMU used")
set(EC_MAXLEN_ADAPTERNAME 128 CACHE STRING "max. adapter name length")
set(EC_MAX_MAPT 1 CACHE STRING " define maximum number of concurrent threads in mapping")
set(EC_MAXODLIST 1024 CACHE STRING "max entries in Object Description list")
set(EC_MAXOELIST 256 CACHE STRING "max entries in Object Entry list")
set(EC_SOE_MAXNAME 60 CACHE STRING "max. length of readable SoE name")
set(EC_SOE_MAXMAPPING 64 CACHE STRING "max. number of SoE mappings")

# Configurable timeouts and retries
set(EC_TIMEOUTRET 2000 CACHE STRING "timeout value in us for tx frame to return to rx")
set(EC_TIMEOUTRET3 "EC_TIMEOUTRET * 3" CACHE STRING "timeout value in us for safe data transfer, max. triple retry")
set(EC_TIMEOUTSAFE 20000 CACHE STRING "timeout value in us for return \"safe\" variant (f.e. wireless)")
set(EC_TIMEOUTEEP 20000 CACHE STRING "timeout value in us for EEPROM access")
set(EC_TIMEOUTTXM 20000 CACHE STRING "timeout value in us for tx mailbox cycle")
set(EC_TIMEOUTRXM 700000 CACHE STRING "timeout value in us for rx mailbox cycle")
set(EC_TIMEOUTSTATE 2000000 CACHE STRING "timeout value in us for check statechange")
set(EC_DEFAULTRETRIES 3 CACHE STRING "default number of retries if wkc <= 0")

# MAC addresses
set(EC_PRIMARY_MAC_ARRAY "{0x0101, 0x0101, 0x0101}")
set(EC_SECONDARY_MAC_ARRAY "{0x0404, 0x0404, 0x0404}")

configure_file(${CMAKE_SOURCE_DIR}/soem/ec_options.h.in
               ${CMAKE_SOURCE_DIR}/soem/ec_options.h)

file(GLOB SOEM_HEADERS
   ${CMAKE_SOURCE_DIR}/soem/*.h
   ${CMAKE_SOURCE_DIR}/oshw/*.h
   ${CMAKE_SOURCE_DIR}/oshw/wpcap/Include/*.h
   ${CMAKE_SOURCE_DIR}/oshw/wpcap/Include/pcap/*.h
   ${CMAKE_SOURCE_DIR}/osal/*.h
   ${CMAKE_SOURCE_DIR}/inc/*.h
   ${CMAKE_SOURCE_DIR}/lib/*.h
)
file(GLOB SOEM_SOURCES
    ${CMAKE_SOURCE_DIR}/soem/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/osal/*.cpp
    ${CMAKE_SOURCE_DIR}/oshw/*.cpp
    ${CMAKE_SOURCE_DIR}/lib/*.cpp
)

file(GLOB FORM_HEADERS
    ${CMAKE_SOURCE_DIR}/form/*.h
)

file(GLOB FORM_SOURCES
    ${CMAKE_SOURCE_DIR}/form/*.cpp
)

file(GLOB FORM_UI
    ${CMAKE_SOURCE_DIR}/form/*.ui
)
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${SOEM_HEADERS}
        ${SOEM_SOURCES}
        ${FORM_HEADERS}
        ${FORM_SOURCES}
        ${FORM_UI}
)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/inc)
include_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${CMAKE_SOURCE_DIR}/soem)
include_directories(${CMAKE_SOURCE_DIR}/oshw)
include_directories(${CMAKE_SOURCE_DIR}/oshw/wpcap/Include)
include_directories(${CMAKE_SOURCE_DIR}/oshw/wpcap/Include/pcap)
include_directories(${CMAKE_SOURCE_DIR}/osal)
include_directories(${CMAKE_SOURCE_DIR}/form/)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(EC_Master_Lite
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        resource_icon.qrc
        inc/ec_control.h
        src/ec_control.cpp


        inc/ec_info.h
        src/ec_info.cpp
        inc/Global_Variable.h
        src/Global_Variable.cpp
        inc/data_struct.h
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET EC_Master_Lite APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(EC_Master_Lite SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(EC_Master_Lite
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(EC_Master_Lite PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::PrintSupport)

link_directories("C:/Program Files/Npcap/Lib/x64")

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    "C:/Users/Thanh Le/Desktop/EtherCat_Master_Window/EC_Master/oshw/wpcap/Lib/x64/wpcap.lib"
    "C:/Users/Thanh Le/Desktop/EtherCat_Master_Window/EC_Master/oshw/wpcap/Lib/x64/Packet.lib"
    Ws2_32
    winmm
)
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.EC_Master_Lite)
endif()
set_target_properties(EC_Master_Lite PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS EC_Master_Lite
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(EC_Master_Lite)
endif()
