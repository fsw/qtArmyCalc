#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include "maiaXmlRpcClient.h"
#include <QPushButton>

#define ACE_VERSION "2.0"
#define ACE_APIURL "http://armycalc.com/api/"
#define ACE_XMLAPIURL "http://armycalc.com:80/api/xmlrpc.php"


#ifdef Q_WS_X11
#define ACE_PLATFORM "linux"
#endif

#ifdef Q_WS_WIN
#define ACE_PLATFORM "windows"
#endif

#ifdef Q_WS_MACX
#define ACE_PLATFORM "macos"
#endif

#define ACE_PATH_TO_ENGINE "engine"

class DialogVersionCheck;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QNetworkAccessManager* download_manager;
    QString engineDownloadPath;
    QString filesPath;
    void unzipEngine();
    void unzipTwrs();
    void reloadEngine();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    MaiaXmlRpcClient *rpcClient;
    QString engineVersion;
    DialogVersionCheck* dialogVersionCheck;
    void unzipHelper(QString file);
    QPushButton* addTabButton;

private slots:
    void on_actionDebug_console_triggered();
    void on_tabWidget_currentChanged(int index);
    void on_tabWidget_tabCloseRequested(int index);
    void engineDownloadFinished(QNetworkReply*);
    void on_actionCheck_for_new_version_triggered();
    void on_actionAbout_triggered();
    void on_actionSave_triggered();
    void on_actionNew_triggered();
    void on_actionTest_connection_triggered();
    void on_actionExit_triggered();
    void on_actionChange_triggered();
    void testConnectionResponse(QVariant &);
    void checkForNewVersionResponse(QVariant &);
    void xmlrpcFault(int, const QString &);
};

#endif // MAINWINDOW_H
