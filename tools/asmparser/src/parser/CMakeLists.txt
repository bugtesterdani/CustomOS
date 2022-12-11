###################################
# Sources
file(GLOB_RECURSE HEADER
    ${HEADER_DIR}/parser/*.h
)

target_sources(parser
    PRIVATE
    ${HEADER}
    elfParser.h
    elfParser.cpp
)