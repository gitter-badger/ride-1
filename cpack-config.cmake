INCLUDE(InstallRequiredSystemLibraries)
SET(CPACK_PACKAGE_DESCRIPTION "Ride")
SET(CPACK_PACKAGE_VENDOR "madeso")

SET(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_INSTALL_PREFIX}")

MESSAGE(STATUS "Setting cpack paths to ${PROJECT_SOURCE_DIR}/gfx/install/")
SET(CPACK_DMG_BACKGROUND_IMAGE "${PROJECT_SOURCE_DIR}/gfx/install/dmg-install.png")
SET(CPACK_DMG_DS_STORE "${PROJECT_SOURCE_DIR}/gfx/install/ds_store")

IF(${APPLE})
  SET(CPACK_GENERATOR DragNDrop)
ENDIF()

# this must always be last
INCLUDE(CPack)
