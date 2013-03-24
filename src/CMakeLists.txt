set(tirclib_SOURCES "IrcConnection.cpp")
set(tirclib_HEADERS "IrcConnection.h" "IrcMessage.h" "IrcReplies.h" "tirclib.h" "${CMAKE_BINARY_DIR}/tirc_config.h")

add_library(tirc SHARED ${tirclib_SOURCES})
target_link_libraries(tirc ${tirclib_REQUIRED_LIBRARIES})

install(TARGETS tirc DESTINATION lib)
install(TARGETS tirc DESTINATION bin)
install(FILES ${tirclib_HEADERS} DESTINATION include)



if(WITH_TESTER)
	message("-- Building tester app too")
	set(temptester_SOURCES "temp_tester.cpp")

	add_executable(tester ${temptester_SOURCES})
	target_link_libraries(tester ${tirclib_REQUIRED_LIBRARIES} tirc)
	install(TARGETS tester DESTINATION bin)
    #TEMP!!
    #install(TARGETS tirc DESTINATION bin)
endif()
