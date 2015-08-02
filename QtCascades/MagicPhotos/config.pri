# Config.pri file version 2.0. Auto-generated by IDE. Any changes made by user will be lost!
BASEDIR = $$quote($$_PRO_FILE_PWD_)

device {
    CONFIG(debug, debug|release) {
        profile {
            INCLUDEPATH += $$quote(${QNX_TARGET}/usr/include/bb/cascades/pickers) \
                $$quote(${QNX_TARGET}/usr/include/bb)

            DEPENDPATH += $$quote(${QNX_TARGET}/usr/include/bb/cascades/pickers) \
                $$quote(${QNX_TARGET}/usr/include/bb)

            LIBS += -lbbcascadespickers \
                -lbb

            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        } else {
            INCLUDEPATH += $$quote(${QNX_TARGET}/usr/include/bb/cascades/pickers) \
                $$quote(${QNX_TARGET}/usr/include/bb)

            DEPENDPATH += $$quote(${QNX_TARGET}/usr/include/bb/cascades/pickers) \
                $$quote(${QNX_TARGET}/usr/include/bb)

            LIBS += -lbbcascadespickers \
                -lbb

            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }

    }

    CONFIG(release, debug|release) {
        !profile {
            INCLUDEPATH += $$quote(${QNX_TARGET}/usr/include/bb/cascades/pickers) \
                $$quote(${QNX_TARGET}/usr/include/bb)

            DEPENDPATH += $$quote(${QNX_TARGET}/usr/include/bb/cascades/pickers) \
                $$quote(${QNX_TARGET}/usr/include/bb)

            LIBS += -lbbcascadespickers \
                -lbb

            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }
    }
}

simulator {
    CONFIG(debug, debug|release) {
        !profile {
            INCLUDEPATH += $$quote(${QNX_TARGET}/usr/include/bb/cascades/pickers) \
                $$quote(${QNX_TARGET}/usr/include/bb)

            DEPENDPATH += $$quote(${QNX_TARGET}/usr/include/bb/cascades/pickers) \
                $$quote(${QNX_TARGET}/usr/include/bb)

            LIBS += -lbbcascadespickers \
                -lbb

            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }
    }
}

config_pri_assets {
    OTHER_FILES += \
        $$quote($$BASEDIR/assets/BlurPage.qml) \
        $$quote($$BASEDIR/assets/BlurPreviewPage.qml) \
        $$quote($$BASEDIR/assets/CartoonPage.qml) \
        $$quote($$BASEDIR/assets/CartoonPreviewPage.qml) \
        $$quote($$BASEDIR/assets/DecolorizePage.qml) \
        $$quote($$BASEDIR/assets/HelpPage.qml) \
        $$quote($$BASEDIR/assets/PixelatePage.qml) \
        $$quote($$BASEDIR/assets/PixelatePreviewPage.qml) \
        $$quote($$BASEDIR/assets/RecolorPage.qml) \
        $$quote($$BASEDIR/assets/RetouchPage.qml) \
        $$quote($$BASEDIR/assets/SketchPage.qml) \
        $$quote($$BASEDIR/assets/SketchPreviewPage.qml) \
        $$quote($$BASEDIR/assets/doc/clone_in_progress.png) \
        $$quote($$BASEDIR/assets/doc/final_image.png) \
        $$quote($$BASEDIR/assets/doc/help.html) \
        $$quote($$BASEDIR/assets/doc/help_de.html) \
        $$quote($$BASEDIR/assets/doc/help_fr.html) \
        $$quote($$BASEDIR/assets/doc/help_ru.html) \
        $$quote($$BASEDIR/assets/doc/mode_blur_button.png) \
        $$quote($$BASEDIR/assets/doc/mode_clone_button.png) \
        $$quote($$BASEDIR/assets/doc/mode_effected_button.png) \
        $$quote($$BASEDIR/assets/doc/mode_hue_selection_button.png) \
        $$quote($$BASEDIR/assets/doc/mode_original_button.png) \
        $$quote($$BASEDIR/assets/doc/mode_sampling_point_button.png) \
        $$quote($$BASEDIR/assets/doc/mode_scroll_button.png) \
        $$quote($$BASEDIR/assets/doc/original_image.png) \
        $$quote($$BASEDIR/assets/doc/recommend_app_button.png) \
        $$quote($$BASEDIR/assets/doc/review_app_button.png) \
        $$quote($$BASEDIR/assets/doc/sampling_point_selected.png) \
        $$quote($$BASEDIR/assets/images/apply.png) \
        $$quote($$BASEDIR/assets/images/edit_mode_blur.png) \
        $$quote($$BASEDIR/assets/images/edit_mode_cartoon.png) \
        $$quote($$BASEDIR/assets/images/edit_mode_decolorize.png) \
        $$quote($$BASEDIR/assets/images/edit_mode_pixelate.png) \
        $$quote($$BASEDIR/assets/images/edit_mode_recolor.png) \
        $$quote($$BASEDIR/assets/images/edit_mode_retouch.png) \
        $$quote($$BASEDIR/assets/images/edit_mode_sketch.png) \
        $$quote($$BASEDIR/assets/images/hue_bar.png) \
        $$quote($$BASEDIR/assets/images/hue_slider.png) \
        $$quote($$BASEDIR/assets/images/mode_blur.png) \
        $$quote($$BASEDIR/assets/images/mode_clone.png) \
        $$quote($$BASEDIR/assets/images/mode_effected.png) \
        $$quote($$BASEDIR/assets/images/mode_hue_selection.png) \
        $$quote($$BASEDIR/assets/images/mode_original.png) \
        $$quote($$BASEDIR/assets/images/mode_sampling_point.png) \
        $$quote($$BASEDIR/assets/images/mode_scroll.png) \
        $$quote($$BASEDIR/assets/images/open.png) \
        $$quote($$BASEDIR/assets/images/review.png) \
        $$quote($$BASEDIR/assets/images/sampling_point.png) \
        $$quote($$BASEDIR/assets/images/save.png) \
        $$quote($$BASEDIR/assets/images/share.png) \
        $$quote($$BASEDIR/assets/images/undo.png) \
        $$quote($$BASEDIR/assets/main.qml) \
        $$quote($$BASEDIR/assets/models/modeSelectionListViewModel.xml)
}

config_pri_source_group1 {
    SOURCES += \
        $$quote($$BASEDIR/src/MagicPhotos.cpp) \
        $$quote($$BASEDIR/src/appsettings.cpp) \
        $$quote($$BASEDIR/src/blureditor.cpp) \
        $$quote($$BASEDIR/src/brushpreviewgenerator.cpp) \
        $$quote($$BASEDIR/src/cartooneditor.cpp) \
        $$quote($$BASEDIR/src/customtimer.cpp) \
        $$quote($$BASEDIR/src/decolorizeeditor.cpp) \
        $$quote($$BASEDIR/src/downloadinviter.cpp) \
        $$quote($$BASEDIR/src/invocationhelper.cpp) \
        $$quote($$BASEDIR/src/main.cpp) \
        $$quote($$BASEDIR/src/pixelateeditor.cpp) \
        $$quote($$BASEDIR/src/recoloreditor.cpp) \
        $$quote($$BASEDIR/src/retoucheditor.cpp) \
        $$quote($$BASEDIR/src/sketcheditor.cpp) \
        $$quote($$BASEDIR/src/trialmanager.cpp)

    HEADERS += \
        $$quote($$BASEDIR/src/MagicPhotos.hpp) \
        $$quote($$BASEDIR/src/appsettings.h) \
        $$quote($$BASEDIR/src/blureditor.h) \
        $$quote($$BASEDIR/src/brushpreviewgenerator.h) \
        $$quote($$BASEDIR/src/cartooneditor.h) \
        $$quote($$BASEDIR/src/customtimer.h) \
        $$quote($$BASEDIR/src/decolorizeeditor.h) \
        $$quote($$BASEDIR/src/downloadinviter.h) \
        $$quote($$BASEDIR/src/invocationhelper.h) \
        $$quote($$BASEDIR/src/pixelateeditor.h) \
        $$quote($$BASEDIR/src/recoloreditor.h) \
        $$quote($$BASEDIR/src/retoucheditor.h) \
        $$quote($$BASEDIR/src/sketcheditor.h) \
        $$quote($$BASEDIR/src/trialmanager.h)
}

INCLUDEPATH += $$quote($$BASEDIR/src)

CONFIG += precompile_header

PRECOMPILED_HEADER = $$quote($$BASEDIR/precompiled.h)

lupdate_inclusion {
    SOURCES += \
        $$quote($$BASEDIR/../src/*.c) \
        $$quote($$BASEDIR/../src/*.c++) \
        $$quote($$BASEDIR/../src/*.cc) \
        $$quote($$BASEDIR/../src/*.cpp) \
        $$quote($$BASEDIR/../src/*.cxx) \
        $$quote($$BASEDIR/../assets/*.qml) \
        $$quote($$BASEDIR/../assets/*.js) \
        $$quote($$BASEDIR/../assets/*.qs) \
        $$quote($$BASEDIR/../assets/doc/*.qml) \
        $$quote($$BASEDIR/../assets/doc/*.js) \
        $$quote($$BASEDIR/../assets/doc/*.qs) \
        $$quote($$BASEDIR/../assets/images/*.qml) \
        $$quote($$BASEDIR/../assets/images/*.js) \
        $$quote($$BASEDIR/../assets/images/*.qs) \
        $$quote($$BASEDIR/../assets/models/*.qml) \
        $$quote($$BASEDIR/../assets/models/*.js) \
        $$quote($$BASEDIR/../assets/models/*.qs)

    HEADERS += \
        $$quote($$BASEDIR/../src/*.h) \
        $$quote($$BASEDIR/../src/*.h++) \
        $$quote($$BASEDIR/../src/*.hh) \
        $$quote($$BASEDIR/../src/*.hpp) \
        $$quote($$BASEDIR/../src/*.hxx)
}

TRANSLATIONS = $$quote($${TARGET}_de.ts) \
    $$quote($${TARGET}_fr.ts) \
    $$quote($${TARGET}_ru.ts) \
    $$quote($${TARGET}.ts)
