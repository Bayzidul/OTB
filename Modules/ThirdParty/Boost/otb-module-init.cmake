# Required
message(STATUS "Looking for required Boost headers")

# Mandatory components
# - date_time -> ossim plugins
set(OTB_Boost_COMPONENTS date_time)
# Optional components
# Boost (OPTIONAL_COMPONENTS does not work with Boost find_package)
# unit_test_framework component is used only in GdalAdapters module
if (BUILD_TESTING)
  list(APPEND OTB_Boost_COMPONENTS unit_test_framework)
endif()

#Quiet find package to avoid message "Could not find boost" because unit_testing_framework is not installed
#message(STATUS "Looking for optional Boost components : ${OTB_Boost_OPTIONAL_COMPONENTS}")
find_package ( Boost
  REQUIRED
  1.35.0
  COMPONENTS ${OTB_Boost_COMPONENTS}
  )
