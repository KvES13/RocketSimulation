#include "flightsettings.h"
#include "ui_flightsettings.h"


FlightSettings::FlightSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FlightSettings)
{
    ui->setupUi(this);
    ui->sbTimeStep->setRange(0.0,1.0);
    ui->sbTimeStep->setValue(0.1);
    ui->sbTimeStep->setSingleStep(0.05);
    ui->leSaveFilePath->setText(qApp->applicationDirPath()+"/log" +
                                QDate::currentDate().toString("ddMMyy")+"_"+
                                QTime::currentTime().toString("hhmm")+".csv");

    ui->tabWidget->setCurrentIndex(0);
    ui->dateTimeLaunch->setDateTime(QDateTime::currentDateTime());

    v_wstages = {ui->stage1,ui->stage2,ui->stage3};
    for(auto& w: v_wstages) w->setEnabled(false);


}

FlightSettings::~FlightSettings()
{
    delete ui;
}

void FlightSettings::on_btnStartSimulation_clicked()
{
    emit startSim(v_stages,environment.get());
}

void FlightSettings::on_btnSelectCfgFile_clicked()
{
    const QString& title = QString::fromUtf8("Выберите конфигурационный файл");
    const QString& path   = qApp->applicationDirPath()+Constants::CfgPath;
    const QString& filters = QString::fromUtf8("Файлы Json (*.json)");
    QString filename = QFileDialog::getOpenFileName(this, title, path, filters);
    if(!filename.isEmpty())
        ui->leCfgFilePath->setText(filename);
}

void FlightSettings::on_btnLoadCfg_clicked()
{
    QString jsonFilePath = ui->leCfgFilePath->text();
    if(jsonFilePath.isEmpty())
    {
        QMessageBox::warning(this,"Warning","Filepath is empty\n" + jsonFilePath,QMessageBox::Ok);
        return;
    }
    ui->btnStartSimulation->setEnabled(true);

    JsonWrapper json(jsonFilePath);
    int numberOfStages =json.getInt("Number of Stage");
    ui->sbStagesNumber->setValue(numberOfStages);

    QString filePath = qApp->applicationDirPath() + Constants::CfgPath+"/";
    for(int i = 1; i <= numberOfStages; i++)
    {
        auto jsStage = json.getSubItem("Stage"+QString::number(numberOfStages)+" Config File List");
        v_stages.push_back(
                    RocketStage::create(i,
                    filePath + jsStage.getString("Rocket Configuration File Path"),
                    filePath + jsStage.getString("Engine Configuration File Path"),
                    filePath + jsStage.getString("Sequence of Event File Path")));

        auto& stage = v_stages[i-1];
        stage.fdr = FlightObserver(stage.rocket.get());
        stage.fdr.filePath = ui->leSaveFilePath->text().toStdString();

        v_wstages[i-1]->setEnabled(true);
        v_wstages[i-1]->fillWidget(stage);

    }

    environment = std::make_shared<Environment>();
    environment->masterClock = SequenceClock(ui->dateTimeLaunch->dateTime(),0.0);

    auto rocketFirstStage = v_stages[0].rocket.get();

    JsonWrapper jsLaunch = json.getSubItem("Launch Condition");

    //Стартовая позиция
    Eigen::Vector3d posInitLLH {jsLaunch.getDouble("Latitude [deg]"),
                           jsLaunch.getDouble("Longitude [deg]"),
                           jsLaunch.getDouble("Height for WGS84 [deg]")};
    rocketFirstStage->position.Initialize(environment->masterClock.UTC_date_init,
                                          posInitLLH);


    //Стартовая скорость
    Eigen::Vector3d velInitNED  {jsLaunch.getDouble("North Velocity [m/s]"),
                            jsLaunch.getDouble("East Velocity [m/s]"),
                            jsLaunch.getDouble("Down Velocity [m/s]")};
    rocketFirstStage->velocity.Initialize(environment->masterClock.UTC_date_init,
                                          velInitNED,
                                          posInitLLH,
                                          rocketFirstStage->position.ECI);

    //Углы эйлера
    Eigen::Vector3d euler {jsLaunch.getDouble("Azimuth [deg]"),
                    jsLaunch.getDouble("Elevation [deg]"),
                    0};

    ui->leStartAzimuth->setText(QString::number(euler[0]));
    ui->leStartElevation->setText(QString::number(euler[1]));

    euler = euler / 180.0 * Constants::pi;
    rocketFirstStage->attitude.Initialize(euler);

    rocketFirstStage->angular_acceleration = {0.0,0.0,0.0};


    ui->leStartLatitude->setText(QString::number(posInitLLH[0]));
    ui->leStartLongtitude->setText(QString::number(posInitLLH[1]));
    ui->leStartAltitude->setText(QString::number(posInitLLH[2]));

    ui->leStartVelosityX->setText(QString::number(velInitNED[0]));
    ui->leStartVelosityY->setText(QString::number(velInitNED[1]));
    ui->leStartVelosityZ->setText(QString::number(velInitNED[2]));
}


void FlightSettings::on_cbSaveSim_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
    {
        ui->leSaveFilePath->setEnabled(true);
        ui->btnSelectSaveFile->setEnabled(true);
    }
    else
    {
        ui->leSaveFilePath->setEnabled(false);
        ui->btnSelectSaveFile->setEnabled(false);
    }
}

void FlightSettings::on_btnSelectSaveFile_clicked()
{
    const QString& title = QString::fromUtf8("Выберите файл для сохранения");
    const QString& path   = qApp->applicationDirPath()+Constants::OutputPath;
    const QString& filters = QString::fromUtf8("Файлы (*.csv)");
    QString filename = QFileDialog::getOpenFileName(this, title, path, filters);
    if(!filename.isEmpty())
        ui->leSaveFilePath->setText(filename);
}

void FlightSettings::on_sbStagesNumber_valueChanged(int arg1)
{
//    if(arg1 == 1)
//        ui->gb2Stage->setEnabled(false);
//    else
//        ui->gb2Stage->setEnabled(true);
}

void FlightSettings::on_leStartLongtitude_editingFinished()
{
    qDebug()<<" on_leStartLongtitude_editingFinished()";
    //Стартовая позиция
    v_stages[0].rocket->
            position.Initialize(environment->masterClock.UTC_date_init,
                                {ui->leStartLatitude->text().toDouble(),
                                 ui->leStartLongtitude->text().toDouble(),
                                 ui->leStartAltitude->text().toDouble()});
}


void FlightSettings::on_leStartLatitude_editingFinished()
{
    qDebug()<<" on_leStartLatitude_editingFinished()";
    //Стартовая позиция
    v_stages[0].rocket->
            position.Initialize(environment->masterClock.UTC_date_init,
                                {ui->leStartLatitude->text().toDouble(),
                                 ui->leStartLongtitude->text().toDouble(),
                                 ui->leStartAltitude->text().toDouble()});
}


void FlightSettings::on_leStartAltitude_editingFinished()
{
    qDebug()<<" on_leStartAltitude_editingFinished()";
    //Стартовая позиция
    v_stages[0].rocket->
            position.Initialize(environment->masterClock.UTC_date_init,
                                {ui->leStartLatitude->text().toDouble(),
                                 ui->leStartLongtitude->text().toDouble(),
                                 ui->leStartAltitude->text().toDouble()});
}


void FlightSettings::on_leStartAzimuth_editingFinished()
{
    //Углы эйлера
    Eigen::Vector3d euler {ui->leStartAzimuth->text().toDouble(),
                    ui->leStartElevation->text().toDouble(),
                    0};
    euler = euler / 180.0 * Constants::pi;
    v_stages[0].rocket->attitude.Initialize(euler);
}


void FlightSettings::on_leStartElevation_editingFinished()
{
    //Углы эйлера
    Eigen::Vector3d euler {ui->leStartAzimuth->text().toDouble(),
                    ui->leStartElevation->text().toDouble(),
                    0};
    euler = euler / 180.0 * Constants::pi;
    v_stages[0].rocket->attitude.Initialize(euler);
}


//void FlightSettings::on_leDiameter1_editingFinished()
//{
//    v_stages[0].rocket->diameter = ui->leDiameter1->text().toDouble();
//    v_stages[0].rocket->area = 0.25 * std::pow(v_stages[0].rocket->diameter, 2) * Constants::pi;
//}


//void FlightSettings::on_leLenght1_editingFinished()
//{
// v_stages[0].rocket->length = ui->leLenght1->text().toDouble();
//}


//void FlightSettings::on_leInert1_editingFinished()
//{
// v_stages[0].rocket->mass.inert = ui->leInert1->text().toDouble();
//}


//void FlightSettings::on_lePropellant1_editingFinished()
//{
// v_stages[0].rocket->mass.propellant = ui->lePropellant1->text().toDouble();
//}

