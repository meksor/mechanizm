
add_executable(
    mechanizm 

    gui/mappings_widget.cpp
    gui/mapping_editor_widget.cpp
    gui/clips_widget.cpp
    gui/clip_editor_widget.cpp
    gui/player_widget.cpp
    gui/sources_widget.cpp
    gui/sequences_widget.cpp
    gui/player_widget.cpp
    gui/launcher_widget.cpp
    gui/application.cpp

    mapping.cpp
    clip.cpp
    source.cpp
    sequence.cpp
    project.cpp
    json.cpp
    main.cpp)
