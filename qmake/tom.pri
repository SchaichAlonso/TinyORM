# Path to the TinyORM source tree
TINYORM_SOURCE_TREE = $$clean_path($$quote($$PWD/..))
# Path to the Tom source tree
TINYTOM_SOURCE_TREE = $$quote($$TINYORM_SOURCE_TREE/tom)

# Qt Common Configuration
# ---

QT *= core sql

CONFIG *= link_prl

include($$TINYORM_SOURCE_TREE/qmake/common.pri)

# Configure TinyORM library
# ---
# everything else is defined in the qmake/common.pri

# Link against the shared library
CONFIG(shared, dll|shared|static|staticlib) | \
CONFIG(dll, dll|shared|static|staticlib): \
    DEFINES *= TINYORM_LINKING_SHARED

# Disable the ORM-related source code
disable_orm: DEFINES *= TINYORM_DISABLE_ORM
# Forbid disabling the tom-related source code and header files
disable_tom: \
    error( "Disabling the tom is forbidden in the tom.pri, please remove\
            the 'disable_tom' configuration." )

# File version
# ---

# Find version numbers in the version header file and assign them to the
# <TARGET>_VERSION_<MAJOR,MINOR,PATCH,TWEAK> and also to the VERSION variable.
load(tiny_version_numbers)
tiny_version_numbers($$quote($$TINYTOM_SOURCE_TREE/include/tom/version.hpp))

# Windows resource and manifest files
# ---

# To find version.hpp
tinyRcIncludepath = $$quote($$TINYTOM_SOURCE_TREE/include/)
# To find Windows manifest
mingw: tinyRcIncludepath += $$quote($$TINYTOM_SOURCE_TREE/resources/)

load(tiny_resource_and_manifest)
tiny_resource_and_manifest(                                               \
    $$tinyRcIncludepath, $$TINYTOM_SOURCE_TREE/resources, tom, Tom        \
)

# Link against TinyORM library
# ---

!isEmpty(TINYORM_SOURCE_TREE): \
exists($$TINYORM_SOURCE_TREE) {
    win32-msvc|win32-clang-msvc: \
        INCLUDEPATH *= \
            $$quote($$TINYORM_SOURCE_TREE/include/) \
            $$quote($$TINYTOM_SOURCE_TREE/include/)
    else: \
        QMAKE_CXXFLAGS += \
            -isystem $$shell_quote($$TINYORM_SOURCE_TREE/include/) \
            -isystem $$shell_quote($$TINYTOM_SOURCE_TREE/include/)
}

!isEmpty(TINYORM_BUILD_TREE): \
exists($$TINYORM_BUILD_TREE): \
    LIBS += $$quote(-L$$clean_path($$TINYORM_BUILD_TREE)/src$${TINY_BUILD_SUBFOLDER}/)

# TinyOrm library can be on the system path or LD_LIBRARY_PATH so don't depend
# on the TINYORM_BUILD_TREE
LIBS += -lTinyOrm
