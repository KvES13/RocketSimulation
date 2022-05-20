#ifndef RESULTS_H
#define RESULTS_H

#include <QWidget>
#include <QString>

#include "rocket/flightobserver.h"
#include "rocket/stage.h"

#include "widgets/userplot.h"
#include "widgets/wresults.h"

namespace Ui {
class Results;
}

class Results : public QWidget
{
    Q_OBJECT

public:
    explicit Results(QWidget *parent = nullptr);
    ~Results();

    void SetFilePath(const QString& filepath, QVector<FlightObserver*> obs);

public slots:
    void paintResults();

private:
    Ui::Results *ui;

    void plotVelocity();
    void plotPosition();
    void plotForce();
    void plotAcceleration();
    void plotMass();
    void plotAmtosphere();


    QString CsvStageFilePath;
    QVector<FlightObserver*> observers;
    int stagesCount;

    QVector<QVector<double>> vvv;
    QVector<QVector<double>> vmass;
    QVector<QVector<double>> vVelocityECEF;
    QVector<QVector<double>> vVelocityECI;
    QVector<QVector<double>> vVelocityNED;

    QVector<QVector<double>> vPosECEF;
    QVector<QVector<double>> vPosECI;
    QVector<QVector<double>> vPosLLH;

    QVector<QVector<double>> vForce;
    QVector<QVector<double>> vForceThrust;
    QVector<QVector<double>> vForceAero;
    QVector<QVector<double>> vForceGravity;


    QVector<QVector<double>> vAcECI;
    QVector<QVector<double>> vAcBody;

};

#endif // RESULTS_H



