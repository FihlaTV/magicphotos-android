#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtGui/QColor>
#include <QtGui/QTransform>
#include <QtGui/QImageReader>
#include <QtGui/QPainter>

#include <libexif/exif-loader.h>
#include <libexif/exif-data.h>
#include <libexif/exif-entry.h>

#include "recoloreditor.h"

RecolorEditor::RecolorEditor() : bb::cascades::CustomControl()
{
    IsChanged    = false;
    CurrentMode  = ModeScroll;
    BrushSize    = 0;
    HelperSize   = 0;
    CurrentHue   = 0;
    BrushOpacity = 0.0;
    Scale        = 1.0;

    RGB16  rgb16;
    HSV    hsv;
    QRgb   rgb;
    QColor color;

    for (int i = 0; i < 65536; i++) {
        rgb16.rgb = i;

        rgb = qRgb(rgb16.r << 3, rgb16.g << 2, rgb16.b << 3);

        color.setRgb(rgb);

        hsv.h = color.hue();
        hsv.s = color.saturation();
        hsv.v = color.value();

        RGB16ToHSVMap[rgb16.rgb] = hsv.hsv;
    }
}

RecolorEditor::~RecolorEditor()
{
}

int RecolorEditor::mode() const
{
    return CurrentMode;
}

void RecolorEditor::setMode(const int &mode)
{
    CurrentMode = mode;
}

int RecolorEditor::brushSize() const
{
    return BrushSize;
}

void RecolorEditor::setBrushSize(const int &size)
{
    BrushSize = size;

    BrushTemplateImage = QImage(BrushSize * 2, BrushSize * 2, QImage::Format_ARGB32);

    for (int x = 0; x < BrushTemplateImage.width(); x++) {
        for (int y = 0; y < BrushTemplateImage.height(); y++) {
            qreal r = qSqrt(qPow(x - BrushSize, 2) + qPow(y - BrushSize, 2));

            if (r <= BrushSize) {
                if (r <= BrushSize * BrushOpacity) {
                    BrushTemplateImage.setPixel(x, y, qRgba(0xFF, 0xFF, 0xFF, 0xFF));
                } else {
                    BrushTemplateImage.setPixel(x, y, qRgba(0xFF, 0xFF, 0xFF, (int)(0xFF * (BrushSize - r) / (BrushSize * (1.0 - BrushOpacity)))));
                }
            } else {
                BrushTemplateImage.setPixel(x, y, qRgba(0xFF, 0xFF, 0xFF, 0x00));
            }
        }
    }

    int brush_width = qMax(1, qMin(qMin((int)(BrushSize / Scale) * 2, CurrentImage.width()), CurrentImage.height()));

    BrushImage = BrushTemplateImage.scaledToWidth(brush_width);
}

int RecolorEditor::helperSize() const
{
    return HelperSize;
}

void RecolorEditor::setHelperSize(const int &size)
{
    HelperSize = size;
}

int RecolorEditor::hue() const
{
    return CurrentHue;
}

void RecolorEditor::setHue(const int &hue)
{
    CurrentHue = hue;
}

qreal RecolorEditor::brushOpacity() const
{
    return BrushOpacity;
}

void RecolorEditor::setBrushOpacity(const qreal &opacity)
{
    BrushOpacity = opacity;

    BrushTemplateImage = QImage(BrushSize * 2, BrushSize * 2, QImage::Format_ARGB32);

    for (int x = 0; x < BrushTemplateImage.width(); x++) {
        for (int y = 0; y < BrushTemplateImage.height(); y++) {
            qreal r = qSqrt(qPow(x - BrushSize, 2) + qPow(y - BrushSize, 2));

            if (r <= BrushSize) {
                if (r <= BrushSize * BrushOpacity) {
                    BrushTemplateImage.setPixel(x, y, qRgba(0xFF, 0xFF, 0xFF, 0xFF));
                } else {
                    BrushTemplateImage.setPixel(x, y, qRgba(0xFF, 0xFF, 0xFF, (int)(0xFF * (BrushSize - r) / (BrushSize * (1.0 - BrushOpacity)))));
                }
            } else {
                BrushTemplateImage.setPixel(x, y, qRgba(0xFF, 0xFF, 0xFF, 0x00));
            }
        }
    }

    int brush_width = qMax(1, qMin(qMin((int)(BrushSize / Scale) * 2, CurrentImage.width()), CurrentImage.height()));

    BrushImage = BrushTemplateImage.scaledToWidth(brush_width);
}

qreal RecolorEditor::scale() const
{
    return Scale;
}

void RecolorEditor::setScale(const qreal &scale)
{
    Scale = scale;

    int brush_width = qMax(1, qMin(qMin((int)(BrushSize / Scale) * 2, CurrentImage.width()), CurrentImage.height()));

    BrushImage = BrushTemplateImage.scaledToWidth(brush_width);
}

bool RecolorEditor::changed() const
{
    return IsChanged;
}

void RecolorEditor::openImage(const QString &image_file)
{
    int         img_orientation = 0;
    ExifLoader *exif_loader     = exif_loader_new();

    if (exif_loader != NULL) {
        ExifData *exif_data;

        exif_loader_write_file(exif_loader, image_file.toUtf8().data());

        exif_data = exif_loader_get_data(exif_loader);

        if (exif_data != NULL) {
            ExifByteOrder exif_bo    = exif_data_get_byte_order(exif_data);
            ExifEntry    *exif_entry = exif_content_get_entry(exif_data->ifd[EXIF_IFD_0], EXIF_TAG_ORIENTATION);

            if (exif_entry != NULL) {
                img_orientation = exif_get_short(exif_entry->data, exif_bo);

                exif_entry_unref(exif_entry);
            }

            exif_data_unref(exif_data);
        }

        exif_loader_unref(exif_loader);
    }

    QImageReader reader(image_file);

    if (reader.canRead()) {
        QSize size = reader.size();

        if (size.width() * size.height() > IMAGE_MPIX_LIMIT * 1000000.0) {
            qreal factor = qSqrt((size.width() * size.height()) / (IMAGE_MPIX_LIMIT * 1000000.0));

            size.setWidth(size.width()   / factor);
            size.setHeight(size.height() / factor);

            reader.setScaledSize(size);
        }

        LoadedImage = reader.read();

        if (!LoadedImage.isNull()) {
            if (img_orientation == 3) {
                QTransform transform;

                transform.rotate(180);

                LoadedImage = LoadedImage.transformed(transform).scaled(LoadedImage.width(), LoadedImage.height());
            } else if (img_orientation == 6) {
                QTransform transform;

                transform.rotate(90);

                LoadedImage = LoadedImage.transformed(transform).scaled(LoadedImage.height(), LoadedImage.width());
            } else if (img_orientation == 8) {
                QTransform transform;

                transform.rotate(270);

                LoadedImage = LoadedImage.transformed(transform).scaled(LoadedImage.height(), LoadedImage.width());
            }

            LoadedImage = LoadedImage.convertToFormat(QImage::Format_RGB16);

            if (!LoadedImage.isNull()) {
                OriginalImage = LoadedImage;
                CurrentImage  = LoadedImage;

                LoadedImage = QImage();

                UndoStack.clear();

                IsChanged = false;

                RepaintImage(true);

                int brush_width = qMax(1, qMin(qMin((int)(BrushSize / Scale) * 2, CurrentImage.width()), CurrentImage.height()));

                BrushImage = BrushTemplateImage.scaledToWidth(brush_width);

                emit undoAvailabilityChanged(false);
                emit imageOpened();
            } else {
                emit imageOpenFailed();
            }
        } else {
            emit imageOpenFailed();
        }
    } else {
        emit imageOpenFailed();
    }
}

void RecolorEditor::saveImage(const QString &image_file)
{
    QString file_name = image_file;

    if (!CurrentImage.isNull()) {
        if (QFileInfo(file_name).suffix().compare("png", Qt::CaseInsensitive) != 0 &&
            QFileInfo(file_name).suffix().compare("jpg", Qt::CaseInsensitive) != 0 &&
            QFileInfo(file_name).suffix().compare("bmp", Qt::CaseInsensitive) != 0) {
            file_name = file_name + ".jpg";
        }

        if (CurrentImage.convertToFormat(QImage::Format_ARGB32).save(file_name)) {
            IsChanged = false;

            emit imageSaved();
        } else {
            emit imageSaveFailed();
        }
    } else {
        emit imageSaveFailed();
    }
}

void RecolorEditor::changeImageAt(bool save_undo, int center_x, int center_y)
{
    if (CurrentMode == ModeOriginal || CurrentMode == ModeEffected) {
        if (save_undo) {
            SaveUndoImage();
        }

        int width  = qMin(BrushImage.width(),  CurrentImage.width());
        int height = qMin(BrushImage.height(), CurrentImage.height());

        int img_x = qMin(qMax(0, center_x - width  / 2), CurrentImage.width()  - width);
        int img_y = qMin(qMax(0, center_y - height / 2), CurrentImage.height() - height);

        QImage   brush_image(width, height, QImage::Format_ARGB32);
        QPainter brush_painter(&brush_image);

        if (CurrentMode == ModeOriginal) {
            brush_painter.setCompositionMode(QPainter::CompositionMode_Source);
            brush_painter.drawImage(QPoint(0, 0), OriginalImage, QRect(img_x, img_y, width, height));
        } else if (CurrentMode == ModeEffected) {
            for (int x = img_x; x < img_x + width; x++) {
                for (int y = img_y; y < img_y + height; y++) {
                    brush_image.setPixel(x - img_x, y - img_y, AdjustHue(OriginalImage.pixel(x, y)));
                }
            }
        }

        QPainter image_painter(&CurrentImage);

        brush_painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        brush_painter.drawImage(QPoint(0, 0), BrushImage);

        image_painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        image_painter.drawImage(QPoint(img_x, img_y), brush_image);

        IsChanged = true;

        RepaintImage(false, QRect(img_x, img_y, width, height));
        RepaintHelper(center_x, center_y);
    }
}

void RecolorEditor::undo()
{
    if (UndoStack.size() > 0) {
        CurrentImage = UndoStack.pop();

        if (UndoStack.size() == 0) {
            emit undoAvailabilityChanged(false);
        }

        IsChanged = true;

        RepaintImage(true);
    }
}

void RecolorEditor::SaveUndoImage()
{
    UndoStack.push(CurrentImage);

    if (UndoStack.size() > UNDO_DEPTH) {
        for (int i = 0; i < UndoStack.size() - UNDO_DEPTH; i++) {
            UndoStack.remove(0);
        }
    }

    emit undoAvailabilityChanged(true);
}

QRgb RecolorEditor::AdjustHue(QRgb rgb)
{
    RGB16 rgb16;
    HSV   hsv;

    rgb16.r = (qRed(rgb)   & 0xf8) >> 3;
    rgb16.g = (qGreen(rgb) & 0xfc) >> 2;
    rgb16.b = (qBlue(rgb)  & 0xf8) >> 3;

    hsv.hsv = RGB16ToHSVMap[rgb16.rgb];

    return QColor::fromHsv(CurrentHue, hsv.s, hsv.v, qAlpha(rgb)).rgba();
}

void RecolorEditor::RepaintImage(bool full, QRect rect)
{
    if (CurrentImage.isNull()) {
        CurrentImageData = bb::ImageData();

        emit needImageRepaint(bb::cascades::Image());
    } else if (full) {
        CurrentImageData = bb::ImageData(bb::PixelFormat::RGBA_Premultiplied, CurrentImage.width(), CurrentImage.height());

        unsigned char *dst_line = CurrentImageData.pixels();

        for (int y = 0; y < CurrentImageData.height(); y++) {
            unsigned char *dst = dst_line;

            for (int x = 0; x < CurrentImageData.width(); x++) {
                QRgb pixel = CurrentImage.pixel(x, y);

                *dst++ = qRed(pixel);
                *dst++ = qGreen(pixel);
                *dst++ = qBlue(pixel);
                *dst++ = qAlpha(pixel);
            }

            dst_line += CurrentImageData.bytesPerLine();
        }

        emit needImageRepaint(bb::cascades::Image(CurrentImageData));
    } else {
        unsigned char *dst_line = CurrentImageData.pixels();

        if (rect.x() >= CurrentImageData.width()) {
            rect.setX(CurrentImageData.width() - 1);
        }
        if (rect.y() >= CurrentImageData.height()) {
            rect.setY(CurrentImageData.height() - 1);
        }
        if (rect.x() < 0) {
            rect.setX(0);
        }
        if (rect.y() < 0) {
            rect.setY(0);
        }
        if (rect.x() + rect.width() > CurrentImageData.width()) {
            rect.setWidth(CurrentImageData.width() - rect.x());
        }
        if (rect.y() + rect.height() > CurrentImageData.height()) {
            rect.setHeight(CurrentImageData.height() - rect.y());
        }

        dst_line += rect.y() * CurrentImageData.bytesPerLine();

        for (int y = rect.y(); y < rect.y() + rect.height(); y++) {
            unsigned char *dst = dst_line;

            dst += rect.x() * 4;

            for (int x = rect.x(); x < rect.x() + rect.width(); x++) {
                QRgb pixel = CurrentImage.pixel(x, y);

                *dst++ = qRed(pixel);
                *dst++ = qGreen(pixel);
                *dst++ = qBlue(pixel);
                *dst++ = qAlpha(pixel);
            }

            dst_line += CurrentImageData.bytesPerLine();
        }

        emit needImageRepaint(bb::cascades::Image(CurrentImageData));
    }
}

void RecolorEditor::RepaintHelper(int center_x, int center_y)
{
    if (CurrentImage.isNull()) {
        emit needHelperRepaint(bb::cascades::Image());
    } else {
        QImage   helper_image = CurrentImage.copy(center_x - HelperSize / (Scale * 2),
                                                  center_y - HelperSize / (Scale * 2), HelperSize / Scale, HelperSize / Scale).scaledToWidth(HelperSize);
        QPainter painter(&helper_image);

        painter.setPen(QPen(Qt::white, 4, Qt::SolidLine));
        painter.drawPoint(helper_image.rect().center());

        bb::ImageData helper_image_data = bb::ImageData(bb::PixelFormat::RGBA_Premultiplied, helper_image.width(), helper_image.height());

        unsigned char *dst_line = helper_image_data.pixels();

        for (int y = 0; y < helper_image_data.height(); y++) {
            unsigned char *dst = dst_line;

            for (int x = 0; x < helper_image_data.width(); x++) {
                QRgb pixel = helper_image.pixel(x, y);

                *dst++ = qRed(pixel);
                *dst++ = qGreen(pixel);
                *dst++ = qBlue(pixel);
                *dst++ = qAlpha(pixel);
            }

            dst_line += helper_image_data.bytesPerLine();
        }

        emit needHelperRepaint(bb::cascades::Image(helper_image_data));
    }
}
