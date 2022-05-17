#ifndef GEOMETRIC_H
#define GEOMETRIC_H

#include <QWidget>
#include <QDateTime>
#include "simulationgl.h"

namespace Ui {
class Geometric;
}

class Geometric : public QWidget
{
    Q_OBJECT

public:
    explicit Geometric(QWidget *parent = nullptr);
    ~Geometric();

private slots:


private:
    Ui::Geometric *ui;
    SimulationGL *ogl;

};

#endif // GEOMETRIC_H
