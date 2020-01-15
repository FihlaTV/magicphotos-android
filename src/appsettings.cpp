#include "appsettings.h"

AppSettings::AppSettings(QObject *parent) : QObject(parent)
{
    DefaultBrushSize = 0;
    Settings         = std::make_unique<QSettings>(QStringLiteral("Oleg Derevenetz"), QStringLiteral("MagicPhotos"));
}

AppSettings &AppSettings::GetInstance()
{
    static AppSettings instance;

    return instance;
}

int AppSettings::defaultBrushSize() const
{
    return DefaultBrushSize;
}

void AppSettings::setDefaultBrushSize(int size)
{
    DefaultBrushSize = size;
}

int AppSettings::brushSize() const
{
    if (Settings->contains(QStringLiteral("BrushSize"))) {
        return Settings->value(QStringLiteral("BrushSize")).toInt();
    } else {
        return DefaultBrushSize;
    }
}

void AppSettings::setBrushSize(int size)
{
    Settings->setValue(QStringLiteral("BrushSize"), size);
}

qreal AppSettings::brushHardness() const
{
    if (Settings->contains(QStringLiteral("BrushHardness"))) {
        return Settings->value(QStringLiteral("BrushHardness")).toDouble();
    } else {
        return 0.75;
    }
}

void AppSettings::setBrushHardness(qreal hardness)
{
    Settings->setValue(QStringLiteral("BrushHardness"), hardness);
}

QString AppSettings::adMobConsent() const
{
    if (Settings->contains(QStringLiteral("AdMobConsent"))) {
        return Settings->value(QStringLiteral("AdMobConsent")).toString();
    } else {
        return QStringLiteral("");
    }
}

void AppSettings::setAdMobConsent(const QString &consent)
{
    Settings->setValue(QStringLiteral("AdMobConsent"), consent);
}
