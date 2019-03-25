#ifndef RETOUCHEDITOR_H
#define RETOUCHEDITOR_H

#include <QtCore/QObject>
#include <QtCore/QPoint>
#include <QtGui/QMouseEvent>

#include "editor.h"

class RetouchEditor : public Editor
{
    Q_OBJECT

    Q_PROPERTY(bool   samplingPointValid READ samplingPointValid NOTIFY samplingPointValidChanged)
    Q_PROPERTY(QPoint samplingPoint      READ samplingPoint      NOTIFY samplingPointChanged)

    Q_ENUMS(Mode)

public:
    explicit RetouchEditor(QQuickItem *parent = nullptr);
    ~RetouchEditor() override = default;

    bool samplingPointValid() const;
    QPoint samplingPoint() const;

    enum Mode {
        ModeScroll,
        ModeSamplingPoint,
        ModeClone,
        ModeBlur
    };

signals:
    void samplingPointValidChanged(bool samplingPointValid);
    void samplingPointChanged(QPoint samplingPoint);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void processOpenedImage() override;

private:
    void ChangeImageAt(bool save_undo, int center_x, int center_y);

    static const int GAUSSIAN_RADIUS = 4;

    bool   IsSamplingPointValid, IsLastBlurPointValid;
    QPoint SamplingPoint, InitialSamplingPoint, LastBlurPoint, InitialTouchPoint;
};

#endif // RETOUCHEDITOR_H
