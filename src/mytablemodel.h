#ifndef MYTABLEMODEL_H
#define MYTABLEMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include "flightobserver.h"

class MyTableModel : public QAbstractTableModel
{
public:
    MyTableModel(const QVector<QVector<double>> & vec,
                 const  QStringList& header,QObject *parent = nullptr) ;
    ~MyTableModel();

    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
private:
    QStringList HorizontalHeader;
    const QVector<QVector<double>> columns;

};

#endif // MYTABLEMODEL_H
