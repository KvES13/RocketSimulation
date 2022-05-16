#ifndef WROCKET_H
#define WROCKET_H

#include <QWidget>
#include <stage.h>

namespace Ui {
class WRocket;
}

class WRocket : public QWidget
{
    Q_OBJECT

public:
    explicit WRocket(QWidget *parent = nullptr);
    ~WRocket();

    void fillWidget(const RocketStage &stage);

private:
    Ui::WRocket *ui;
};

#endif // WROCKET_H
