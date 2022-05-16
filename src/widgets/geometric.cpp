#include "geometric.h"
#include "ui_geometric.h"

Geometric::Geometric(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Geometric)
{
    ui->setupUi(this);


     img = {QPixmap(":/textures/A"),QPixmap(":/textures/AA"),
            QPixmap(":/textures/AAA"),QPixmap(":/textures/AAAA"),
            QPixmap(":/textures/OOO")};

    ui->lblIMG->setPixmap(img[0]);
}

Geometric::~Geometric()
{
    delete ui;
}

void Geometric::on_pushButton_clicked()
{
    if(index != 0)
        index --;

    ui->lblIMG->setPixmap(img[index]);
}


void Geometric::on_pushButton_2_clicked()
{
    if(index != img.size()-1)
        index ++;

    ui->lblIMG->setPixmap(img[index]);
}

