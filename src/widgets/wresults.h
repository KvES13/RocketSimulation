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
                      QStringList&& header,
                      QWidget *parent = nullptr);
    ~wResults();

public slots:
    void slotOpenDialog();

private:

    static constexpr std::array<Qt::GlobalColor,4> colors = {Qt::blue,Qt::red, Qt::green, Qt::white};

    const QVector<QVector<double>> &values;

    QVector<QCustomPlot*> vPlots;
    QStringList headerData;
    QTableView *tableView;
    QGridLayout *grid;
    MyTableModel *tableModel;
    QPushButton *btnSettingsDialog;

};

#endif // WRESULTS_H
