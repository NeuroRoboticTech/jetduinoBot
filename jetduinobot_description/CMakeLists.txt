cmake_minimum_required(VERSION 2.8.3)
project(jetduinobot_description)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS urdf xacro)


catkin_package(
  CATKIN_DEPENDS urdf xacro
)

install(DIRECTORY robots
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
install(DIRECTORY meshes
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
install(DIRECTORY test
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
install(DIRECTORY urdf
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

#if(CATKIN_ENABLE_TESTING)

#	catkin_add_gtest(${PROJECT_NAME}_test_urdf test/test_urdf.cpp)

#endif()

