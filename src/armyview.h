#ifndef ARMYVIEW_H
#define ARMYVIEW_H

#include <QWebView>
#include <QWebInspector>

class ArmyView : public QWebView
{
Q_OBJECT
public:
    explicit ArmyView(QWidget *parent, QString engineurl, QString twrpath );
    ~ArmyView();
    void showInspector( void );
    QString twrpath;

private:
    QWebInspector *inspector;

signals:

public slots:
    void armyViewLoadFinished(bool ok);

};

#endif // ARMYVIEW_H
