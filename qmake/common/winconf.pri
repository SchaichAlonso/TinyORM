# WinApi
# ---
# For orientation in these versions, see:
# https://developer.microsoft.com/en-us/windows/downloads/sdk-archive/
# https://microsoft.fandom.com/wiki/List_of_Windows_codenames
# https://en.wikipedia.org/wiki/Windows_11_version_history
# https://en.wikipedia.org/wiki/Microsoft_Windows_SDK

# The ideal case would be not to define these and rely on what is defined
# in <qt_windows.h> but Qt uses too old values for these, eg. MSYS2 patches these and
# uses the latest versions, so we have to define these manually because the original
# Qt code doesn't maintain these correctly.
# All have to be defined because of checks at the beginning of <qt_windows.h> (fixed)
# Windows 11 "22H2" - 0x0A00000C
DEFINES *= WINVER=_WIN32_WINNT_WIN10
DEFINES *= NTDDI_VERSION=NTDDI_WIN10_NI
DEFINES *= _WIN32_WINNT=_WIN32_WINNT_WIN10
# Internet Explorer 11
DEFINES *= _WIN32_IE=_WIN32_IE_IE110
# Exclude unneeded header files
DEFINES *= WIN32_LEAN_AND_MEAN
DEFINES *= NOMINMAX

# Needed to suppress this for <=Qt5 because a lot of new warning on Qt v5, I will not
# invest more time to this to correctly fix it because Qt5 support will be removed soon
if(win32-msvc|win32-clang-msvc): \
versionAtMost(QT_MAJOR_VERSION, 5): \
    DEFINES *= _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING

# Latest qmake's msvc fixes
# ---
win32-msvc {
    greaterThan(QMAKE_MSC_VER, 1909) {
        QMAKE_CXXFLAGS     *= -permissive-
        QMAKE_CXXFLAGS     -= -Zc:referenceBinding
    }

    greaterThan(QMAKE_MSC_VER, 1919): \
        QMAKE_CXXFLAGS     *= -Zc:externConstexpr
}

win32-msvc|win32-clang-msvc {
    greaterThan(QMAKE_MSC_VER, 1927) {
        # Visual Studio 2019 (16.8 or 16.9) / Visual C++ 19.28 and up
        MSVC_VER            = 16.8
        QMAKE_CFLAGS_C11    = /std:c11
        QMAKE_CFLAGS_C17    = /std:c17
    }

    greaterThan(QMAKE_MSC_VER, 1928) {
        # Visual Studio 2019 (16.10 or 16.11) / Visual C++ 19.29 and up
        MSVC_VER            = 16.10

        # -std:c++20 compiler option for Visual Studio 2019 16.11.0 and up
        greaterThan(QMAKE_MSC_FULL_VER, 192930132): \
            QMAKE_CXXFLAGS_CXX2A = -std:c++20
    }

    greaterThan(QMAKE_MSC_VER, 1929): \
        # Visual Studio 2022 (17.0) / Visual C++ 19.30 and up
        MSVC_VER            = 17.0

    greaterThan(QMAKE_MSC_VER, 1938) {
        # Visual Studio 2022 (17.9) / Visual C++ 19.39 and up
        MSVC_VER            = 17.9
        MSVC_TOOLSET_VER    = 143
    }

    greaterThan(QMAKE_MSC_VER, 1939) {
        # Visual Studio 2022 (17.10) / Visual C++ 19.40 and up
        MSVC_VER            = 17.10
        MSVC_TOOLSET_VER    = 144
    }
}

# Compiler and Linker options
# ---

win32-msvc {
    # C/C++ conformant preprocessor
    QMAKE_CXXFLAGS += -guard:cf -bigobj -Zc:preprocessor
    QMAKE_CXXFLAGS_WARN_ON = -external:anglebrackets -external:W0 -W4 -wd4702
                             # Enable and check it from time to time
#                             -external:templates-
}

# clang-cl.exe notes:
# /RTC    - https://lists.llvm.org/pipermail/cfe-commits/Week-of-Mon-20130902/088105.html
# /bigobj - clang-cl uses it by default - https://reviews.llvm.org/D12981
win32-clang-msvc {
    QMAKE_CXXFLAGS_WARN_ON = -W4

    # Relative paths in -Yu or -Fp are throwing -Wmicrosoft-include warning on clang-cl
    # with msvc, but not all, only if it contains more ../../, eg. 5 levels up, this is
    # happening especially for auto tests which have deep folder structure.
    precompile_header: \
        QMAKE_CXXFLAGS_WARN_ON += -Wno-microsoft-include

    # Always use colors in diagnostics
    equals(QMAKE_LINK, lld-link): \
        QMAKE_LFLAGS *= --color-diagnostics
}

win32-msvc|win32-clang-msvc {
    QMAKE_CXXFLAGS_DEBUG += -RTC1 -sdl
    # Abort compiling on warnings for Debug builds only, Release builds must go on
    # as far as possible
    CONFIG(debug, debug|release): \
        QMAKE_CXXFLAGS_WARN_ON *= -WX

    # I don't use -MP flag because using the jom
    QMAKE_LFLAGS += /guard:cf
    # Abort linking on warnings for Debug builds only, Release builds must go on as far as possible
    QMAKE_LFLAGS_DEBUG += /WX
    # Looks like clang-cl does know nothing about these, for now enabling
    QMAKE_LFLAGS_RELEASE += /OPT:REF,ICF=5
}

# -mthreads is unused on Clang and also g++ and -fexceptions is enabled by default,
# these are old obsolete/deprecated flags for of mingw32, also CMake doesn't use these.
gcc|clang {
    QMAKE_CXXFLAGS_EXCEPTIONS_ON -= -fexceptions -mthreads
    QMAKE_LFLAGS_EXCEPTIONS_ON -= -mthreads
}
