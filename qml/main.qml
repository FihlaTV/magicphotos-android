import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3

import "Util.js" as UtilScript

ApplicationWindow {
    id:                           mainWindow
    title:                        qsTr("MagicPhotos")
    visible:                      true
    Screen.orientationUpdateMask: Qt.PortraitOrientation         | Qt.LandscapeOrientation |
                                  Qt.InvertedPortraitOrientation | Qt.InvertedLandscapeOrientation
    Material.theme:               Material.System
    Material.primary:             Material.Teal

    readonly property int screenOrientation: Screen.orientation

    onScreenOrientationChanged: {
        if (mainStackView.depth > 0 && typeof mainStackView.currentItem.bannerViewHeight === "number") {
            AdMobHelper.showBannerView();
        } else {
            AdMobHelper.hideBannerView();
        }
    }

    StackView {
        id:           mainStackView
        anchors.fill: parent

        onCurrentItemChanged: {
            for (var i = 0; i < depth; i++) {
                var item = get(i, StackView.DontLoad);

                if (item !== null) {
                    item.focus = false;
                }
            }

            if (depth > 0) {
                currentItem.forceActiveFocus();

                if (typeof currentItem.bannerViewHeight === "number") {
                    AdMobHelper.showBannerView();
                } else {
                    AdMobHelper.hideBannerView();
                }

                if (currentItem.allowInterstitial) {
                    AdMobHelper.showInterstitial();
                }
            } else {
                AdMobHelper.hideBannerView();
            }
        }
    }

    MultiPointTouchArea {
        anchors.fill: parent
        z:            1
        enabled:      mainStackView.busy
    }

    Component.onCompleted: {
        AppSettings.defaultBrushSize = UtilScript.pt(16);

        var component = Qt.createComponent("Core/ModeSelectionPage.qml");

        if (component.status === Component.Ready) {
            mainStackView.push(component);
        } else {
            console.log(component.errorString());
        }
    }
}
