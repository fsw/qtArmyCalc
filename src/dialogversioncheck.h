#ifndef DIALOGVERSIONCHECK_H
#define DIALOGVERSIONCHECK_H

#include <QDialog>
#include <QNetworkAccessManager>
#include "mainwindow.h"

namespace Ui {
    class DialogVersionCheck;
}

class DialogVersionCheck : public QDialog {
    Q_OBJECT
public:
    DialogVersionCheck(MainWindow *parent = 0);
    ~DialogVersionCheck();
    void setup(QString localAppVersion, QString remoteAppVersion, QString localEngineVersion, QString remoteEngineVersion, QString note);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogVersionCheck *ui;
    QNetworkReply *currentDownload;

private slots:
    void on_finishUpdateButton_clicked();
    void on_updateEngineButton_clicked();
    void engineDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void engineDownloadFinished();
};

#endif // DIALOGVERSIONCHECK_H
