vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO silverqx/TinyORM
    REF "v${VERSION}"
    SHA512 7ad4f9407fb354425b2dc6fb35ec9bd5d4b4077650f379bd3e683ddf57883140ee6f3b22d0e718862d484c00a89bd55995c0f6aa446bfee14362968752ce777c
    HEAD_REF main
)

# STL4043 _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING already defined, see:
# https://github.com/silverqx/TinyORM/blob/main/cmake/CommonModules/TinyCommon.cmake#L122

vcpkg_check_features(
    OUT_FEATURE_OPTIONS FEATURE_OPTIONS
    PREFIX TINYORM
    FEATURES
        build-mysql-driver   BUILD_MYSQL_DRIVER
        disable-thread-local DISABLE_THREAD_LOCAL
        inline-constants     INLINE_CONSTANTS
        mysql-ping           MYSQL_PING
        orm                  ORM
        strict-mode          STRICT_MODE
        tom                  TOM
        tom-example          TOM_EXAMPLE
)

if(TINYORM_BUILD_MYSQL_DRIVER)
    list(APPEND FEATURE_OPTIONS -DBUILD_DRIVERS:BOOL=ON)
endif()

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DCMAKE_CXX_SCAN_FOR_MODULES:BOOL=OFF
        -DBUILD_TESTS:BOOL=OFF
        -DBUILD_TREE_DEPLOY:BOOL=OFF
        -DTINY_PORT:STRING=${PORT}
        -DTINY_VCPKG:BOOL=ON
        -DVERBOSE_CONFIGURE:BOOL=ON
        ${FEATURE_OPTIONS}
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup()

if(TINYORM_TOM_EXAMPLE)
    vcpkg_copy_tools(TOOL_NAMES tom AUTO_CLEAN)
endif()

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
