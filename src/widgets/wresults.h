#ifndef WRESULTS_H
#define WRESULTS_H

#include <QWidget>
#include <QTableView>
#include <QGridLayout>
#include <QVector>
#include <memory>

#include "plotsettingsdialog.h"

#include "mytablemodel.h"


class wResults : public QWidget
{
    Q_OBJECT

public:
    explicit wResults(const QVector<QVector<double> > &data,
                      QStringList&& header, int stagesCount,
                      bool displayTable = true, QWidget *parent = nullptr);
    ~wResults();

public slots:
    void slotOpenDialog();

private:

    static constexpr std::array<Qt::GlobalColor,10> colors =
    {Qt::blue,Qt::red,Qt::darkRed, Qt::green,Qt::darkGreen, Qt::lightGray,
     Qt::yellow,Qt::darkYellow, Qt::darkCyan,Qt::white};

    const QVector<QVector<double>> values;


    QVector<QCustomPlot*> vPlots;
    QStringList headerData;
    int graphsCount;
    bool isDisplayedTable;


};

#endif // WRESULTS_H
