add_library(LDSO_slam
    # all of the source files 
    Feature.cc
)

target_link_libraries(LDSO_slam
    ${THIRD_PARTY_LIBS}
    )