#ifndef WRESULTS_H
#define WRESULTS_H

#include <QWidget>
#include <QTableView>
#include <QGridLayout>
#include <QVector>
#include <memory>

#include "plotsettingsdialog.h"

#include "mytablemodel.h"

struct PlotInfo
{
    QVector<QVector<double>> values;
    int graphsCount;
    QStringList headerData;
    QString title = "";
    bool isDisplayedTable = true;
};


class wResults : public QWidget
{
    Q_OBJECT

public:
    explicit wResults(PlotInfo &&info,QWidget *parent = nullptr);
    ~wResults();

public slots:
    void slotOpenDialog();

private:

    void setUI();

    static constexpr std::array<Qt::GlobalColor,10> colors =
    {Qt::blue,Qt::red,Qt::darkRed, Qt::green,Qt::darkGreen, Qt::lightGray,
     Qt::yellow,Qt::darkYellow, Qt::darkCyan,Qt::white};

    const QVector<QVector<double>> values;

    PlotInfo plotInfo;
    QVector<QCustomPlot*> vPlots;


};

#endif // WRESULTS_H
