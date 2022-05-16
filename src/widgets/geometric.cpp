#include "geometric.h"
#include "ui_geometric.h"

Geometric::Geometric(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Geometric)
{
    ui->setupUi(this);


}

Geometric::~Geometric()
{
    delete ui;
}

