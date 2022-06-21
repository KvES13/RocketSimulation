#include "plotsettingsdialog.h"

PlotSettingsDialog::PlotSettingsDialog(QVector<QCustomPlot *> &plots,
                                       const QStringList& plotTitle,QWidget *parent):
    QDialog(parent),vPlots(plots), title(plotTitle)
{
    setUI();
}

PlotSettingsDialog::~PlotSettingsDialog()
{

}

void PlotSettingsDialog::setUI()
{

    QGridLayout *mainGrid =  new QGridLayout(this);

    int row = 0;
    int plotIndex = 0;
    for(auto& plot : vPlots)
    {
        QGroupBox *gb = new QGroupBox(this);

        QGridLayout *grid = new QGridLayout(gb);

        for(int i = 0; i < plot->graphCount(); i++)
        {

            auto graph = plot->graph(i); 
            auto gbGraph = new QGroupBox(title[plotIndex] +" "+
                                         QString::number(i+1)+" ступени",gb);
            auto gridGraph = new QGridLayout(gbGraph);

            QLabel *lblX = new QLabel(gbGraph);
            lblX->setText("X: ");
            lblX->setMaximumSize(QSize(20, 20));
            QLabel *lblY = new QLabel(gbGraph);
            lblY->setText("Y: ");
            lblY->setMaximumSize(QSize(20, 20));
            QLabel *lblWidth = new QLabel("Толщина линии",gbGraph);
            QLabel *lblColor = new QLabel("Текущий цвет отрисовки",gbGraph);

            QLineEdit *leX = new QLineEdit(gbGraph);
            QLineEdit *leY = new QLineEdit(gbGraph);
            leX->setText(plot->xAxis->label());
            leY->setText(plot->yAxis->label());

            QLineEdit *lineColor = new QLineEdit(gbGraph);
            lineColor->setReadOnly(true);
            lineColor->setMinimumSize(QSize(20, 20));
            lineColor->setMaximumSize(QSize(20, 20));
            lineColor->setCursor(QCursor(Qt::IBeamCursor));
            lineColor->setStyleSheet(
                        QString("background-color: %1;").arg(
                            graph->pen().color().name()));
            v_leColor.append(lineColor);

            QSpinBox *spBox = new QSpinBox(gbGraph);
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


            QPushButton *pbColor = new QPushButton("Выбрать цвет",gbGraph);
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

               gridGraph->addWidget(lblX,row+i,0);
               gridGraph->addWidget(leX,row+i,1);
               gridGraph->addWidget(lblWidth,row+i,2);
               gridGraph->addWidget(spBox,row+i,3);

               row++;

               gridGraph->addWidget(lblY,row+i,0);
               gridGraph->addWidget(leY,row+i,1);

               gridGraph->addWidget(lblColor,row+i,2);
               gridGraph->addWidget(pbColor,row+i,3);
               gridGraph->addWidget(lineColor,row+i,4);
               row++;


            grid->addWidget(gbGraph);
        }
          mainGrid->addWidget(gb);
          plotIndex++;

    }

    QDialogButtonBox *btnBox = new QDialogButtonBox(this);
    btnBox ->setOrientation(Qt::Horizontal);
    btnBox ->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    connect(btnBox,&QDialogButtonBox::accepted, this, &PlotSettingsDialog::accept);
    connect(btnBox, &QDialogButtonBox::rejected, this, &PlotSettingsDialog::reject);


    mainGrid->addWidget(btnBox);
}






































