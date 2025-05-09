
# =============================================
if (WIN32)
    message("CMAKE_CXX_COMPILER_ID ${CMAKE_CXX_COMPILER_ID}") # PRE_BUILD | PRE_LINK | POST_BUILD
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD COMMAND python "${CMAKE_SOURCE_DIR}/../scripts/make-output/cpp-lib.py" win32msvc ${PROJECT_NAME} ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR})
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD COMMAND python "${CMAKE_SOURCE_DIR}/../scripts/make-output/cpp-lib.py" win32mingw ${PROJECT_NAME} ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR})
    endif ()
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS TRUE
    )
elseif (ANDROID)
    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS AndroidExtras)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS AndroidExtras)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::AndroidExtras)
    set(ANDROID_ABI armeabi-v7a)
    set(ANDROID_NATIVE_API_LEVEL 21)
    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD COMMAND python3 "${CMAKE_SOURCE_DIR}/../scripts/make-output/cpp-lib.py" android ${PROJECT_NAME} ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR})
    #    set_target_properties(qtframeless PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/android-build/libs/armeabi-v7a")
elseif (UNIX AND NOT APPLE AND NOT WIN32 AND NOT ANDROID)
    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS X11Extras)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS X11Extras)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::X11Extras Qt${QT_VERSION_MAJOR}::CorePrivate)
    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD COMMAND python3 "${CMAKE_SOURCE_DIR}/../scripts/make-output/cpp-lib.py" linux64 ${PROJECT_NAME} ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR})
endif()
# =============================================




