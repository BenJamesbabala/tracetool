ADD_EXECUTABLE(test_filter test_filter.cpp)
TARGET_LINK_LIBRARIES(test_filter tracelib)

SET_TARGET_PROPERTIES(test_filter PROPERTIES COMPILE_FLAGS /I..)

ENABLE_TESTING()
ADD_TEST(NAME test_filter COMMAND test_filter)
