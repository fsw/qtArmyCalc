#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QWebFrame>
#include <QFile>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFile file( QString(ACE_PATH_TO_ENGINE) + "/VERSION" );

    if (!file.open (QIODevice::ReadOnly)){
        engineVersion = "UNKNOWN";
    } else {

        QTextStream stream ( &file );
        if( !stream.atEnd() ) {
             engineVersion = stream.readLine();
        }
        file.close();
    }

    this->setWindowTitle(QString("ArmyCalc - version ") + ACE_VERSION + " - engine version " + engineVersion);


    QWebSettings *settings = ui->webView->settings();
    settings->setAttribute(QWebSettings::JavascriptEnabled, true);
    settings->setAttribute(QWebSettings::PluginsEnabled, true);
    settings->setAttribute(QWebSettings::AutoLoadImages, true);
    settings->setAttribute(QWebSettings::JavaEnabled, false);
    settings->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);

    ui->webView->setUrl( QUrl("engine/qtbody.html"));

    rpcClient = new MaiaXmlRpcClient(QUrl("http://armycalc.com:80/xmlrpc.php"), this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void MainWindow::on_actionChange_triggered()
{
    QDir directory;
    QString path = QFileDialog::getExistingDirectory (this, tr("Directory"), directory.path());
    if ( path.isNull() == false )
    {
        directory.setPath(path);
        //fillList();
        QWebFrame* f = ui->webView->page()->mainFrame();
        f->evaluateJavaScript(QString("calc.loadTWR('") + path + "/')");
    }

    // QFileDialog::setOption with value QFileDialog::ShowDirsOnly
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}


void MainWindow::testConnectionResponse(QVariant &arg) {
                qDebug() << arg.toString();
}

void MainWindow::testConnectionFault(int error, const QString &message) {
                qDebug() << "EEE:" << error << "-" << message;
}

void MainWindow::on_actionTest_connection_triggered()
{
    QVariantList args;
    args << QVariant(12);
    rpcClient->call("armycalc.test", args,
            this, SLOT(testConnectionResponse(QVariant &)),
            this, SLOT(testConnectionFault(int, const QString &)));


}

void MainWindow::on_actionNew_triggered()
{
    ui->webView->page()->mainFrame()->evaluateJavaScript("calc.newArmy()");
}

void MainWindow::on_actionSave_triggered()
{

}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,"About",
        QString("<b>ArmyCalc</b> &copy; 2009-2011 CadoSolutions")+
        "<br/><center>version: <b>" + ACE_VERSION +
        "</b><br/>engine version: <b>" + engineVersion +
        "</b><br/>platform: <b>" + ACE_PLATFORM +
        "</b></center><br/>"+
        "<br/>This free, open source application <br/> is a part of the ArmyCalc.com project."
        "<br/>For more information, <br/> "+
        "please visit <a href='http://armycalc.com'>http://armycalc.com</a><br/>");

}
