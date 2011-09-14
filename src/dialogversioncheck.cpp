#include "dialogversioncheck.h"
#include "ui_dialogversioncheck.h"

DialogVersionCheck::DialogVersionCheck(MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::DialogVersionCheck)
{
    ui->setupUi(this);
}

DialogVersionCheck::~DialogVersionCheck()
{
    delete ui;
}

void DialogVersionCheck::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DialogVersionCheck::setup(QString localAppVersion, QString remoteAppVersion, QString localEngineVersion, QString remoteEngineVersion, QString note){


    QString appFont("<font color=\"red\">");
    if(localAppVersion == remoteAppVersion)
        appFont = "<font color=\"green\">";

    ui->label_localAppVersion->setText(appFont + localAppVersion + "</font>");
    ui->label_remoteAppVersion->setText(appFont + remoteAppVersion + "</font>");

    QString engineFont("<font color=\"red\">");
    if(localEngineVersion == remoteEngineVersion)
        engineFont = "<font color=\"green\">";

    ui->label_localEngineVersion->setText(engineFont + localEngineVersion + "</font>");
    ui->label_remoteEngineVersion->setText(engineFont + remoteEngineVersion + "</font>");
    ui->label_note->setText(note);

    ui->label_application->setVisible(localAppVersion != remoteAppVersion);

    ui->updateEngineButton->setVisible(localEngineVersion != remoteEngineVersion);
    ui->progressBar->setVisible( false );
    ui->finishUpdateButton->setVisible( false );
    ui->label_error->setVisible( false );
}






void DialogVersionCheck::on_updateEngineButton_clicked()
{

    MainWindow* mw = ((MainWindow *)this->parentWidget());


    QNetworkRequest request( mw->engineDownloadPath );
    currentDownload = mw->download_manager->get(request);

    ui->updateEngineButton->setVisible( false );
    ui->progressBar->setValue( 0 );
    ui->progressBar->setVisible( true );
    ui->finishUpdateButton->setVisible( false );

    connect(currentDownload, SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(engineDownloadProgress(qint64,qint64)));
    connect(currentDownload, SIGNAL(finished()),
            SLOT(engineDownloadFinished()));

    //this->accept();
}



void DialogVersionCheck::engineDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    ui->progressBar->setValue( (bytesReceived / bytesTotal) * 100 );
}

void DialogVersionCheck::engineDownloadFinished()
{

    if (currentDownload->error()) {
        ui->progressBar->setVisible( false );
        ui->label_error->setVisible( true );
        ui->label_error->setText(currentDownload->errorString());
    } else {

        currentDownload->readAll();

        ui->progressBar->setVisible( false );
        ui->finishUpdateButton->setVisible( true );
    }
}




void DialogVersionCheck::on_finishUpdateButton_clicked()
{

    this->accept();
    MainWindow* mw = ((MainWindow *)this->parentWidget());
    mw->reloadEngine();
}
