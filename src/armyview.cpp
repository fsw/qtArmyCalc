#include "armyview.h"
#include <QWebFrame>




ArmyViewCallback::ArmyViewCallback(){}
ArmyViewCallback::~ArmyViewCallback(){}

void ArmyViewCallback::setStatus(QString string){}


ArmyView::ArmyView(QWidget *parent, QString engineurl, QString twrpath) :
    QWebView(parent)
{

    this->twrpath = twrpath;
    QWebSettings *settings = this->settings();
    settings->setAttribute(QWebSettings::JavascriptEnabled, true);
    settings->setAttribute(QWebSettings::PluginsEnabled, true);
    settings->setAttribute(QWebSettings::AutoLoadImages, true);
    settings->setAttribute(QWebSettings::JavaEnabled, false);
    settings->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
    //settings->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);

    inspector = NULL;

    connect(this, SIGNAL(loadFinished(bool)),
                    SLOT(armyViewLoadFinished(bool)));

    this->setUrl( QUrl( engineurl ));

}

ArmyView::~ArmyView()
{
    if(inspector){
        delete inspector;
    }
}


void ArmyView::showInspector( void )
{
    if(inspector){
        delete inspector;
    }

    inspector = new QWebInspector;
    this->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    inspector->setPage(this->page());
    inspector->show();


}


void ArmyView::armyViewLoadFinished(bool ok)
{

    QWebFrame* f = this->page()->mainFrame();

    f->evaluateJavaScript(QString("calc.loadTWR('") + this->twrpath + "/',true)");
    //f->evaluateJavaScript("alert(1)");

}
