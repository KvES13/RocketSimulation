#include "plotsettingsdialog.h"

PlotSettingsDialog::PlotSettingsDialog(QVector<QCustomPlot *> &plots, QWidget *parent) :
    QDialog(parent),vPlots(plots)
{
    setUI();

}

PlotSettingsDialog::~PlotSettingsDialog()
{
    //delete ui;
}

void PlotSettingsDialog::setUI()
{
    int row = 0;
    QGridLayout *mainGrid =  new QGridLayout(this);

    for(auto& plot : vPlots)
    {
        QGroupBox *gb = new QGroupBox(this);

        gb->setTitle(plot->graph(0)->name());

        QGridLayout *grid = new QGridLayout(gb);

        QLabel *lblX = new QLabel(gb);
        lblX->setText("X: ");
        lblX->setMaximumSize(QSize(20, 20));
        QLabel *lblY = new QLabel(gb);
        lblY->setText("Y: ");
        lblY->setMaximumSize(QSize(20, 20));
        QLabel *lblWidth = new QLabel(gb);
        lblWidth->setText("Толщина линии");
        QLabel *lblColor = new QLabel(gb);
        lblColor->setText("Текущий цвет отрисовки");

        QLineEdit *leX = new QLineEdit(gb);
        QLineEdit *leY = new QLineEdit(gb);
        leX->setText(plot->xAxis->label());
        leY->setText(plot->yAxis->label());

        QLineEdit *lineColor = new QLineEdit(gb);
        lineColor->setReadOnly(true);
        lineColor->setMinimumSize(QSize(20, 20));
        lineColor->setMaximumSize(QSize(20, 20));
        lineColor->setCursor(QCursor(Qt::IBeamCursor));
        lineColor->setStyleSheet(QString("background-color: %1;").arg(plot->graph(0)->pen().color().name()));
        v_leColor.append(lineColor);

        QSpinBox *spBox = new QSpinBox(gb);
        spBox->setValue(plot->graph(0)->pen().width());
        spBox->setMinimum(1);
        v_SpinBox.append(spBox);
        connect(v_SpinBox.last(),QOverload<int>::of(&QSpinBox::valueChanged),this,
                [&](int i){
            int index = v_SpinBox.indexOf(static_cast<QSpinBox*>(sender()));
            if(index != -1)
            {
                QPen pen = vPlots[index]->graph(0)->pen();
                pen.setWidth(i);
                vPlots[index]->graph(0)->setPen(pen);
            }
        });


        QPushButton *pbColor = new QPushButton(gb);
        pbColor->setText("Выбрать цвет");
        v_btn.append(pbColor);

        //Цвет графика
        connect(v_btn.last(),&QPushButton::clicked,this,
                [&](){
            int index = v_btn.indexOf(static_cast<QPushButton*>(sender()));
            if(index != -1)
            {
                QPen pen = vPlots[index]->graph(0)->pen();
                pen.setColor(QColorDialog::getColor().name());
                v_leColor[index]->setStyleSheet(QString("background-color: %1;").arg(pen.color().name()) );
                vPlots[index]->graph(0)->setPen(pen);
            }
        });

        grid->addWidget(lblX,row,0);
        grid->addWidget(leX,row,1);
        grid->addWidget(lblWidth,row,2);
        grid->addWidget(spBox,row,3);

        row++;

        grid->addWidget(lblY,row,0);
        grid->addWidget(leY,row,1);

        grid->addWidget(lblColor,row,2);
        grid->addWidget(pbColor,row,3);
        grid->addWidget(lineColor,row,4);
        row++;

        mainGrid->addWidget(gb);
    }

    QDialogButtonBox *btnBox = new QDialogButtonBox(this);
    btnBox ->setOrientation(Qt::Horizontal);
    btnBox ->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    connect(btnBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(btnBox, SIGNAL(rejected()), this, SLOT(reject()));


    mainGrid->addWidget(btnBox);
}






































