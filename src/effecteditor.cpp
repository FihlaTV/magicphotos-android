#include <QtCore/QtMath>
#include <QtCore/QPoint>
#include <QtCore/QRect>
#include <QtCore/QThread>
#include <QtGui/QColor>
#include <QtGui/QPainter>

#include "effecteditor.h"

EffectEditor::EffectEditor(QQuickItem *parent) :
    Editor(parent)
{
}

void EffectEditor::setEffectedImage(const QImage &image)
{
    OriginalImage = LoadedImage;
    EffectedImage = image;
    CurrentImage  = EffectedImage;

    LoadedImage = QImage();

    Changed = true;

    setImplicitWidth(CurrentImage.width());
    setImplicitHeight(CurrentImage.height());

    update();

    emit scaleChanged();
    emit imageOpened();
}

void EffectEditor::mousePressEvent(QMouseEvent *event)
{
    if (Mode == ModeOriginal || Mode == ModeEffected) {
        ChangeImageAt(true, event->pos().x(), event->pos().y());

        emit mouseEvent(MousePressed, event->pos().x(), event->pos().y());
    }
}

void EffectEditor::mouseMoveEvent(QMouseEvent *event)
{
    if (Mode == ModeOriginal || Mode == ModeEffected) {
        ChangeImageAt(false, event->pos().x(), event->pos().y());

        emit mouseEvent(MouseMoved, event->pos().x(), event->pos().y());
    }
}

void EffectEditor::mouseReleaseEvent(QMouseEvent *event)
{
    if (Mode == ModeOriginal || Mode == ModeEffected) {
        emit mouseEvent(MouseReleased, event->pos().x(), event->pos().y());
    }
}

void EffectEditor::ChangeImageAt(bool save_undo, int center_x, int center_y)
{
    if (Mode != ModeScroll) {
        if (save_undo) {
            SaveUndoImage();
        }

        int width  = BrushImage.width();
        int height = BrushImage.height();

        int img_x = center_x - width  / 2;
        int img_y = center_y - height / 2;

        QImage brush_image(width, height, QImage::Format_ARGB32);

        brush_image.fill(qRgba(0, 0, 0, 0));

        QPainter brush_painter(&brush_image);

        brush_painter.setCompositionMode(QPainter::CompositionMode_Source);

        if (Mode == ModeOriginal) {
            brush_painter.drawImage(QPoint(0, 0), OriginalImage, QRect(img_x, img_y, width, height));
        } else {
            brush_painter.drawImage(QPoint(0, 0), EffectedImage, QRect(img_x, img_y, width, height));
        }

        brush_painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        brush_painter.drawImage(QPoint(0, 0), BrushImage);

        QPainter image_painter(&CurrentImage);

        image_painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        image_painter.drawImage(QPoint(img_x, img_y), brush_image);

        Changed = true;

        update();

        if (qFloor(HelperSize / scale()) > 0) {
            QImage helper_image = CurrentImage.copy(center_x - qFloor((HelperSize / scale()) / 2),
                                                    center_y - qFloor((HelperSize / scale()) / 2),
                                                    qFloor(HelperSize / scale()),
                                                    qFloor(HelperSize / scale())).scaledToWidth(HelperSize);

            emit helperImageReady(helper_image);
        } else {
            emit helperImageReady(QImage());
        }
    }
}
