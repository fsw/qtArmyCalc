#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "maiaXmlRpcClient.h"

#define ACE_VERSION "2.0"

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

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    MaiaXmlRpcClient *rpcClient;
    QString engineVersion;

private slots:
    void on_actionAbout_triggered();
    void on_actionSave_triggered();
    void on_actionNew_triggered();
    void on_actionTest_connection_triggered();
    void on_actionExit_triggered();
    void on_actionChange_triggered();
    void testConnectionResponse(QVariant &);
    void testConnectionFault(int, const QString &);
};

#endif // MAINWINDOW_H
