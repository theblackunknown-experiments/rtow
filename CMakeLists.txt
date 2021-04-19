cmake_minimum_required(VERSION 3.15)

project(rtow
    VERSION 0.0.0
    DESCRIPTION "Ray Tracing in One Weekend"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

##############################
##         Options          ##
##############################

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

##############################
##        Includes          ##
##############################

include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)

##############################
##        Packages          ##
##############################

##############################
##         Targets          ##
##############################

add_executable(rtow)

target_sources(rtow
    PRIVATE
        main.cpp)

target_compile_definitions(rtow
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:
            NOMINMAX
            WIN32_LEAN_AND_MEAN
            VC_EXTRALEAN
        >)

##############################
##          Config          ##
##############################

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}-config.cmake.in" [=[
    @PACKAGE_INIT@
    include("@PACKAGE_PATH_EXPORT_TARGETS@")
]=])

set(PATH_EXPORT_TARGETS "${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME}-targets.cmake")
configure_package_config_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}-config.cmake"
    PATH_VARS
        PATH_EXPORT_TARGETS
    INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME}
)

write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}-config-version.cmake"
  COMPATIBILITY AnyNewerVersion
)

##############################
##         Install          ##
##############################

install(
    TARGETS
        rtow
    EXPORT ${CMAKE_PROJECT_NAME}-targets
    COMPONENT runtime
)

install(EXPORT ${CMAKE_PROJECT_NAME}-targets
    NAMESPACE ${CMAKE_PROJECT_NAME}::
    DESTINATION "${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME}"
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}-config-version.cmake"
    DESTINATION
        "${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME}"
)

##############################
##          Notes           ##
##############################

#  If defined, the following flags inhibit definition
#     of the indicated items.
#
#  NOGDICAPMASKS     - CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#  NOVIRTUALKEYCODES - VK_*
#  NOWINMESSAGES     - WM_*, EM_*, LB_*, CB_*
#  NOWINSTYLES       - WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#  NOSYSMETRICS      - SM_*
#  NOMENUS           - MF_*
#  NOICONS           - IDI_*
#  NOKEYSTATES       - MK_*
#  NOSYSCOMMANDS     - SC_*
#  NORASTEROPS       - Binary and Tertiary raster ops
#  NOSHOWWINDOW      - SW_*
#  OEMRESOURCE       - OEM Resource values
#  NOATOM            - Atom Manager routines
#  NOCLIPBOARD       - Clipboard routines
#  NOCOLOR           - Screen colors
#  NOCTLMGR          - Control and Dialog routines
#  NODRAWTEXT        - DrawText() and DT_*
#  NOGDI             - All GDI defines and routines
#  NOKERNEL          - All KERNEL defines and routines
#  NOUSER            - All USER defines and routines
#  NONLS             - All NLS defines and routines
#  NOMB              - MB_* and MessageBox()
#  NOMEMMGR          - GMEM_*, LMEM_*, GHND, LHND, associated routines
#  NOMETAFILE        - typedef METAFILEPICT
#  NOMINMAX          - Macros min(a,b) and max(a,b)
#  NOMSG             - typedef MSG and associated routines
#  NOOPENFILE        - OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#  NOSCROLL          - SB_* and scrolling routines
#  NOSERVICE         - All Service Controller routines, SERVICE_ equates, etc.
#  NOSOUND           - Sound driver routines
#  NOTEXTMETRIC      - typedef TEXTMETRIC and associated routines
#  NOWH              - SetWindowsHook and WH_*
#  NOWINOFFSETS      - GWL_*, GCL_*, associated routines
#  NOCOMM            - COMM driver routines
#  NOKANJI           - Kanji support stuff.
#  NOHELP            - Help engine interface.
#  NOPROFILER        - Profiler interface.
#  NODEFERWINDOWPOS  - DeferWindowPos routines
#  NOMCX             - Modem Configuration Extensions
