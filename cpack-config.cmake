INCLUDE(InstallRequiredSystemLibraries)
SET(CPACK_PACKAGE_DESCRIPTION "Ride")
SET(CPACK_PACKAGE_VENDOR "madeso")

MESSAGE(STATUS "Setting cpack paths to ${PROJECT_SOURCE_DIR}/gfx/install/")
SET(CPACK_DMG_BACKGROUND_IMAGE "${PROJECT_SOURCE_DIR}/gfx/install/install.png")
SET(CPACK_DMG_DS_STORE "${PROJECT_SOURCE_DIR}/gfx/install/ds_store")
INCLUDE(CPack)