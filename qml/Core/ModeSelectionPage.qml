import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.12

import "../Util.js" as UtilScript

Page {
    id: modeSelectionPage

    header: Pane {
        Material.background: Material.Red

        Label {
            anchors.centerIn: parent
            text:             qsTr("MagicPhotos")
            font.pointSize:   24
        }
    }

    footer: ToolBar {
        RowLayout {
            anchors.fill: parent

            ToolButton {
                implicitWidth:    UtilScript.pt(48)
                implicitHeight:   UtilScript.pt(48)
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

    property string selectedMode: ""

    onSelectedModeChanged: {
        if (selectedMode !== "") {
            if (UIHelper.requestReadStoragePermission()) {
                waitRectangle.visible = true;

                UIHelper.showGallery();
            }
        }
    }

    function fileSelected(image_file, image_orientation) {
        waitRectangle.visible = false;

        var component;

        if (selectedMode === "DECOLORIZE"){
            component = Qt.createComponent("DecolorizePage.qml");

            if (component.status === Component.Ready) {
                mainStackView.push(component, {imageOrientation: image_orientation, imageFile: image_file});
            } else {
                console.log(component.errorString());
            }
        } else if (selectedMode === "SKETCH") {
            component = Qt.createComponent("SketchPreviewPage.qml");

            if (component.status === Component.Ready) {
                mainStackView.push(component, {imageOrientation: image_orientation, imageFile: image_file});
            } else {
                console.log(component.errorString());
            }
        } else if (selectedMode === "CARTOON") {
            component = Qt.createComponent("CartoonPreviewPage.qml");

            if (component.status === Component.Ready) {
                mainStackView.push(component, {imageOrientation: image_orientation, imageFile: image_file});
            } else {
                console.log(component.errorString());
            }
        } else if (selectedMode === "BLUR") {
            component = Qt.createComponent("BlurPreviewPage.qml");

            if (component.status === Component.Ready) {
                mainStackView.push(component, {imageOrientation: image_orientation, imageFile: image_file});
            } else {
                console.log(component.errorString());
            }
        } else if (selectedMode === "PIXELATE") {
            component = Qt.createComponent("PixelatePreviewPage.qml");

            if (component.status === Component.Ready) {
                mainStackView.push(component, {imageOrientation: image_orientation, imageFile: image_file});
            } else {
                console.log(component.errorString());
            }
        } else if (selectedMode === "RECOLOR") {
            component = Qt.createComponent("RecolorPage.qml");

            if (component.status === Component.Ready) {
                mainStackView.push(component, {imageOrientation: image_orientation, imageFile: image_file});
            } else {
                console.log(component.errorString());
            }
        } else if (selectedMode === "RETOUCH") {
            component = Qt.createComponent("RetouchPage.qml");

            if (component.status === Component.Ready) {
                mainStackView.push(component, {imageOrientation: image_orientation, imageFile: image_file});
            } else {
                console.log(component.errorString());
            }
        }

        selectedMode = "";
    }

    function fileSelectionCancelled() {
        waitRectangle.visible = false;

        selectedMode = "";
    }

    function fileSelectionFailed() {
        waitRectangle.visible = false;

        selectedMode = "";

        imageSelectionFailedMessageDialog.open();
    }

    ScrollView {
        id:            modeSelectionScrollView
        anchors.fill:  parent
        contentWidth:  modeSelectionLayout.width
        contentHeight: modeSelectionLayout.height
        clip:          true

        GridLayout {
            id:            modeSelectionLayout
            width:         modeSelectionScrollView.width
            columns:       2
            rowSpacing:    UtilScript.pt(4)
            columnSpacing: UtilScript.pt(4)

            property int itemSize: (width - columnSpacing * (columns - 1)) / columns

            Image {
                source:                 "qrc:/resources/images/edit_mode_decolorize.png"
                fillMode:               Image.PreserveAspectFit
                Layout.preferredWidth:  parent.itemSize
                Layout.preferredHeight: parent.itemSize

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        modeSelectionPage.selectedMode = "DECOLORIZE";
                    }
                }
            }

            Image {
                source:                 "qrc:/resources/images/edit_mode_sketch.png"
                fillMode:               Image.PreserveAspectFit
                Layout.preferredWidth:  parent.itemSize
                Layout.preferredHeight: parent.itemSize

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        modeSelectionPage.selectedMode = "SKETCH";
                    }
                }
            }

            Image {
                source:                 "qrc:/resources/images/edit_mode_cartoon.png"
                fillMode:               Image.PreserveAspectFit
                Layout.preferredWidth:  parent.itemSize
                Layout.preferredHeight: parent.itemSize

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        modeSelectionPage.selectedMode = "CARTOON";
                    }
                }
            }

            Image {
                source:                 "qrc:/resources/images/edit_mode_blur.png"
                fillMode:               Image.PreserveAspectFit
                Layout.preferredWidth:  parent.itemSize
                Layout.preferredHeight: parent.itemSize

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        modeSelectionPage.selectedMode = "BLUR";
                    }
                }
            }

            Image {
                source:                 "qrc:/resources/images/edit_mode_pixelate.png"
                fillMode:               Image.PreserveAspectFit
                Layout.preferredWidth:  parent.itemSize
                Layout.preferredHeight: parent.itemSize

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        modeSelectionPage.selectedMode = "PIXELATE";
                    }
                }
            }

            Image {
                source:                 "qrc:/resources/images/edit_mode_recolor.png"
                fillMode:               Image.PreserveAspectFit
                Layout.preferredWidth:  parent.itemSize
                Layout.preferredHeight: parent.itemSize

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        modeSelectionPage.selectedMode = "RECOLOR";
                    }
                }
            }

            Image {
                source:                 "qrc:/resources/images/edit_mode_retouch.png"
                fillMode:               Image.PreserveAspectFit
                Layout.preferredWidth:  parent.itemSize
                Layout.preferredHeight: parent.itemSize

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        modeSelectionPage.selectedMode = "RETOUCH";
                    }
                }
            }
        }
    }

    Rectangle {
        id:           waitRectangle
        anchors.fill: parent
        z:            10
        color:        "black"
        opacity:      0.75
        visible:      false

        BusyIndicator {
            anchors.centerIn: parent
            running:          parent.visible
        }

        MouseArea {
            anchors.fill: parent
        }
    }

    MessageDialog {
        id:              imageSelectionFailedMessageDialog
        title:           qsTr("Error")
        text:            qsTr("Could not open image")
        standardButtons: StandardButton.Ok
    }

    Component.onCompleted: {
        UIHelper.imageSelected.connect(fileSelected);
        UIHelper.imageSelectionCancelled.connect(fileSelectionCancelled);
        UIHelper.imageSelectionFailed.connect(fileSelectionFailed);
    }
}
