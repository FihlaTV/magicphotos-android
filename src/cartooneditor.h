#ifndef CARTOONEDITOR_H
#define CARTOONEDITOR_H

#include <QtCore/QObject>
#include <QtGui/QMouseEvent>
#include <QtGui/QImage>

#include "effecteditor.h"
#include "previewgenerator.h"

class CartoonEditor : public EffectEditor
{
    Q_OBJECT

    Q_PROPERTY(int radius    READ radius    WRITE setRadius)
    Q_PROPERTY(int threshold READ threshold WRITE setThreshold)

public:
    explicit CartoonEditor(QQuickItem *parent = nullptr);

    CartoonEditor(const CartoonEditor &) = delete;
    CartoonEditor(CartoonEditor &&) noexcept = delete;

    CartoonEditor &operator=(const CartoonEditor &) = delete;
    CartoonEditor &operator=(CartoonEditor &&) noexcept = delete;

    ~CartoonEditor() noexcept override = default;

    int radius() const;
    void setRadius(int radius);

    int threshold() const;
    void setThreshold(int threshold);

protected:
    void ProcessOpenedImage() override;

private:
    int Radius, Threshold;
};

class CartoonPreviewGenerator : public PreviewGenerator
{
    Q_OBJECT

    Q_PROPERTY(int radius    READ radius    WRITE setRadius)
    Q_PROPERTY(int threshold READ threshold WRITE setThreshold)

public:
    explicit CartoonPreviewGenerator(QQuickItem *parent = nullptr);

    CartoonPreviewGenerator(const CartoonPreviewGenerator &) = delete;
    CartoonPreviewGenerator(CartoonPreviewGenerator &&) noexcept = delete;

    CartoonPreviewGenerator &operator=(const CartoonPreviewGenerator &) = delete;
    CartoonPreviewGenerator &operator=(CartoonPreviewGenerator &&) noexcept = delete;

    ~CartoonPreviewGenerator() noexcept override = default;

    int radius() const;
    void setRadius(int radius);

    int threshold() const;
    void setThreshold(int threshold);

protected:
    void StartImageGenerator() override;

private:
    int Radius, Threshold;
};

class CartoonImageGenerator : public QObject
{
    Q_OBJECT

public:
    explicit CartoonImageGenerator(QObject *parent = nullptr);

    CartoonImageGenerator(const CartoonImageGenerator &) = delete;
    CartoonImageGenerator(CartoonImageGenerator &&) noexcept = delete;

    CartoonImageGenerator &operator=(const CartoonImageGenerator &) = delete;
    CartoonImageGenerator &operator=(CartoonImageGenerator &&) noexcept = delete;

    ~CartoonImageGenerator() noexcept override = default;

    void SetRadius(int radius);
    void SetThreshold(int threshold);
    void SetInput(const QImage &input_image);

public slots:
    void start();

signals:
    void imageReady(const QImage &outputImage);
    void finished();

private:
    int    Radius, Threshold;
    QImage InputImage;
};

#endif // CARTOONEDITOR_H
