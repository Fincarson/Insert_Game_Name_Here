cmake_minimum_required(VERSION 3.27)
project(Insert_Game_Name_Here)

set(CMAKE_CXX_STANDARD 17)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories(../../../../../../../AppData/Local/Temp)
include_directories(Resource)

file(COPY Resource/audios DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource/fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource/images DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY allegro_monolith-5.2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


add_executable(Insert_Game_Name_Here
        Engine/Allegro5Exception.cpp
        Engine/Allegro5Exception.hpp
        Engine/AudioHelper.cpp
        Engine/AudioHelper.hpp
        Engine/Collider.cpp
        Engine/Collider.hpp
        Engine/GameEngine.cpp
        Engine/GameEngine.hpp
        Engine/Group.cpp
        Engine/Group.hpp
        Engine/IControl.cpp
        Engine/IControl.hpp
        Engine/IObject.cpp
        Engine/IObject.hpp
        Engine/IScene.cpp
        Engine/IScene.hpp
        Engine/LOG.cpp
        Engine/LOG.hpp
        Engine/Point.cpp
        Engine/Point.hpp
        Engine/Resources.cpp
        Engine/Resources.hpp
        Engine/Sprite.cpp
        Engine/Sprite.hpp

        main.cpp
        main.cpp
        Scenes/MenuScene.hpp
        Scenes/MenuScene.cpp

        UI/Image.cpp
        UI/ImageButton.cpp
        UI/ImageButton.hpp
        UI/Label.cpp
        UI/Label.hpp
        UI/Slider.cpp
        UI/Slider.hpp
        UI/Textbox.cpp
        UI/Textbox.hpp
        UI/TextButton.cpp
        UI/TextButton.hpp
        Scenes/PlayScene.cpp
        Scenes/PlayScene.hpp
        Engine/AnimSprite.cpp
        Engine/AnimSprite.hpp
        Sprites/Player.cpp
        Sprites/Player.hpp
        UI/Video.cpp
)






IF (WIN32)
    target_include_directories(Insert_Game_Name_Here PUBLIC C:/allegro/include)
    target_link_directories(Insert_Game_Name_Here PUBLIC C:/allegro/lib)
    target_compile_options(Insert_Game_Name_Here PUBLIC ${ALLEGRO5_CFLAGS_OTHER})
    target_link_libraries(Insert_Game_Name_Here liballegro_monolith)
ELSE()
    find_package(PkgConfig)
    pkg_check_modules(ALLEGRO5 REQUIRED allegro-5 allegro_main-5 allegro_font-5 allegro_dialog-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5 allegro_audio-5 allegro_acodec-5)
    target_include_directories(Insert_Game_Name_Here PUBLIC lib)
    target_include_directories(Insert_Game_Name_Here PUBLIC ${ALLEGRO5_INCLUDE_DIRS})
    target_compile_options(Insert_Game_Name_Here PUBLIC ${ALLEGRO5_CFLAGS_OTHER})
    target_link_libraries(Insert_Game_Name_Here ${ALLEGRO5_LIBRARIES})
    target_link_directories(Insert_Game_Name_Here PUBLIC ${ALLEGRO5_LIBRARY_DIRS})
ENDIF()