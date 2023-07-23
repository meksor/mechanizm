
add_executable(
    mechanizm 

    gui/main_window.cpp
    gui/project_widget.cpp
    gui/player_widget.cpp

    gui/source/sources_window.cpp
    gui/source/source_table.cpp
    gui/source/source_info.cpp

    gui/clip/clip_editor_window.cpp
    gui/clip/clip_info.cpp
    gui/clip/clip_table.cpp
    gui/clip/clips_window.cpp
    gui/clip/rythmic_point_table.cpp

    gui/sequence/sequence_info.cpp
    gui/sequence/sequence_table.cpp
    gui/sequence/sequences_window.cpp

    gui/mapping/mapping_info.cpp
    gui/mapping/mapping_table.cpp
    gui/mapping/mappings_window.cpp
    gui/mapping/channel_table.cpp
    gui/mapping/mapping_editor_window.cpp


    mapping.cpp
    clip.cpp
    source.cpp
    sequence.cpp
    project.cpp
    json.cpp
    application.cpp
    main.cpp)
