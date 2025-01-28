
add_library(LDSO_slam 
    # all of the source files 
    Feature.cc
    Settings.cc
    internal/GlobalCalib.cc

)
# # Add the include directory
# target_include_directories(LDSO_slam PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(LDSO_slam
    ${THIRD_PARTY_LIBS}
    )