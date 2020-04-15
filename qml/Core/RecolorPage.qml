import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0
import ImageEditor 1.0

import "../Util.js" as UtilScript

Page {
    id: recolorPage

    header: Pane {
        topPadding:          recolorPage.bannerViewHeight
        Material.background: Material.LightBlue

        ButtonGroup {
            buttons: headerRow.children
        }

        Row {
            id:               headerRow
            anchors.centerIn: parent
            enabled:          false

            Button {
                id:             scrollModeButton
                implicitWidth:  UtilScript.dp(UIHelper.screenDpi, 48)
                implicitHeight: UtilScript.dp(UIHelper.screenDpi, 48)
                checkable:      true
                checked:        true

                contentItem: Image {
                    source:   "qrc:/resources/images/mode_scroll.png"
                    fillMode: Image.PreserveAspectFit
                }
            }

            Button {
                id:             originalModeButton
                implicitWidth:  UtilScript.dp(UIHelper.screenDpi, 48)
                implicitHeight: UtilScript.dp(UIHelper.screenDpi, 48)
                checkable:      true

                contentItem: Image {
                    source:   "qrc:/resources/images/mode_original.png"
                    fillMode: Image.PreserveAspectFit
                }
            }

            Button {
                id:             effectedModeButton
                implicitWidth:  UtilScript.dp(UIHelper.screenDpi, 48)
                implicitHeight: UtilScript.dp(UIHelper.screenDpi, 48)
                checkable:      true

                contentItem: Image {
                    source:   "qrc:/resources/images/mode_effected.png"
                    fillMode: Image.PreserveAspectFit
                }
            }

            Button {
                id:             hueSelectionModeButton
                implicitWidth:  UtilScript.dp(UIHelper.screenDpi, 48)
                implicitHeight: UtilScript.dp(UIHelper.screenDpi, 48)
                checkable:      true

                contentItem: Image {
                    source:   "qrc:/resources/images/mode_hue_selection.png"
                    fillMode: Image.PreserveAspectFit
                }
            }
        }
    }

    footer: ToolBar {
        RowLayout {
            anchors.fill: parent

            ToolButton {
                id:               saveToolButton
                implicitWidth:    UtilScript.dp(UIHelper.screenDpi, 48)
                implicitHeight:   UtilScript.dp(UIHelper.screenDpi, 48)
                enabled:          false
                Layout.alignment: Qt.AlignHCenter

                contentItem: Image {
                    source:   "qrc:/resources/images/tool_save.png"
                    fillMode: Image.PreserveAspectFit
                }

                onClicked: {
                    recolorPage.shareActionActive = false;

                    recolorPage.editor.saveImage(MediaStoreHelper.imageFilePath);
                }
            }

            ToolButton {
                id:               shareToolButton
                implicitWidth:    UtilScript.dp(UIHelper.screenDpi, 48)
                implicitHeight:   UtilScript.dp(UIHelper.screenDpi, 48)
                enabled:          false
                Layout.alignment: Qt.AlignHCenter

                contentItem: Image {
                    source:   "qrc:/resources/images/tool_share.png"
                    fillMode: Image.PreserveAspectFit
                }

                onClicked: {
                    recolorPage.shareActionActive = true;

                    recolorPage.editor.saveImage(MediaStoreHelper.imageFilePath);
                }
            }

            ToolButton {
                id:               undoToolButton
                implicitWidth:    UtilScript.dp(UIHelper.screenDpi, 48)
                implicitHeight:   UtilScript.dp(UIHelper.screenDpi, 48)
                enabled:          false
                Layout.alignment: Qt.AlignHCenter

                contentItem: Image {
                    source:   "qrc:/resources/images/tool_undo.png"
                    fillMode: Image.PreserveAspectFit
                }

                onClicked: {
                    recolorPage.editor.undo();
                }
            }

            ToolButton {
                implicitWidth:    UtilScript.dp(UIHelper.screenDpi, 48)
                implicitHeight:   UtilScript.dp(UIHelper.screenDpi, 48)
                Layout.alignment: Qt.AlignHCenter

                contentItem: Image {
                    source:   "qrc:/resources/images/tool_settings.png"
                    fillMode: Image.PreserveAspectFit
                }

                onClicked: {
                    brushSettingsPane.visible = !brushSettingsPane.visible;
                }
            }

            ToolButton {
                implicitWidth:    UtilScript.dp(UIHelper.screenDpi, 48)
                implicitHeight:   UtilScript.dp(UIHelper.screenDpi, 48)
                Layout.alignment: Qt.AlignHCenter

                contentItem: Image {
                    source:   "qrc:/resources/images/tool_help.png"
                    fillMode: Image.PreserveAspectFit
                }

                onClicked: {
                    Qt.openUrlExternally(qsTr("https://magicphotos.sourceforge.io/help/android/help.html"));
                }
            }
        }
    }

    readonly property bool allowInterstitial: true

    readonly property int bannerViewHeight:   AdMobHelper.bannerViewHeight

    readonly property var editor:             editorLoader.item

    property bool shareActionActive:          false

    property int imageOrientation:            -1

    property string imagePath:                ""

    property var editorComponent: Component {
        RecolorEditor {
        }
    }

    Keys.onReleased: {
        if (event.key === Qt.Key_Back) {
            if (brushSettingsPane.visible) {
                brushSettingsPane.visible = false;
            } else if (editor.changed) {
                backMessageDialog.open();
            } else {
                mainStackView.pop();
            }

            event.accepted = true;
        }
    }

    function updateEditorParameters() {
        editor.brushSize     = AppSettings.brushSize;
        editor.brushHardness = AppSettings.brushHardness;
    }

    Rectangle {
        id:           editorRectangle
        anchors.fill: parent
        color:        "transparent"

        PinchArea {
            id:           editorPinchArea
            anchors.fill: parent
            enabled:      scrollModeButton.checked

            onPinchUpdated: {
                var pinch_prev_center = mapToItem(editorFlickable.contentItem, pinch.previousCenter.x, pinch.previousCenter.y);
                var pinch_center      = mapToItem(editorFlickable.contentItem, pinch.center.x, pinch.center.y);
                var pinch_prev_scale  = pinch.previousScale;
                var pinch_scale       = pinch.scale;

                if (editorFlickable.initialContentWidth > 0.0) {
                    editorFlickable.contentX += pinch_prev_center.x - pinch_center.x;
                    editorFlickable.contentY += pinch_prev_center.y - pinch_center.y;

                    var scale = 1.0 + pinch_scale - pinch_prev_scale;

                    if (editorFlickable.contentWidth * scale / editorFlickable.initialContentWidth >= 0.5 &&
                        editorFlickable.contentWidth * scale / editorFlickable.initialContentWidth <= 4.0) {
                        editorFlickable.resizeContent(editorFlickable.contentWidth * scale, editorFlickable.contentHeight * scale, pinch_center);
                    }
                }
            }

            onPinchFinished: {
                editorFlickable.returnToBounds();
            }

            Flickable {
                id:             editorFlickable
                anchors.fill:   parent
                leftMargin:     width  > contentWidth  ? (width  - contentWidth)  / 2 : 0
                topMargin:      height > contentHeight ? (height - contentHeight) / 2 : 0
                boundsBehavior: Flickable.StopAtBounds
                interactive:    scrollModeButton.checked

                property real initialContentWidth:  0.0
                property real initialContentHeight: 0.0

                Loader {
                    id:              editorLoader
                    sourceComponent: recolorPage.editorComponent

                    onLoaded: {
                        item.scale           = Qt.binding(function() { return editorFlickable.contentWidth > 0.0 && editorFlickable.initialContentWidth > 0.0 ? editorFlickable.contentWidth / editorFlickable.initialContentWidth : 1.0; });
                        item.transformOrigin = Item.TopLeft;
                        item.mode            = Qt.binding(function() { return editorMode(scrollModeButton.checked, originalModeButton.checked, effectedModeButton.checked, hueSelectionModeButton.checked); });
                        item.helperSize      = Qt.binding(function() { return helper.width; });
                        item.hue             = 180;

                        recolorPage.updateEditorParameters();
                    }

                    function editorMode(scroll_mode, original_mode, effected_mode, hue_selection_mode) {
                        if (scroll_mode) {
                            return EffectEditor.ModeScroll;
                        } else if (original_mode) {
                            return EffectEditor.ModeOriginal;
                        } else if (effected_mode) {
                            return EffectEditor.ModeEffected;
                        } else if (hue_selection_mode) {
                            return EffectEditor.ModeEffected;
                        } else {
                            return EffectEditor.ModeScroll;
                        }
                    }
                }
            }
        }

        Rectangle {
            id:           helperRectangle
            anchors.top:  parent.top
            anchors.left: parent.left
            z:            1
            width:        UtilScript.dp(UIHelper.screenDpi, 128)
            height:       UtilScript.dp(UIHelper.screenDpi, 128)
            color:        "transparent"
            visible:      false

            OpacityMask {
                id:           opacityMask
                anchors.fill: parent

                source: Helper {
                    id:      helper
                    width:   opacityMask.width
                    height:  opacityMask.height
                    visible: false
                }

                maskSource: Image {
                    width:    opacityMask.width
                    height:   opacityMask.height
                    source:   "qrc:/resources/images/helper_mask.png"
                    fillMode: Image.PreserveAspectFit
                    visible:  false
                }
            }
        }

        Rectangle {
            id:           waitRectangle
            anchors.fill: parent
            z:            2
            color:        "black"
            opacity:      0.75

            BusyIndicator {
                anchors.centerIn: parent
                running:          parent.visible
            }

            MultiPointTouchArea {
                anchors.fill: parent
            }
        }
    }

    Rectangle {
        id:                     hueZoneRectangle
        anchors.right:          parent.right
        anchors.verticalCenter: parent.verticalCenter
        z:                      1
        width:                  UtilScript.dp(UIHelper.screenDpi, 48)
        height:                 parent.height * 3 / 4
        color:                  "transparent"
        border.color:           "black"
        border.width:           UtilScript.dp(UIHelper.screenDpi, 1)
        visible:                hueSelectionModeButton.checked

        gradient: Gradient {
            GradientStop { position: 1.0;  color: "#FF0000" }
            GradientStop { position: 0.85; color: "#FFFF00" }
            GradientStop { position: 0.76; color: "#00FF00" }
            GradientStop { position: 0.5;  color: "#00FFFF" }
            GradientStop { position: 0.33; color: "#0000FF" }
            GradientStop { position: 0.16; color: "#FF00FF" }
            GradientStop { position: 0.0;  color: "#FF0000" }
        }

        MouseArea {
            anchors.fill: parent

            Rectangle {
                id:            hueSliderRectangle
                anchors.left:  parent.left
                anchors.right: parent.right
                y:             Math.max(0, Math.min(hueZoneRectangle.height - height, hueZoneRectangle.height - recolorPage.editor.hue / (359 / hueZoneRectangle.height)));
                height:        UtilScript.dp(UIHelper.screenDpi, 6)
                color:         "transparent"
                border.color:  "black"
                border.width:  UtilScript.dp(UIHelper.screenDpi, 2)
            }

            onPressed: {
                recolorPage.editor.hue = (height - Math.max(0, Math.min(height, mouse.y))) * (359 / height);
            }

            onPositionChanged: {
                recolorPage.editor.hue = (height - Math.max(0, Math.min(height, mouse.y))) * (359 / height);
            }
        }
    }

    Pane {
        id:                 brushSettingsPane
        anchors.bottom:     parent.bottom
        anchors.left:       parent.left
        anchors.right:      parent.right
        z:                  2
        visible:            false
        Material.elevation: 5

        onVisibleChanged: {
            if (visible) {
                brushSizeSlider.value     = AppSettings.brushSize;
                brushHardnessSlider.value = AppSettings.brushHardness;
            }
        }

        GridLayout {
            anchors.fill:    parent
            anchors.margins: UtilScript.dp(UIHelper.screenDpi, 4)
            rows:            2
            columns:         3
            rowSpacing:      UtilScript.dp(UIHelper.screenDpi, 8)
            columnSpacing:   UtilScript.dp(UIHelper.screenDpi, 4)

            BrushPreviewGenerator {
                id:             brushPreviewGenerator
                brushSize:      brushSizeSlider.value
                maxBrushSize:   brushSizeSlider.to
                brushHardness:  brushHardnessSlider.value
                Layout.rowSpan: 2
            }

            Slider {
                id:               brushSizeSlider
                from:             UtilScript.dp(UIHelper.screenDpi, 8)
                to:               UtilScript.dp(UIHelper.screenDpi, 24)
                value:            UtilScript.dp(UIHelper.screenDpi, 16)
                stepSize:         1.0
                Layout.fillWidth: true

                onPressedChanged: {
                    if (!pressed) {
                        AppSettings.brushSize = value;

                        recolorPage.updateEditorParameters();
                    }
                }
            }

            Label {
                id:             brushSizeLabel
                text:           qsTr("Brush Size")
                font.pixelSize: UtilScript.dp(UIHelper.screenDpi, 16)
                font.family:    "Helvetica"
            }

            Slider {
                id:               brushHardnessSlider
                from:             0.0
                to:               1.0
                value:            0.75
                stepSize:         0.1
                Layout.fillWidth: true

                onPressedChanged: {
                    if (!pressed) {
                        AppSettings.brushHardness = value;

                        recolorPage.updateEditorParameters();
                    }
                }
            }

            Label {
                id:             brushHardnessLabel
                text:           qsTr("Brush Hardness")
                font.pixelSize: UtilScript.dp(UIHelper.screenDpi, 16)
                font.family:    "Helvetica"
            }
        }
    }

    MessageDialog {
        id:              imageOpenFailedMessageDialog
        title:           qsTr("Error")
        text:            qsTr("Could not open image")
        standardButtons: StandardButton.Ok
    }

    MessageDialog {
        id:              imageSavedMessageDialog
        title:           qsTr("Info")
        text:            qsTr("Image saved successfully")
        standardButtons: StandardButton.Ok
    }

    MessageDialog {
        id:              imageSaveFailedMessageDialog
        title:           qsTr("Error")
        text:            qsTr("Could not save image")
        standardButtons: StandardButton.Ok
    }

    MessageDialog {
        id:              backMessageDialog
        title:           qsTr("Warning")
        text:            qsTr("Are you sure? Current image is not saved and will be lost.")
        standardButtons: StandardButton.Yes | StandardButton.No

        onYes: {
            mainStackView.pop();
        }
    }

    Connections {
        target: recolorPage.editor

        onImageOpened: {
            waitRectangle.visible = false;

            headerRow.enabled       = true;
            saveToolButton.enabled  = true;
            shareToolButton.enabled = true;

            editorFlickable.contentWidth         = recolorPage.editor.width;
            editorFlickable.contentHeight        = recolorPage.editor.height;
            editorFlickable.initialContentWidth  = recolorPage.editor.width;
            editorFlickable.initialContentHeight = recolorPage.editor.height;
        }

        onImageOpenFailed: {
            waitRectangle.visible = false;

            headerRow.enabled       = false;
            saveToolButton.enabled  = false;
            shareToolButton.enabled = false;

            imageOpenFailedMessageDialog.open();
        }

        onImageSaved: {
            if (recolorPage.shareActionActive) {
                UIHelper.shareImage(imagePath);
            } else if (UIHelper.requestWriteStoragePermission() &&
                       MediaStoreHelper.addImageToMediaStore(imagePath)) {
                imageSavedMessageDialog.open();
            } else {
                imageSaveFailedMessageDialog.open();
            }
        }

        onImageSaveFailed: {
            imageSaveFailedMessageDialog.open();
        }

        onUndoAvailabilityUpdated: {
            if (available) {
                undoToolButton.enabled = true;
            } else {
                undoToolButton.enabled = false;
            }
        }

        onMouseEvent: {
            var rect = recolorPage.editor.mapToItem(editorRectangle, x, y);

            if (eventType === Editor.MousePressed ||
                eventType === Editor.MouseMoved) {
                helperRectangle.visible = true;

                if (rect.y < editorRectangle.height / 2) {
                    if (rect.x < editorRectangle.width / 2) {
                        helperRectangle.anchors.left  = undefined;
                        helperRectangle.anchors.right = editorRectangle.right;
                    } else {
                        helperRectangle.anchors.right = undefined;
                        helperRectangle.anchors.left  = editorRectangle.left;
                    }
                }
            } else if (eventType === Editor.MouseReleased) {
                helperRectangle.visible = false;
            }
        }

        onHelperImageReady: {
            helper.setHelperImage(helperImage);
        }
    }

    Component.onCompleted: {
        if (imageOrientation !== -1 && imagePath !== "") {
            editor.openImage(imagePath, imageOrientation);
        }
    }
}
