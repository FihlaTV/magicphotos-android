#include <array>
#include <memory>

#include <QtCore/QtMath>
#include <QtCore/QThread>
#include <QtGui/QPainter>

#include "blureditor.h"

BlurEditor::BlurEditor(QQuickItem *parent) :
    EffectEditor(parent),
    Radius      (0)
{
}

int BlurEditor::radius() const
{
    return Radius;
}

void BlurEditor::setRadius(int radius)
{
    Radius = radius;
}

void BlurEditor::ProcessOpenedImage()
{
    auto thread    = std::make_unique<QThread>();
    auto generator = std::make_unique<BlurImageGenerator>();

    generator->moveToThread(thread.get());

    connect(thread.get(),    &QThread::started,               generator.get(), &BlurImageGenerator::start);
    connect(thread.get(),    &QThread::finished,              thread.get(),    &QThread::deleteLater);
    connect(generator.get(), &BlurImageGenerator::imageReady, this,            &BlurEditor::setEffectedImage);
    connect(generator.get(), &BlurImageGenerator::finished,   thread.get(),    &QThread::quit);
    connect(generator.get(), &BlurImageGenerator::finished,   generator.get(), &BlurImageGenerator::deleteLater);

    generator->SetRadius(Radius);
    generator->SetInput(LoadedImage);

    thread->start(QThread::LowPriority);

    static_cast<void>(thread.release());
    static_cast<void>(generator.release());
}

BlurPreviewGenerator::BlurPreviewGenerator(QQuickItem *parent) :
    PreviewGenerator(parent),
    Radius          (0)
{
}

int BlurPreviewGenerator::radius() const
{
    return Radius;
}

void BlurPreviewGenerator::setRadius(int radius)
{
    Radius = radius;

    if (!LoadedImage.isNull()) {
        if (ImageGeneratorRunning) {
            RestartImageGenerator = true;
        } else {
            StartImageGenerator();
        }
    }
}

void BlurPreviewGenerator::StartImageGenerator()
{
    auto thread    = std::make_unique<QThread>();
    auto generator = std::make_unique<BlurImageGenerator>();

    generator->moveToThread(thread.get());

    connect(thread.get(),    &QThread::started,               generator.get(), &BlurImageGenerator::start);
    connect(thread.get(),    &QThread::finished,              thread.get(),    &QThread::deleteLater);
    connect(generator.get(), &BlurImageGenerator::imageReady, this,            &BlurPreviewGenerator::setEffectedImage);
    connect(generator.get(), &BlurImageGenerator::finished,   thread.get(),    &QThread::quit);
    connect(generator.get(), &BlurImageGenerator::finished,   generator.get(), &BlurImageGenerator::deleteLater);

    generator->SetRadius(Radius);
    generator->SetInput(LoadedImage);

    thread->start(QThread::LowPriority);

    static_cast<void>(thread.release());
    static_cast<void>(generator.release());

    ImageGeneratorRunning = true;

    emit generationStarted();
}

BlurImageGenerator::BlurImageGenerator(QObject *parent) :
    QObject(parent),
    Radius (0)
{
}

void BlurImageGenerator::SetRadius(int radius)
{
    Radius = radius;
}

void BlurImageGenerator::SetInput(const QImage &input_image)
{
    InputImage = input_image;
}

void BlurImageGenerator::start()
{
    QImage blur_image = InputImage;

    QImage::Format format = blur_image.format();

    blur_image = blur_image.convertToFormat(QImage::Format_ARGB32_Premultiplied);

    constexpr std::array<int, 17> tab = {14, 10, 8, 6, 5, 5, 4, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2};

    int alpha = Radius < 1 ? 16 : (Radius > 17 ? 1 : tab[Radius - 1]);

    int r1 = blur_image.rect().top();
    int r2 = blur_image.rect().bottom();
    int c1 = blur_image.rect().left();
    int c2 = blur_image.rect().right();

    int bpl = blur_image.bytesPerLine();

    std::array<int, 4> rgba = {};
    unsigned char     *p;

    for (int col = c1; col <= c2; col++) {
        p = blur_image.scanLine(r1) + col * 4;

        for (int i = 0; i < 4; i++) {
            rgba[i] = p[i] << 4;
        }

        p += bpl;

        for (int j = r1; j < r2; j++, p += bpl) {
            for (int i = 0; i < 4; i++) {
                p[i] = static_cast<unsigned char>((rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4);
            }
        }
    }

    for (int row = r1; row <= r2; row++) {
        p = blur_image.scanLine(row) + c1 * 4;

        for (int i = 0; i < 4; i++) {
            rgba[i] = p[i] << 4;
        }

        p += 4;

        for (int j = c1; j < c2; j++, p += 4) {
            for (int i = 0; i < 4; i++) {
                p[i] = static_cast<unsigned char>((rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4);
            }
        }
    }

    for (int col = c1; col <= c2; col++) {
        p = blur_image.scanLine(r2) + col * 4;

        for (int i = 0; i < 4; i++) {
            rgba[i] = p[i] << 4;
        }

        p -= bpl;

        for (int j = r1; j < r2; j++, p -= bpl) {
            for (int i = 0; i < 4; i++) {
                p[i] = static_cast<unsigned char>((rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4);
            }
        }
    }

    for (int row = r1; row <= r2; row++) {
        p = blur_image.scanLine(row) + c2 * 4;

        for (int i = 0; i < 4; i++) {
            rgba[i] = p[i] << 4;
        }

        p -= 4;

        for (int j = c1; j < c2; j++, p -= 4) {
            for (int i = 0; i < 4; i++) {
                p[i] = static_cast<unsigned char>((rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4);
            }
        }
    }

    blur_image = blur_image.convertToFormat(format);

    emit imageReady(blur_image);
    emit finished();
}
