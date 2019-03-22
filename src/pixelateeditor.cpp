#include <QtCore/QtMath>
#include <QtCore/QFileInfo>
#include <QtCore/QThread>
#include <QtGui/QTransform>
#include <QtGui/QImageReader>
#include <QtGui/QPainter>

#include "pixelateeditor.h"

PixelateEditor::PixelateEditor(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    IsChanged    = false;
    CurrentMode  = ModeScroll;
    BrushSize    = 0;
    HelperSize   = 0;
    PixelDenom   = 0;
    BrushOpacity = 0.0;

    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton | Qt::MiddleButton);

    setFlag(QQuickItem::ItemHasContents, true);

    QObject::connect(this, &PixelateEditor::scaleChanged, this, &PixelateEditor::scaleWasChanged);
}

bool PixelateEditor::changed() const
{
    return IsChanged;
}

int PixelateEditor::mode() const
{
    return CurrentMode;
}

void PixelateEditor::setMode(int mode)
{
    CurrentMode = mode;
}

int PixelateEditor::brushSize() const
{
    return BrushSize;
}

void PixelateEditor::setBrushSize(int size)
{
    BrushSize = size;

    BrushTemplateImage = QImage(BrushSize * 2, BrushSize * 2, QImage::Format_ARGB32);

    for (int y = 0; y < BrushTemplateImage.height(); y++) {
        for (int x = 0; x < BrushTemplateImage.width(); x++) {
            qreal r = qSqrt(qPow(x - BrushSize, 2) + qPow(y - BrushSize, 2));

            if (r <= BrushSize) {
                if (r <= BrushSize * BrushOpacity) {
                    BrushTemplateImage.setPixel(x, y, qRgba(0xFF, 0xFF, 0xFF, 0xFF));
                } else {
                    BrushTemplateImage.setPixel(x, y, qRgba(0xFF, 0xFF, 0xFF, qFloor(0xFF * (BrushSize - r) / (BrushSize * (1.0 - BrushOpacity)))));
                }
            } else {
                BrushTemplateImage.setPixel(x, y, qRgba(0xFF, 0xFF, 0xFF, 0x00));
            }
        }
    }

    int brush_width = qMax(1, qMin(qMin(qFloor(BrushSize / scale()) * 2, CurrentImage.width()), CurrentImage.height()));

    BrushImage = BrushTemplateImage.scaledToWidth(brush_width);
}

int PixelateEditor::helperSize() const
{
    return HelperSize;
}

void PixelateEditor::setHelperSize(int size)
{
    HelperSize = size;
}

int PixelateEditor::pixDenom() const
{
    return PixelDenom;
}

void PixelateEditor::setPixDenom(int pix_denom)
{
    PixelDenom = pix_denom;
}

qreal PixelateEditor::brushOpacity() const
{
    return BrushOpacity;
}

void PixelateEditor::setBrushOpacity(qreal opacity)
{
    BrushOpacity = opacity;

    BrushTemplateImage = QImage(BrushSize * 2, BrushSize * 2, QImage::Format_ARGB32);

    for (int y = 0; y < BrushTemplateImage.height(); y++) {
        for (int x = 0; x < BrushTemplateImage.width(); x++) {
            qreal r = qSqrt(qPow(x - BrushSize, 2) + qPow(y - BrushSize, 2));

            if (r <= BrushSize) {
                if (r <= BrushSize * BrushOpacity) {
                    BrushTemplateImage.setPixel(x, y, qRgba(0xFF, 0xFF, 0xFF, 0xFF));
                } else {
                    BrushTemplateImage.setPixel(x, y, qRgba(0xFF, 0xFF, 0xFF, qFloor(0xFF * (BrushSize - r) / (BrushSize * (1.0 - BrushOpacity)))));
                }
            } else {
                BrushTemplateImage.setPixel(x, y, qRgba(0xFF, 0xFF, 0xFF, 0x00));
            }
        }
    }

    int brush_width = qMax(1, qMin(qMin(qFloor(BrushSize / scale()) * 2, CurrentImage.width()), CurrentImage.height()));

    BrushImage = BrushTemplateImage.scaledToWidth(brush_width);
}

void PixelateEditor::openImage(const QString &image_file, int image_orientation)
{
    if (!image_file.isNull()) {
        QImageReader reader(image_file);

        if (reader.canRead()) {
            QSize size = reader.size();

            if (size.width() * size.height() > IMAGE_MPIX_LIMIT * 1000000.0) {
                qreal factor = qSqrt((size.width() * size.height()) / (IMAGE_MPIX_LIMIT * 1000000.0));

                size.setWidth(qFloor(size.width()   / factor));
                size.setHeight(qFloor(size.height() / factor));

                reader.setScaledSize(size);
            }

            LoadedImage = reader.read();

            if (!LoadedImage.isNull()) {
                if (image_orientation == 90) {
                    QTransform transform;

                    transform.rotate(90);

                    LoadedImage = LoadedImage.transformed(transform).scaled(LoadedImage.height(), LoadedImage.width());
                } else if (image_orientation == 180) {
                    QTransform transform;

                    transform.rotate(180);

                    LoadedImage = LoadedImage.transformed(transform).scaled(LoadedImage.width(), LoadedImage.height());
                } else if (image_orientation == 270) {
                    QTransform transform;

                    transform.rotate(270);

                    LoadedImage = LoadedImage.transformed(transform).scaled(LoadedImage.height(), LoadedImage.width());
                }

                LoadedImage = LoadedImage.convertToFormat(QImage::Format_RGB32);

                if (!LoadedImage.isNull()) {
                    auto thread    = new QThread();
                    auto generator = new PixelateImageGenerator();

                    generator->moveToThread(thread);

                    QObject::connect(thread,    &QThread::started,                   generator, &PixelateImageGenerator::start);
                    QObject::connect(thread,    &QThread::finished,                  thread,    &QThread::deleteLater);
                    QObject::connect(generator, &PixelateImageGenerator::imageReady, this,      &PixelateEditor::effectedImageReady);
                    QObject::connect(generator, &PixelateImageGenerator::finished,   thread,    &QThread::quit);
                    QObject::connect(generator, &PixelateImageGenerator::finished,   generator, &PixelateImageGenerator::deleteLater);

                    generator->setPixelDenom(PixelDenom);
                    generator->setInput(LoadedImage);

                    thread->start(QThread::LowPriority);
                } else {
                    emit imageOpenFailed();
                }
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

void PixelateEditor::saveImage(const QString &image_file)
{
    QString file_name = image_file;

    if (!file_name.isNull()) {
        if (!CurrentImage.isNull()) {
            if (QFileInfo(file_name).suffix().compare("png", Qt::CaseInsensitive) != 0 &&
                QFileInfo(file_name).suffix().compare("jpg", Qt::CaseInsensitive) != 0 &&
                QFileInfo(file_name).suffix().compare("bmp", Qt::CaseInsensitive) != 0) {
                file_name = file_name + ".jpg";
            }

            if (CurrentImage.convertToFormat(QImage::Format_ARGB32).save(file_name)) {
                IsChanged = false;

                emit imageSaved(file_name);
            } else {
                emit imageSaveFailed();
            }
        } else {
            emit imageSaveFailed();
        }
    } else {
        emit imageSaveFailed();
    }
}

void PixelateEditor::undo()
{
    if (UndoStack.count() > 0) {
        CurrentImage = UndoStack.pop();

        if (UndoStack.count() == 0) {
            emit undoAvailabilityChanged(false);
        }

        IsChanged = true;

        update();
    }
}

void PixelateEditor::paint(QPainter *painter)
{
    painter->drawImage(QRectF(0, 0, width(), height()), CurrentImage, QRectF(0, 0, CurrentImage.width(), CurrentImage.height()));
}

void PixelateEditor::effectedImageReady(const QImage &effected_image)
{
    OriginalImage = LoadedImage;
    EffectedImage = effected_image;
    CurrentImage  = EffectedImage;

    LoadedImage = QImage();

    UndoStack.clear();

    IsChanged = true;

    setImplicitWidth(CurrentImage.width());
    setImplicitHeight(CurrentImage.height());

    update();

    int brush_width = qMax(1, qMin(qMin(qFloor(BrushSize / scale()) * 2, CurrentImage.width()), CurrentImage.height()));

    BrushImage = BrushTemplateImage.scaledToWidth(brush_width);

    emit undoAvailabilityChanged(false);
    emit imageOpened();
}

void PixelateEditor::scaleWasChanged()
{
    int brush_width = qMax(1, qMin(qMin(qFloor(BrushSize / scale()) * 2, CurrentImage.width()), CurrentImage.height()));

    BrushImage = BrushTemplateImage.scaledToWidth(brush_width);
}

void PixelateEditor::mousePressEvent(QMouseEvent *event)
{
    if (CurrentMode == ModeOriginal || CurrentMode == ModeEffected) {
        ChangeImageAt(true, event->pos().x(), event->pos().y());

        emit mouseEvent(MousePressed, event->pos().x(), event->pos().y());
    }
}

void PixelateEditor::mouseMoveEvent(QMouseEvent *event)
{
    if (CurrentMode == ModeOriginal || CurrentMode == ModeEffected) {
        ChangeImageAt(false, event->pos().x(), event->pos().y());

        emit mouseEvent(MouseMoved, event->pos().x(), event->pos().y());
    }
}

void PixelateEditor::mouseReleaseEvent(QMouseEvent *event)
{
    if (CurrentMode == ModeOriginal || CurrentMode == ModeEffected) {
        emit mouseEvent(MouseReleased, event->pos().x(), event->pos().y());
    }
}

void PixelateEditor::SaveUndoImage()
{
    UndoStack.push(CurrentImage);

    if (UndoStack.count() > UNDO_DEPTH) {
        for (int i = 0; i < UndoStack.count() - UNDO_DEPTH; i++) {
            UndoStack.remove(0);
        }
    }

    emit undoAvailabilityChanged(true);
}

void PixelateEditor::ChangeImageAt(bool save_undo, int center_x, int center_y)
{
    if (CurrentMode != ModeScroll) {
        if (save_undo) {
            SaveUndoImage();
        }

        int width  = qMin(BrushImage.width(),  CurrentImage.width());
        int height = qMin(BrushImage.height(), CurrentImage.height());

        int img_x = qMin(qMax(0, center_x - width  / 2), CurrentImage.width()  - width);
        int img_y = qMin(qMax(0, center_y - height / 2), CurrentImage.height() - height);

        QImage   brush_image(width, height, QImage::Format_ARGB32);
        QPainter brush_painter(&brush_image);

        brush_painter.setCompositionMode(QPainter::CompositionMode_Source);

        if (CurrentMode == ModeOriginal) {
            brush_painter.drawImage(QPoint(0, 0), OriginalImage, QRect(img_x, img_y, width, height));
        } else {
            brush_painter.drawImage(QPoint(0, 0), EffectedImage, QRect(img_x, img_y, width, height));
        }

        QPainter image_painter(&CurrentImage);

        brush_painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        brush_painter.drawImage(QPoint(0, 0), BrushImage);

        image_painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        image_painter.drawImage(QPoint(img_x, img_y), brush_image);

        IsChanged = true;

        update();

        QImage helper_image = CurrentImage.copy(center_x - qFloor((HelperSize / scale()) / 2),
                                                center_y - qFloor((HelperSize / scale()) / 2),
                                                qFloor(HelperSize / scale()),
                                                qFloor(HelperSize / scale())).scaledToWidth(HelperSize);

        emit helperImageReady(helper_image);
    }
}

PixelatePreviewGenerator::PixelatePreviewGenerator(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    PixelateGeneratorRunning = false;
    RestartPixelateGenerator = false;
    PixelDenom               = 0;

    setFlag(QQuickItem::ItemHasContents, true);
}

int PixelatePreviewGenerator::pixDenom() const
{
    return PixelDenom;
}

void PixelatePreviewGenerator::setPixDenom(int pix_denom)
{
    PixelDenom = pix_denom;

    if (!LoadedImage.isNull()) {
        if (PixelateGeneratorRunning) {
            RestartPixelateGenerator = true;
        } else {
            StartPixelateGenerator();
        }
    }
}

void PixelatePreviewGenerator::openImage(const QString &image_file, int image_orientation)
{
    if (!image_file.isNull()) {
        QImageReader reader(image_file);

        if (reader.canRead()) {
            QSize size = reader.size();

            if (size.width() * size.height() > IMAGE_MPIX_LIMIT * 1000000.0) {
                qreal factor = qSqrt((size.width() * size.height()) / (IMAGE_MPIX_LIMIT * 1000000.0));

                size.setWidth(qFloor(size.width()   / factor));
                size.setHeight(qFloor(size.height() / factor));

                reader.setScaledSize(size);
            }

            LoadedImage = reader.read();

            if (!LoadedImage.isNull()) {
                if (image_orientation == 90) {
                    QTransform transform;

                    transform.rotate(90);

                    LoadedImage = LoadedImage.transformed(transform).scaled(LoadedImage.height(), LoadedImage.width());
                } else if (image_orientation == 180) {
                    QTransform transform;

                    transform.rotate(180);

                    LoadedImage = LoadedImage.transformed(transform).scaled(LoadedImage.width(), LoadedImage.height());
                } else if (image_orientation == 270) {
                    QTransform transform;

                    transform.rotate(270);

                    LoadedImage = LoadedImage.transformed(transform).scaled(LoadedImage.height(), LoadedImage.width());
                }

                LoadedImage = LoadedImage.convertToFormat(QImage::Format_RGB32);

                if (!LoadedImage.isNull()) {
                    emit imageOpened();

                    if (PixelateGeneratorRunning) {
                        RestartPixelateGenerator = true;
                    } else {
                        StartPixelateGenerator();
                    }
                } else {
                    emit imageOpenFailed();
                }
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

void PixelatePreviewGenerator::paint(QPainter *painter)
{
    if (!PixelatedImage.isNull()) {
        QImage image = PixelatedImage.scaled(QSize(qFloor(width()), qFloor(height())), Qt::KeepAspectRatio,
                                             smooth() ? Qt::SmoothTransformation : Qt::FastTransformation);

        painter->drawImage(QPointF((width()  - image.width())  / 2,
                                   (height() - image.height()) / 2), image);
    }
}

void PixelatePreviewGenerator::pixelatedImageReady(const QImage &pixelated_image)
{
    PixelateGeneratorRunning = false;
    PixelatedImage           = pixelated_image;

    setImplicitWidth(PixelatedImage.width());
    setImplicitHeight(PixelatedImage.height());

    update();

    emit generationFinished();

    if (RestartPixelateGenerator) {
        StartPixelateGenerator();

        RestartPixelateGenerator = false;
    }
}

void PixelatePreviewGenerator::StartPixelateGenerator()
{
    auto thread    = new QThread();
    auto generator = new PixelateImageGenerator();

    generator->moveToThread(thread);

    QObject::connect(thread,    &QThread::started,                   generator, &PixelateImageGenerator::start);
    QObject::connect(thread,    &QThread::finished,                  thread,    &QThread::deleteLater);
    QObject::connect(generator, &PixelateImageGenerator::imageReady, this,      &PixelatePreviewGenerator::pixelatedImageReady);
    QObject::connect(generator, &PixelateImageGenerator::finished,   thread,    &QThread::quit);
    QObject::connect(generator, &PixelateImageGenerator::finished,   generator, &PixelateImageGenerator::deleteLater);

    generator->setPixelDenom(PixelDenom);
    generator->setInput(LoadedImage);

    thread->start(QThread::LowPriority);

    PixelateGeneratorRunning = true;

    emit generationStarted();
}

PixelateImageGenerator::PixelateImageGenerator(QObject *parent) : QObject(parent)
{
    PixelDenom = 0;
}

void PixelateImageGenerator::setPixelDenom(int pix_denom)
{
    PixelDenom = pix_denom;
}

void PixelateImageGenerator::setInput(const QImage &input_image)
{
    InputImage = input_image;
}

void PixelateImageGenerator::start()
{
    QImage pixelated_image = InputImage;

    int pix_size = pixelated_image.width() > pixelated_image.height() ? pixelated_image.width() / PixelDenom : pixelated_image.height() / PixelDenom;

    if (pix_size != 0) {
        for (int pix_y = 0; pix_y < pixelated_image.height() / pix_size + 1; pix_y++) {
            for (int pix_x = 0; pix_x < pixelated_image.width() / pix_size + 1; pix_x++) {
                int avg_r  = 0;
                int avg_g  = 0;
                int avg_b  = 0;
                int pixels = 0;

                for (int y = pix_y * pix_size; y < (pix_y + 1) * pix_size && y < pixelated_image.height(); y++) {
                    for (int x = pix_x * pix_size; x < (pix_x + 1) * pix_size && x < pixelated_image.width(); x++) {
                        QRgb pixel = pixelated_image.pixel(x, y);

                        avg_r += qRed(pixel);
                        avg_g += qGreen(pixel);
                        avg_b += qBlue(pixel);

                        pixels++;
                    }
                }

                if (pixels != 0) {
                    avg_r = avg_r / pixels;
                    avg_g = avg_g / pixels;
                    avg_b = avg_b / pixels;

                    for (int y = pix_y * pix_size; y < (pix_y + 1) * pix_size && y < pixelated_image.height(); y++) {
                        for (int x = pix_x * pix_size; x < (pix_x + 1) * pix_size && x < pixelated_image.width(); x++) {
                            pixelated_image.setPixel(x, y, qRgba(avg_r, avg_g, avg_b, qAlpha(pixelated_image.pixel(x, y))));
                        }
                    }
                }
            }
        }
    }

    emit imageReady(pixelated_image);
    emit finished();
}
