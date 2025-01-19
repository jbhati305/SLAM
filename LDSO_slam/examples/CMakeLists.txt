# we have 3 types of example binaries
# All target different types of datasets namely Kitti, EuRoC and TUM

add_executable(run_ldso_slam_tum_mono run_ldso_slam_tum_mono.cc)
target_link_libraries(run_ldso_slam_tum_mono 
    LDSO_slam ${THIRD_PARTY_LIBS}
    )

# here follows kiti and euroc ...skip for now