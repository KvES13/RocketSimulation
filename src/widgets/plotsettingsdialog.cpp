#include "plotsettingsdialog.h"

PlotSettingsDialog::PlotSettingsDialog(QVector<QCustomPlot *> &plots,
                                       QWidget *parent) :
    QDialog(parent),vPlots(plots)
{
    setUI();
}

PlotSettingsDialog::~PlotSettingsDialog()
{

}

void PlotSettingsDialog::setUI()
{
    int row = 0;
    QGridLayout *mainGrid =  new QGridLayout(this);

    for(auto& plot : vPlots)
    {
         QGroupBox *gb = new QGroupBox(this);
         gb->setTitle("D");

        QGridLayout *grid = new QGridLayout(gb);
        for(int i = 0; i < plot->graphCount(); i++)
        {

            auto graph = plot->graph(i); 

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
            lineColor->setStyleSheet(
                        QString("background-color: %1;").arg(
                            graph->pen().color().name()));
            v_leColor.append(lineColor);

            QSpinBox *spBox = new QSpinBox(gb);
            spBox->setValue(graph->pen().width());
            spBox->setMinimum(1);
            spBox->setWhatsThis(QString::number(i));
            v_SpinBox.append(spBox);
            connect(v_SpinBox.last(),QOverload<int>::of(&QSpinBox::valueChanged),
                    this,[&](int i){
                int index = v_SpinBox.indexOf(static_cast<QSpinBox*>(sender()));
                if(index != -1)
                {
                    int graph_index = QString(v_SpinBox[index]->whatsThis()).toInt();
                    QPen pen = vPlots[index]->graph(graph_index)->pen();
                    pen.setWidth(i);
                    vPlots[index]->graph(graph_index)->setPen(pen);
                }
            });


            QPushButton *pbColor = new QPushButton(gb);
            pbColor->setText("Выбрать цвет");
            pbColor->setWhatsThis(QString::number(i));
            v_btn.append(pbColor);

            //Цвет графика
            connect(v_btn.last(),&QPushButton::clicked,this,
                    [&](){
                int index = v_btn.indexOf(static_cast<QPushButton*>(sender()));

                qDebug()<<v_btn.size()<<" Index "<<index;
                if(index != -1)
                {
                    int graph_index = QString(v_btn[index]->whatsThis()).toInt();
                    qDebug()<<"gra "<<graph_index;
                    QPen pen = vPlots[index]->graph(graph_index)->pen();
                    pen.setColor(QColorDialog::getColor().name());
                    v_leColor[index]->setStyleSheet(
                                QString("background-color: %1;").arg(
                                    pen.color().name()) );
                    vPlots[index]->graph(graph_index)->setPen(pen);
                }
            });

            grid->addWidget(lblX,row+i,0+i*5);
            grid->addWidget(leX,row+i,1+i*5);
            grid->addWidget(lblWidth,row+i,2+i*5);
            grid->addWidget(spBox,row+i,3+i*5);

            row++;

            grid->addWidget(lblY,row+i,0+i*5);
            grid->addWidget(leY,row+i,1+i*5);

            grid->addWidget(lblColor,row+i,2+i*5);
            grid->addWidget(pbColor,row+i,3+i*5);
            grid->addWidget(lineColor,row+i,4+i*5);
            row++;

            mainGrid->addWidget(gb);
        }
    }

    QDialogButtonBox *btnBox = new QDialogButtonBox(this);
    btnBox ->setOrientation(Qt::Horizontal);
    btnBox ->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    connect(btnBox,&QDialogButtonBox::accepted, this, &PlotSettingsDialog::accept);
    connect(btnBox, &QDialogButtonBox::rejected, this, &PlotSettingsDialog::reject);


    mainGrid->addWidget(btnBox);
}






































