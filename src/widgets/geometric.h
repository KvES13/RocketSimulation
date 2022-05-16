#ifndef GEOMETRIC_H
#define GEOMETRIC_H

#include <QWidget>
#include <QDateTime>

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

};

#endif // GEOMETRIC_H
