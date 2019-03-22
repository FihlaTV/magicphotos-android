#include <QtGui/QPainter>

#include "helper.h"

Helper::Helper(QQuickPaintedItem *parent) : QQuickPaintedItem(parent)
{
    setFlag(QQuickItem::ItemHasContents, true);
}

void Helper::paint(QPainter *painter)
{
    painter->drawImage(QRectF(0, 0, width(), height()), HelperImage, QRectF(0, 0, HelperImage.width(), HelperImage.height()));
}

void Helper::helperImageReady(const QImage &helper_image)
{
    HelperImage = helper_image;

    update();
}
