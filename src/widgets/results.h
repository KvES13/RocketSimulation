#ifndef RESULTS_H
#define RESULTS_H

#include <QWidget>
#include <QString>

#include "rocket/flightobserver.h"
#include "rocket/stage.h"

#include "widgets/userplot.h"
#include "widgets/wresults.h"


class Results : public QWidget
{
    Q_OBJECT

public:
    explicit Results(QWidget *parent = nullptr);
    ~Results();

    void SetObservers(QVector<FlightObserver *> obs);

public slots:
    void paintResults();

private:

    void plot();
    void plotVelocity();
    void plotPosition();
    void plotForce();
    void plotAcceleration();
    void plotMass();
    void plotAmtosphere();

    void resetData();

    QTabWidget *tabWidget;
    QVector<FlightObserver*> observers;
    int stagesCount;
};

#endif // RESULTS_H



