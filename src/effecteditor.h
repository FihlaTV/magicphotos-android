#ifndef EFFECTEDITOR_H
#define EFFECTEDITOR_H

#include <QtCore/QObject>
#include <QtGui/QMouseEvent>
#include <QtGui/QImage>

#include "editor.h"

class EffectEditor : public Editor
{
    Q_OBJECT

public:
    explicit EffectEditor(QQuickItem *parent = nullptr);

    EffectEditor(const EffectEditor &) = delete;
    EffectEditor(EffectEditor &&) noexcept = delete;

    EffectEditor &operator=(const EffectEditor &) = delete;
    EffectEditor &operator=(EffectEditor &&) noexcept = delete;

    ~EffectEditor() noexcept override = default;

    enum EditorMode {
        ModeScroll,
        ModeOriginal,
        ModeEffected
    };
    Q_ENUM(EditorMode)

protected slots:
    void setEffectedImage(const QImage &image);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void ChangeImageAt(bool save_undo, int center_x, int center_y);

    QImage OriginalImage, EffectedImage;
};

#endif // EFFECTEDITOR_H
