#ifndef ARMYVIEW_H
#define ARMYVIEW_H

#include <QWebView>
#include <QWebInspector>
#include <QObject>

class ArmyViewCallback : public QObject
{
Q_OBJECT
    public:
        ArmyViewCallback();
        ~ArmyViewCallback();

    public slots:
        void setStatus(QString string);

};


class ArmyView : public QWebView
{
Q_OBJECT
public:
    explicit ArmyView(QWidget *parent, QString engineurl, QString twrpath );
    ~ArmyView();
    void showInspector( void );
    QString twrpath;
    ArmyViewCallback* callback;

private:
    QWebInspector *inspector;

signals:

public slots:
    void armyViewLoadFinished(bool ok);

};

#endif // ARMYVIEW_H
