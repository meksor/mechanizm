
add_executable(
    mechanizm 

    gui/main_window.cpp
    gui/project_widget.cpp
    gui/sources_window.cpp
    gui/source_table.cpp
    gui/source_info.cpp
    gui/clip_info.cpp
    gui/clip_table.cpp
    gui/clips_window.cpp
    gui/clip_editor_window.cpp
    gui/player_widget.cpp
    gui/rythmic_point_table.cpp

    mapping.cpp
    clip.cpp
    source.cpp
    sequence.cpp
    project.cpp
    json.cpp
    application.cpp
    main.cpp)
