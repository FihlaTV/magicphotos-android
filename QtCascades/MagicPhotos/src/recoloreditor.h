#ifndef RECOLOREDITOR_H
#define RECOLOREDITOR_H

#include <QtCore/qmath.h>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStack>
#include <QtCore/QHash>
#include <QtGui/QImage>

#include <bb/ImageData>
#include <bb/cascades/Image>
#include <bb/cascades/CustomControl>

class RecolorEditor : public bb::cascades::CustomControl
{
    Q_OBJECT

    Q_PROPERTY(int   mode         READ mode         WRITE setMode)
    Q_PROPERTY(int   brushSize    READ brushSize    WRITE setBrushSize)
    Q_PROPERTY(int   helperSize   READ helperSize   WRITE setHelperSize)
    Q_PROPERTY(int   hue          READ hue          WRITE setHue)
    Q_PROPERTY(qreal brushOpacity READ brushOpacity WRITE setBrushOpacity)
    Q_PROPERTY(qreal scale        READ scale        WRITE setScale)
    Q_PROPERTY(bool  changed      READ changed)

    Q_ENUMS(Mode)

public:
    explicit RecolorEditor();
    virtual ~RecolorEditor();

    int  mode() const;
    void setMode(const int &mode);

    int  brushSize() const;
    void setBrushSize(const int &size);

    int  helperSize() const;
    void setHelperSize(const int &size);

    int  hue() const;
    void setHue(const int &hue);

    qreal brushOpacity() const;
    void  setBrushOpacity(const qreal &opacity);

    qreal scale() const;
    void  setScale(const qreal &scale);

    bool changed() const;

    Q_INVOKABLE void openImage(const QString &image_file);
    Q_INVOKABLE void saveImage(const QString &image_file);

    Q_INVOKABLE void changeImageAt(bool save_undo, int center_x, int center_y);

    Q_INVOKABLE void undo();

    enum Mode {
        ModeScroll,
        ModeOriginal,
        ModeEffected,
        ModeHueSelection
    };

signals:
    void imageOpened();
    void imageOpenFailed();

    void imageSaved();
    void imageSaveFailed();

    void undoAvailabilityChanged(bool available);

    void needImageRepaint(const bb::cascades::Image &image);
    void needHelperRepaint(const bb::cascades::Image &image);

private:
    union RGB16 {
        quint16 rgb;
        struct {
            unsigned r : 5;
            unsigned g : 6;
            unsigned b : 5;
        };
    };

    union HSV {
        quint32 hsv;
        struct {
            qint16 h;
            quint8 s;
            quint8 v;
        };
    };

    void SaveUndoImage();

    QRgb AdjustHue(QRgb rgb);

    void RepaintImage(bool full, QRect rect = QRect());
    void RepaintHelper(int center_x, int center_y);

    static const int UNDO_DEPTH = 4;

    static const qreal IMAGE_MPIX_LIMIT = 1.0;

    bool                    IsChanged;
    int                     CurrentMode, BrushSize, HelperSize, CurrentHue;
    qreal                   BrushOpacity, Scale;
    QImage                  LoadedImage, OriginalImage, CurrentImage, BrushTemplateImage, BrushImage;
    QStack<QImage>          UndoStack;
    QHash<quint16, quint32> RGB16ToHSVMap;
    bb::ImageData           CurrentImageData;
};

#endif // RECOLOREDITOR_H
