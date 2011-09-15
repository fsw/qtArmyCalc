#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QWebFrame>
#include <QFile>
#include <QMessageBox>
#include <QMap>
#include "dialogversioncheck.h"
#include "quazip.h"
#include "quazipfile.h"
#include "armyview.h"



MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //QPixmap pixmap;
    //pixmap.load(":/icons/about.png");
    //ui->centralWidget->setStyleSheet( setPixmap(pixmap);

    addTabButton = new QPushButton("+",this);
    ui->tabWidget->setCornerWidget(addTabButton);          
    connect(addTabButton, SIGNAL(pressed()), this, SLOT(on_actionNew_triggered()));

    filesPath = QDir::homePath()+"/armycalc/";


    //TODO check for corruption? recreate?
    if(!QDir(filesPath).exists()){
        QDir().mkdir(filesPath);
        QDir().mkdir(filesPath+"/engine/");
        QDir().mkdir(filesPath+"/twrs/");
        QDir().mkdir(filesPath+"/armies/");

        QFile engine_infile( ":/engine.zip" );
        engine_infile.open(QIODevice::ReadOnly);
        QFile engine_outfile( filesPath + "engine.zip" );
        engine_outfile.open(QIODevice::WriteOnly);
        engine_outfile.write(engine_infile.readAll());
        engine_outfile.close();
        engine_infile.close();

        this->unzipEngine();

        QFile twrs_infile( ":/twrs.zip" );
        twrs_infile.open(QIODevice::ReadOnly);
        QFile twrs_outfile( filesPath + "twrs.zip" );
        twrs_outfile.open(QIODevice::WriteOnly);
        twrs_outfile.write(twrs_infile.readAll());
        twrs_outfile.close();
        twrs_infile.close();

        this->unzipTwrs();

        QMessageBox::information(this,
                                 "armycalc folder created",
                                 "I was unable to find 'armycalc' folder.\n It was created in " + filesPath );

    }


    //ui->centralWidget->layout()->addWidget(inspector);


    this->reloadEngine();

    rpcClient = new MaiaXmlRpcClient(QUrl(ACE_XMLAPIURL), this);

    download_manager = new QNetworkAccessManager(this);

   // connect(download_manager, SIGNAL(finished(QNetworkReply*)),
    //         this, SLOT(engineDownloadFinished(QNetworkReply*)));

    dialogVersionCheck = new DialogVersionCheck(this);
    engineDownloadPath = QString(ACE_APIURL) + "engine/engine.zip";
}

MainWindow::~MainWindow()
{
    delete rpcClient;
    delete download_manager;
    delete dialogVersionCheck;
    delete addTabButton;
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



    // QFileDialog::setOption with value QFileDialog::ShowDirsOnly
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}


void MainWindow::testConnectionResponse(QVariant &arg) {
    int resp = arg.toInt();
    if(resp == 22)
        QMessageBox::information(this,"Success", "Connection test passed. Remote functions should work fine." );
    else
        QMessageBox::warning( this, "Connection Error", "Server response differs from expected" );

    //qDebug() << arg.toString();
}

void MainWindow::checkForNewVersionResponse(QVariant &arg) {
    //qDebug() << arg.typeName();

    QMap<QString, QVariant> resp = arg.toMap();

    QString version = resp["version"].toString();
    QString engine = resp["engine"].toString();
    QString note = resp["note"].toString();

    dialogVersionCheck->setup(ACE_VERSION, version, engineVersion, engine, note);
    dialogVersionCheck->exec();

    /*QMessageBox::information(this,
                             "Version Check",
                             "Connection test passed. Remote functions should <b>work</b> fine." );
     */

/*
    int resp = arg.toInt();
    if(resp == 22)
        QMessageBox::information(this,"Success", "Connection test passed. Remote functions should work fine." );
    else
        QMessageBox::warning( this, "Connection Error", "Server response differs from expected" );
*/
    //qDebug() << arg.toString();
}

void MainWindow::xmlrpcFault(int error, const QString &message) {

    QMessageBox::warning( this, QString("Connection Error (")+QString::number(error)+")", message );
    //qDebug() << "EEE:" << error << "-" << message;
}

void MainWindow::on_actionTest_connection_triggered()
{
    QVariantList args;
    args << QVariant(12);
    rpcClient->call("armycalc.test", args,
            this, SLOT(testConnectionResponse(QVariant &)),
            this, SLOT(xmlrpcFault(int, const QString &)));


}

void MainWindow::on_actionNew_triggered()
{

    QString path = QFileDialog::getExistingDirectory (this, tr("Directory"), filesPath+"/twrs/");
    if ( path.isNull() == false )
    {
        //directory.setPath(path);
        //fillList();
        ArmyView * av = new ArmyView( this, filesPath + "engine/qtbody.html" , path );

        ui->tabWidget->addTab(av,QString("New Army"));





        ui->tabWidget->setCurrentWidget(av);
    }


    //QWebView* wv = (QWebView*)ui->tabWidget->widget(ui->tabWidget->currentIndex());

    //QWebFrame* f = wv->page()->mainFrame();


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

void MainWindow::on_actionCheck_for_new_version_triggered()
{

    QVariantList args;
    args << QVariant("en");
    args << QVariant(ACE_VERSION);
    args << QVariant(engineVersion);
    args << QVariant(ACE_PLATFORM);

    rpcClient->call("armycalc.checkForNewVersion", args,
            this, SLOT(checkForNewVersionResponse(QVariant &)),
            this, SLOT(xmlrpcFault(int, const QString &)));

}

void MainWindow::engineDownloadFinished(QNetworkReply*)
{

}

void MainWindow::unzipHelper(QString file){
    QuaZip* zip = new QuaZip(filesPath + file + ".zip");
    zip->open(QuaZip::mdUnzip);

    if(zip->getZipError()==UNZ_OK) {

        QuaZipFile zfile( zip );

        for(bool more=zip->goToFirstFile(); more; more=zip->goToNextFile()) {

            zfile.open(QIODevice::ReadOnly);
            qDebug() << zfile.getActualFileName();

            //this looks lame but works
            if(QString(zfile.getActualFileName()).right(1)=="/"){
                QDir().mkdir(filesPath + file + "/" + zfile.getActualFileName());
            } else {
                QFile outfile( filesPath + file + "/" + zfile.getActualFileName() );
                outfile.open(QIODevice::WriteOnly);
                outfile.write(zfile.readAll());
                outfile.close();
            }
            zfile.close();
        }
        if(zip->getZipError()==UNZ_OK) {
          // ok, there was no error
        }
    }

    //qDebug() << zip->getZipError();

    delete zip;
}

void MainWindow::unzipEngine(){

    this->unzipHelper("engine");

}

void MainWindow::unzipTwrs(){

    this->unzipHelper("twrs");

}

void MainWindow::reloadEngine()
{

    QFile file( filesPath + "engine/" + "VERSION" );

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

    //TODO: warning!
    // save all?
    for(int i=0;i<ui->tabWidget->count();i++){
        ArmyView * wv = (ArmyView *)ui->tabWidget->widget(i);
        wv->setUrl( QUrl( filesPath + "engine/qtbody.html" ));
    }

    //this->close();
    //QCoreApplication::exit(1000);

}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{

    ArmyView * av = (ArmyView *)ui->tabWidget->widget(index);
    ui->tabWidget->removeTab(index);
    delete av;
}

void MainWindow::on_tabWidget_currentChanged(int index)
{

    ArmyView * av = (ArmyView *)ui->tabWidget->widget(index);

}

void MainWindow::on_actionDebug_console_triggered()
{
    if(ui->tabWidget->currentIndex() > -1){

        ArmyView * av = (ArmyView *)ui->tabWidget->widget(ui->tabWidget->currentIndex());
        av->showInspector();

    }

}
