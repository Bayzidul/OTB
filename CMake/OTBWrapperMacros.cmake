
# When used externally, OTBParseArguments is included explicitely in UseOTB.cmake
# Otherwise, let's include it here
if (NOT COMMAND OTB_PARSE_ARGUMENTS)
  include(OTBParseArguments)
endif()

macro(OTB_CREATE_APPLICATION)
  OTB_PARSE_ARGUMENTS(
      APPLICATION
      "NAME;SOURCES;INCLUDE_DIRS;LINK_LIBRARIES;INSTALL_PATH"
      ""
      ${ARGN})

   set( APPLICATION_TARGET_NAME otbapp_${APPLICATION_NAME} )

   # Build the library as a MODULE (shared lib even if OTB is built statically)
   include_directories(${APPLICATION_INCLUDE_DIRS})
   add_library(${APPLICATION_TARGET_NAME} MODULE ${APPLICATION_SOURCES})
   target_link_libraries(${APPLICATION_TARGET_NAME} OTBApplicationEngine ${APPLICATION_LINK_LIBRARIES})
   
   # Remove the usual 'lib' prefix to make it clear it is a plugin
   # and not a library to link against
   set_target_properties(${APPLICATION_TARGET_NAME} PROPERTIES PREFIX "")

   # on Apple platform, a "MODULE" library gets a ".so" extension
   # but the ITK factory mechanism looks for ".dylib"
   if (APPLE)
     set_target_properties(${APPLICATION_TARGET_NAME} PROPERTIES SUFFIX ".dylib")
   endif()

   # When called from the OTB build system, use OTB_INSTALL_APP_DIR_CM24
   if (NOT APPLICATION_INSTALL_PATH AND OTB_INSTALL_APP_DIR_CM24)
     set(APPLICATION_INSTALL_PATH ${OTB_INSTALL_APP_DIR_CM24})
   endif()

   # Install only if a path has been provided
   if (APPLICATION_INSTALL_PATH)
     install(TARGETS ${APPLICATION_TARGET_NAME}
             LIBRARY DESTINATION ${APPLICATION_INSTALL_PATH})

     if (NOT WIN32)
       configure_file( ${CMAKE_SOURCE_DIR}/CMake/otbcli.sh.in
                       ${CMAKE_CURRENT_BINARY_DIR}/otbcli_${APPLICATION_NAME}.sh
                       @ONLY )
       install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/otbcli_${APPLICATION_NAME}.sh
               DESTINATION ${OTB_INSTALL_BIN_DIR_CM24})
               
       if (WRAP_QT)
           configure_file( ${CMAKE_SOURCE_DIR}/CMake/otbgui.sh.in
                           ${CMAKE_CURRENT_BINARY_DIR}/otbgui_${APPLICATION_NAME}.sh
                           @ONLY )
           install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/otbgui_${APPLICATION_NAME}.sh
                   DESTINATION ${OTB_INSTALL_BIN_DIR_CM24})
       endif(WRAP_QT)
     endif(NOT WIN32)
   endif()
   
   list(APPEND OTB_APPLICATIONS_NAME_LIST ${APPLICATION_NAME})
   set(OTB_APPLICATIONS_NAME_LIST ${OTB_APPLICATIONS_NAME_LIST}
       CACHE STRING "List of all applications" FORCE)
   
endmacro(OTB_CREATE_APPLICATION)





macro(OTB_TEST_APPLICATION)
  OTB_PARSE_ARGUMENTS(
      TESTAPPLICATION
      "NAME;APP;OPTIONS;TESTENVOPTIONS;VALID"
      ""
      ${ARGN})

   if (NOT TESTAPPLICATION_TESTENVOPTIONS)
     add_test(NAME ${TESTAPPLICATION_NAME}
              COMMAND otbTestDriver
              ${TESTAPPLICATION_VALID}
              Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
              ${TESTAPPLICATION_APP}
              $<TARGET_FILE_DIR:otbapp_${TESTAPPLICATION_APP}>
              ${TESTAPPLICATION_OPTIONS}
              --testenv)
   else (NOT TESTAPPLICATION_TESTENVOPTIONS)
     add_test(NAME ${TESTAPPLICATION_NAME}
              COMMAND otbTestDriver
              ${TESTAPPLICATION_VALID}
              Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
              ${TESTAPPLICATION_APP}
              $<TARGET_FILE_DIR:otbapp_${TESTAPPLICATION_APP}>
              ${TESTAPPLICATION_OPTIONS}
              --testenv ${TESTAPPLICATION_TESTENVOPTIONS})
   endif (NOT TESTAPPLICATION_TESTENVOPTIONS)
   
endmacro(OTB_TEST_APPLICATION)
