#ifndef ANDROIDGW_H
#define ANDROIDGW_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtAndroidExtras/QAndroidJniObject>

class AndroidGW : public QObject
{
    Q_OBJECT

public:
    explicit AndroidGW(QObject *parent = 0);

    static AndroidGW *instance();

    Q_INVOKABLE QString getSaveDirectory();
    Q_INVOKABLE int     getScreenDPI();
    Q_INVOKABLE void    showGallery();
    Q_INVOKABLE void    refreshGallery(QString image_file);
    Q_INVOKABLE void    shareImage(QString image_file);
    Q_INVOKABLE void    showAdView();
    Q_INVOKABLE void    hideAdView();
    Q_INVOKABLE void    createInterstitialAd();
    Q_INVOKABLE void    showInterstitialAd();

signals:
    void adViewHeightUpdated(int adview_height);

    void imageSelected(QString image_file, int image_orientation);
    void imageSelectionCancelled();
    void imageSelectionFailed();

private:
    static AndroidGW *Instance;
};

#endif // ANDROIDGW_H
