#ifndef EDITOR_H
#define EDITOR_H

#include <QtCore/QtGlobal>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStack>
#include <QtGui/QMouseEvent>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtQuick/QQuickPaintedItem>

class Editor : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(bool changed READ changed)

    Q_PROPERTY(int   mode          READ mode          WRITE setMode)
    Q_PROPERTY(int   brushSize     READ brushSize     WRITE setBrushSize)
    Q_PROPERTY(int   helperSize    READ helperSize    WRITE setHelperSize)
    Q_PROPERTY(qreal brushHardness READ brushHardness WRITE setBrushHardness)

public:
    explicit Editor(QQuickItem *parent = nullptr);

    Editor(const Editor &) = delete;
    Editor(Editor &&) noexcept = delete;

    Editor &operator=(const Editor &) = delete;
    Editor &operator=(Editor &&) noexcept = delete;

    ~Editor() noexcept override = default;

    bool changed() const;

    int mode() const;
    void setMode(int mode);

    int brushSize() const;
    void setBrushSize(int size);

    int helperSize() const;
    void setHelperSize(int size);

    qreal brushHardness() const;
    void setBrushHardness(qreal hardness);

    Q_INVOKABLE void openImage(const QString &image_path, int image_orientation);
    Q_INVOKABLE void saveImage(const QString &image_path);

    Q_INVOKABLE void undo();

    void paint(QPainter *painter) override;

    enum EditorMode {
        ModeScroll
    };
    Q_ENUM(EditorMode)

    enum MouseState {
        MousePressed,
        MouseMoved,
        MouseReleased
    };
    Q_ENUM(MouseState)

private slots:
    void scaleBrushImage();

signals:
    void imageOpened();
    void imageOpenFailed();
    void imageSaved(const QString &imagePath);
    void imageSaveFailed();
    void undoAvailabilityUpdated(bool available);
    void mouseEvent(int eventType, int x, int y);
    void helperImageReady(const QImage &helperImage);

protected:
    virtual void ProcessOpenedImage() = 0;

    void SaveUndoImage();

    bool   Changed;
    int    Mode, HelperSize;
    QImage LoadedImage, CurrentImage, BrushImage;

private:
    static const int UNDO_DEPTH = 8;

    static constexpr qreal IMAGE_MPIX_LIMIT = 1.0;

    int            BrushSize;
    qreal          BrushHardness;
    QImage         BrushTemplateImage;
    QStack<QImage> UndoStack;
};

#endif // EDITOR_H
