#ifndef PLOTSETTINGSDIALOG_H
#define PLOTSETTINGSDIALOG_H

#include <QDialog>
#include "qcustomplot.h"


class PlotSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlotSettingsDialog(QVector<QCustomPlot*>& plots,const QStringList& plotTitle,
                                QWidget *parent = nullptr);
    ~PlotSettingsDialog();

    QVector<QCustomPlot*> &vPlots;
private:

    void setUI();

    QVector<QPushButton*> v_btn;
    QVector<QLineEdit*> v_leColor;
    QVector<QLineEdit*> v_XlineEdit;
    QVector<QLineEdit*> v_YlineEdit;
    QVector<QSpinBox*> v_SpinBox;
    const QStringList& title;

};

#endif // PLOTSETTINGSDIALOG_H
