#include "geometric.h"
#include "ui_geometric.h"

Geometric::Geometric(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Geometric)
{
    ui->setupUi(this);

  //  ogl = new SimulationGL(this);
   // ogl->resize(this->width(),this->height());
}

Geometric::~Geometric()
{
    delete ui;
}

