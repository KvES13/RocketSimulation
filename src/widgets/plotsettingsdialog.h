#ifndef PLOTSETTINGSDIALOG_H
#define PLOTSETTINGSDIALOG_H

#include <QDialog>
#include "qcustomplot.h"


class PlotSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlotSettingsDialog(QVector<QCustomPlot*>& plots,QWidget *parent = nullptr);
    ~PlotSettingsDialog();

    QVector<QCustomPlot*> &vPlots;
private:

    void setUI();

    QVector<QPushButton*> v_btn;
    QVector<QLineEdit*> v_leColor;
    QVector<QLineEdit*> v_XlineEdit;
    QVector<QLineEdit*> v_YlineEdit;
    QVector<QSpinBox*> v_SpinBox;
};

#endif // PLOTSETTINGSDIALOG_H
