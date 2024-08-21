# Obtain all installed configuration names for currently searched package
function(tiny_get_target_configurations out_target_configuations)

    # Parent folder contains all target configurations
    get_filename_component(targetsDir ${CMAKE_PARENT_LIST_FILE} DIRECTORY)

    file(GLOB targets "${targetsDir}/${PACKAGE_FIND_NAME}[Tt]argets-*.cmake")

    set(configurations)

    foreach(target ${targets})
        string(REGEX MATCH "${PACKAGE_FIND_NAME}[Tt]argets-([a-zA-Z]+).cmake$"
            out "${target}"
        )

        if(NOT CMAKE_MATCH_1)
            continue()
        endif()

        string(TOLOWER ${CMAKE_MATCH_1} configuration)
        list(APPEND configurations ${configuration})
    endforeach()

    set(TINY_${CMAKE_FIND_PACKAGE_NAME}_FOUND_CONFIGURATIONS "${configurations}"
        CACHE INTERNAL
        "Found all installed target configurations for single-configuration \
installations.")

    set(${out_target_configuations} ${configurations} PARENT_SCOPE)

endfunction()

# Check whether passed configurations contains only one Debug configuration
function(tiny_is_debug_only_config out_is_debug_only_config target_configurations)

    list(LENGTH target_configurations count)
    set(result FALSE)

    if(count EQUAL 1)
        list(GET target_configurations 0 configuration)
        string(TOLOWER ${configuration} configuration)

        if(configuration STREQUAL "debug")
            set(result TRUE)
        endif()
    endif()

    set(${out_is_debug_only_config} ${result} PARENT_SCOPE)

endfunction()

# Convert target configurations to the printable string
function(tiny_printable_configurations out_configurations configurations)

    set(result)

    foreach(configuration ${configurations})
        string(SUBSTRING ${configuration} 0 1 firstLetter)
        string(TOUPPER ${firstLetter} firstLetter)

        string(REGEX REPLACE "^.(.*)" "${firstLetter}\\1"
            configuration ${configuration}
        )

        list(APPEND result ${configuration})
    endforeach()

    string(JOIN "," result ${result})

    set(${out_configurations} ${result} PARENT_SCOPE)

endfunction()

# Convert to the boolean value
function(tiny_to_bool out_variable value)

    # This function may look useless, but I'm using it because I want to have unified
    # boolean values, so it's TRUE or FALSE and not ON/OFF, ...

    # Don't use the Variable Reference here ${value}, an undefined value can be
    # controlled while passing the value to this function using an unquoted
    # Variable Reference like:
    # tiny_to_bool(xyz ${some_bool})
    # This means if the value is undefined (empty) and will be unquoted, then it fails;
    # if it is undefined and quoted, it returns FALSE
    if(value)
        set(${out_variable} TRUE PARENT_SCOPE)
    else()
        set(${out_variable} FALSE PARENT_SCOPE)
    endif()

endfunction()

# Build configuration type requirements for an Install Tree
# Build configurations are considered to match:
# 1. For Debug - CVF has to contain Debug configuration
# 2. For Non Debug - CVF can not contain only Debug configuration
function(tiny_build_type_requirements_install_tree
        out_package_version out_package_version_unsuitable
        cvf_is_multi_config cvf_is_vcpkg
)
    # Obtain all installed target configurations
    # Needed in the Package Config file for the info message
    tiny_get_target_configurations(cvfTargetConfigurations)

    message(DEBUG "cvfTargetConfigurations = ${cvfTargetConfigurations}")
    message(DEBUG "cvf_is_multi_config = ${cvf_is_multi_config}")
    message(DEBUG "cvf_is_vcpkg = ${cvf_is_vcpkg}")
    message(DEBUG "CMAKE_CURRENT_LIST_FILE = ${CMAKE_CURRENT_LIST_FILE}")

    # The isMultiConfig means that the current application that is linking against
    # the TinyORM is multi-config, the cvf_is_multi_config means that the TinyORM package
    # is multi-config, these are two different things

    # The below check if the TinyORM package is suitable and the checks are done only if
    # TinyORM is single-config, they are not done for multi-config

    # VCPKG is multi-config too, but it doesn't set GENERATOR_IS_MULTI_CONFIG property
    if(NOT cvf_is_multi_config AND NOT cvf_is_vcpkg)
        tiny_is_debug_only_config(cvfIsDebugOnly "${cvfTargetConfigurations}")

        list(LENGTH cvfTargetConfigurations count)

        get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
        tiny_to_bool(isMultiConfig ${isMultiConfig})

        # Used in STREQUAL comparisons
        string(TOLOWER "${CMAKE_BUILD_TYPE}" cmakeBuildTypeLower)

        message(DEBUG "isMultiConfig = ${isMultiConfig}")
        message(DEBUG "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
        message(DEBUG "cvfIsDebugOnly = ${cvfIsDebugOnly}")
        message(DEBUG "MSVC = ${MSVC}")

        # Multi-config is not suitable for TinyORM single-config
        if(isMultiConfig)
            # The single-config is correct, it will be used in the info message to print
            # that multi-config is not suitable for TinyORM multi-config
            set(${out_package_version} "${${out_package_version}} single-config"
                PARENT_SCOPE)
            set(${out_package_version_unsuitable} TRUE PARENT_SCOPE)
            return()

        # Unsuitable because no installed targets
        elseif(count EQUAL 0)
            set(${out_package_version} "${${out_package_version}} no-configuration"
                PARENT_SCOPE)
            set(${out_package_version_unsuitable} TRUE PARENT_SCOPE)
            return()

        # Match Debug builds types for MSVC (linking debug against release
        # (or vice-versa) cause crashes)
        elseif(MSVC AND ((cmakeBuildTypeLower STREQUAL "debug" AND
                NOT "debug" IN_LIST cvfTargetConfigurations) OR
                (NOT cmakeBuildTypeLower STREQUAL "debug" AND cvfIsDebugOnly))
        )
            # Obtain target configurations in printable format
            tiny_printable_configurations(tinyPrintableConfigurations
                "${cvfTargetConfigurations}"
            )

            set(${out_package_version}
                "${${out_package_version}} single-config [${tinyPrintableConfigurations}]"
                PARENT_SCOPE)
            set(${out_package_version_unsuitable} TRUE PARENT_SCOPE)
            return()
        endif()
    endif()

endfunction()

# Build configuration type requirements for a Build Tree
# Build configurations are considered to match only when they are equal if
# the MATCH_EQUAL_EXPORTED_BUILDTREE option is enabled or for MSVC, build
# configurations are considered to match if both are "Debug" or neither
# is "Debug", otherwise all other configurations can be matched freely.
function(tiny_build_type_requirements_build_tree
        out_package_version out_package_version_unsuitable
        cvf_is_multi_config cvf_config_build_type cvf_match_buildtree
)
    message(DEBUG "cvf_is_multi_config = ${cvf_is_multi_config}")
    message(DEBUG "CMAKE_CURRENT_LIST_FILE = ${CMAKE_CURRENT_LIST_FILE}")

    # The isMultiConfig means that the current application that is linking against
    # the TinyORM is multi-config, the cvf_is_multi_config means that the TinyORM package
    # is multi-config, these are two different things

    # The below check if the TinyORM package is suitable and the checks are done only if
    # TinyORM is single-config, they are not done for multi-config

    if(NOT cvf_is_multi_config)
        get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
        tiny_to_bool(isMultiConfig ${isMultiConfig})

        # Used in STREQUAL comparisons
        string(TOLOWER "${CMAKE_BUILD_TYPE}" cmakeBuildTypeLower)
        string(TOLOWER "${cvf_config_build_type}" cvfConfigBuildTypeLower)

        message(DEBUG "isMultiConfig = ${isMultiConfig}")
        message(DEBUG "cvf_match_buildtree = ${cvf_match_buildtree}")
        message(DEBUG "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
        message(DEBUG "cvf_config_build_type = ${cvf_config_build_type}")
        message(DEBUG "MSVC = ${MSVC}")

        # TODO forbid MINGW64 vs UCRT64 vs MSVC mismatch on windows silverqx

        # Multi-config is not suitable for TinyORM single-config
        if(isMultiConfig)
            # The single-config is correct, it will be used in the info message to print
            # that multi-config is not suitable for TinyORM multi-config
            set(${out_package_version} "${${out_package_version}} single-config"
                PARENT_SCOPE)
            set(${out_package_version_unsuitable} TRUE PARENT_SCOPE)
            return()

        # Match Debug builds types for MSVC (linking debug against release
        # (or vice-versa) cause crashes)
        # Or if matching equal build tree was enabled and builds types don't match then
        # also tag as unsuitable (this is Build tree specific)
        elseif((cvf_match_buildtree AND
                NOT cmakeBuildTypeLower STREQUAL cvfConfigBuildTypeLower) OR
                (MSVC AND ((cmakeBuildTypeLower STREQUAL "debug" AND
                    NOT cvfConfigBuildTypeLower STREQUAL "debug") OR
                    (NOT cmakeBuildTypeLower STREQUAL "debug" AND
                        cvfConfigBuildTypeLower STREQUAL "debug")))
        )
            set(${out_package_version}
                "${${out_package_version}} single-config CMAKE_BUILD_TYPE=${cvf_config_build_type}"
                PARENT_SCOPE)
            set(${out_package_version_unsuitable} TRUE PARENT_SCOPE)
            return()
        endif()
    endif()

endfunction()

# Information about build type/s, used in the info message
function(tiny_get_build_types out_build_types cvf_is_multi_config cvf_is_vcpkg)

    tiny_printable_configurations(tinyPrintableConfigurations
        "${TINY_${CMAKE_FIND_PACKAGE_NAME}_FOUND_CONFIGURATIONS}"
    )

    if(cvf_is_vcpkg)
        set(tiny_build_type " Vcpkg[${tinyPrintableConfigurations}]")
    elseif(cvf_is_multi_config)
        set(tiny_build_type " Multi-Config[${tinyPrintableConfigurations}]")
    else()
        set(tiny_build_type " [${tinyPrintableConfigurations}]")
    endif()

    set(${out_build_types} ${tiny_build_type} PARENT_SCOPE)

endfunction()
