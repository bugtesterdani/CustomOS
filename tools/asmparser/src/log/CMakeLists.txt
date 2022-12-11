###################################
# Sources
file(GLOB_RECURSE HEADER
    ${HEADER_DIR}/log/*.h
)

target_sources(log
    PRIVATE
    ${HEADER}
    log.cpp
)