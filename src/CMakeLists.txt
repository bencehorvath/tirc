set(tirclib_SOURCES "IrcConnection.cpp")
set(tirclib_HEADERS "IrcConnection.h" "IrcMessage.h" "IrcReplies.h" "tirclib.h" "${CMAKE_BINARY_DIR}/tirc_config.h")

add_library(tirclib SHARED ${tirclib_SOURCES})
target_link_libraries(tirclib ${tirclib_REQUIRED_LIBRARIES})

install(TARGETS tirclib DESTINATION lib)
install(FILES ${tirclib_HEADERS} DESTINATION include)



if(WITH_TESTER)
	message("-- Building tester app too")
	set(temptester_SOURCES "temp_tester.cpp")

	add_executable(tester ${temptester_SOURCES})
	target_link_libraries(tester ${tirclib_REQUIRED_LIBRARIES} tirclib)
	install(TARGETS tester DESTINATION bin)
    #TEMP!!
    install(TARGETS tirclib DESTINATION bin)
endif()
