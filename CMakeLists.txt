cmake_minimum_required(VERSION 3.10)
project(GazeboImageTransport)

find_package(Protobuf REQUIRED)
find_package(OpenCV REQUIRED)
find_package(gazebo REQUIRED)
find_package(TBB REQUIRED)

include_directories(${Protobuf_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS} ${GAZEBO_INCLUDE_DIRS} ${TBB_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})

set(PROTO_FILES src/my_image.proto)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

add_library(proto_files ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(proto_files ${Protobuf_LIBRARIES})

add_executable(image_publisher src/image_publisher.cpp ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(image_publisher proto_files ${Protobuf_LIBRARIES} ${OpenCV_LIBRARIES} ${GAZEBO_LIBRARIES} ${TBB_LIBRARIES})
target_link_options(image_publisher PRIVATE "-Wl,--copy-dt-needed-entries")

add_executable(image_subscriber src/image_subscriber.cpp ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(image_subscriber proto_files ${Protobuf_LIBRARIES} ${OpenCV_LIBRARIES}  ${GAZEBO_LIBRARIES} ${TBB_LIBRARIES})
target_link_options(image_subscriber PRIVATE "-Wl,--copy-dt-needed-entries")

install(TARGETS image_publisher image_subscriber
        RUNTIME DESTINATION bin)
install(FILES ${PROTO_FILES} DESTINATION proto)

