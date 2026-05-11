add_library(Event STATIC)

target_link_libraries(Event
    PRIVATE
        Utility
)

target_sources(Event PRIVATE
    EventBus.cpp
    EventListener.cpp
    EventSubscription.cpp
)

target_include_directories(Event PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)