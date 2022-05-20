#include "mytablemodel.h"

MyTableModel::MyTableModel(const QVector<QVector<double> > &vec,
                           const QStringList &header, QObject *parent) : QAbstractTableModel(parent) ,
    HorizontalHeader(header), columns(vec)
{
}

MyTableModel::~MyTableModel()
{
}

int MyTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return columns[0].size();
}

int MyTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return columns.size();
}

QVariant MyTableModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
        return columns[index.column()][index.row()];
    return QVariant::Invalid;
}

QVariant MyTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        return QVariant(HorizontalHeader[section]);
    }

    return QVariant();
}
