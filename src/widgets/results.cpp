#include "results.h"
#include "ui_results.h"

Results::Results(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Results)
{
    ui->setupUi(this);
}

Results::~Results()
{
    delete ui;
}

void Results::SetFilePath(const QString &filepath, QVector<FlightObserver*>obs)
{
    CsvStageFilePath = filepath;
    ui->leCSVfile->setText(CsvStageFilePath);
    observers = obs;
    stagesCount = observers.size();
}

void Results::paintResults()
{
    plotPosition();
    plotVelocity();
    plotForce();
    plotAcceleration();
    plotMass();
    plotAmtosphere();
}

void Results::plotVelocity()
{
    QTabWidget *tabVelocity = new QTabWidget(ui->tabWidget);
    ui->tabWidget->addTab(tabVelocity,"Скорость");

    for(auto &fdr: observers)
    {
        vVelocityECEF.push_back(fdr->countup_time);
        vVelocityECEF.push_back(std::move(fdr->vvelocity[0]));
        vVelocityECEF.push_back(std::move(fdr->vvelocity[1]));
        vVelocityECEF.push_back(std::move(fdr->vvelocity[2]));

        vVelocityECI.push_back(fdr->countup_time);
        vVelocityECI.push_back(std::move(fdr->vvelocity[3]));
        vVelocityECI.push_back(std::move(fdr->vvelocity[4]));
        vVelocityECI.push_back(std::move(fdr->vvelocity[5]));

        vVelocityNED.push_back(fdr->countup_time);
        vVelocityNED.push_back(std::move(fdr->vvelocity[6]));
        vVelocityNED.push_back(std::move(fdr->vvelocity[7]));
        vVelocityNED.push_back(std::move(fdr->vvelocity[8]));
    }
    wResults  *resECEF = new wResults(vVelocityECEF,
                             QStringList{"Время","X","Y","Z"},
                              stagesCount,tabVelocity);
    tabVelocity->addTab(resECEF,"Скорость ECEF");

    wResults  *resECI = new wResults(vVelocityECI,
                                     QStringList{"Время","X","Y","Z"},
                                     stagesCount,tabVelocity);
    tabVelocity->addTab(resECI,"Скорость ECI");

    wResults  *resNED = new wResults(vVelocityNED,
                                 QStringList{"Время",
                                             "North Velocity [m/s]",
                                             "East Velocity [m/s]",
                                             "Down Velocity [m/s]"},
                                 stagesCount,tabVelocity);
    tabVelocity->addTab(resNED,"NED");
}

void Results::plotPosition()
{
    QTabWidget *tabPosition = new QTabWidget(ui->tabWidget);
    ui->tabWidget->addTab(tabPosition,"Координаты");
    for(auto &fdr: observers)
    {
        vPosECEF.push_back(fdr->countup_time);
        vPosECEF.push_back(std::move(fdr->vposition[0]));
        vPosECEF.push_back(std::move(fdr->vposition[1]));
        vPosECEF.push_back(std::move(fdr->vposition[2]));

        vPosECI.push_back(fdr->countup_time);
        vPosECI.push_back(std::move(fdr->vposition[3]));
        vPosECI.push_back(std::move(fdr->vposition[4]));
        vPosECI.push_back(std::move(fdr->vposition[5]));

        vPosLLH.push_back(fdr->countup_time);
        vPosLLH.push_back(std::move(fdr->vposition[6]));
        vPosLLH.push_back(std::move(fdr->vposition[7]));
        vPosLLH.push_back(std::move(fdr->vposition[8]));
    }
    wResults  *posECEF = new wResults(vPosECEF,
                                     QStringList{"Время","X","Y","Z"},
                                      stagesCount,tabPosition);
    tabPosition->addTab(posECEF,"Координаты ECEF");

    wResults  *posECI = new wResults(vPosECI,
                                     QStringList{"Время","X","Y","Z"},
                                     stagesCount,tabPosition);
    tabPosition->addTab(posECI,"Координаты ECI");

    wResults  *posLLH = new wResults(vPosLLH,
                                     QStringList{"Время",
                                                 "Latitude [deg]",
                                                 "Longitude [deg]",
                                                 "Height for WGS84 [deg]"},
                                     stagesCount,tabPosition);
    tabPosition->addTab(posLLH,"Координаты LLH");
}

void Results::plotForce()
{
    QTabWidget *tabForce = new QTabWidget(ui->tabWidget);
    ui->tabWidget->addTab(tabForce,"Силы");

    for(auto &fdr: observers)
    {
    vForce.push_back(fdr->countup_time);
    vForce.push_back(std::move(fdr->thrust));

    vForceThrust.push_back(fdr->countup_time);
    vForceThrust.push_back(std::move(fdr->vforce[0]));
    vForceThrust.push_back(std::move(fdr->vforce[1]));
    vForceThrust.push_back(std::move(fdr->vforce[2]));

    vForceAero.push_back(fdr->countup_time);
    vForceAero.push_back(std::move(fdr->vforce[3]));
    vForceAero.push_back(std::move(fdr->vforce[4]));
    vForceAero.push_back(std::move(fdr->vforce[5]));

    vForceGravity.push_back(fdr->countup_time);
    vForceGravity.push_back(std::move(fdr->vforce[6]));
    vForceGravity.push_back(std::move(fdr->vforce[7]));
    vForceGravity.push_back(std::move(fdr->vforce[8]));

    }
    wResults  *resForce = new wResults(vForce,
                                  QStringList{"Время","Thrust [N]]"},
                                       stagesCount,tabForce);
    tabForce->addTab(resForce,"Общая");

    wResults  *resThrust = new wResults(vForceThrust,
                                  QStringList{"Время","Fx-thrust [N]",
                                              "Fy-thrust [N]","Fz-thrust [N]"},
                                        stagesCount,tabForce);
    tabForce->addTab(resThrust,"Сила тяги");

    wResults  *resAero = new wResults(vForceAero,
                                  QStringList{"Время","Fx-aero [N]",
                                              "Fy-aero [N]","Fz-aero [N]"},
                                      stagesCount,tabForce);
    tabForce->addTab(resAero,"Аэродинамические силы");

    wResults  *resGravity = new wResults(vForceGravity,
                                  QStringList{"Время","Fx-gravity [N]",
                                              "Fy-gravity [N]","Fz-gravity [N]"},
                                         stagesCount,tabForce);
    tabForce->addTab(resGravity,"Вес");
}

void Results::plotAcceleration()
{
    QTabWidget *tabAc = new QTabWidget(ui->tabWidget);
    ui->tabWidget->addTab(tabAc,"Ускорение");

    for(auto &fdr: observers)
    {
        vAcECI.push_back(fdr->countup_time);
        vAcECI.push_back(std::move(fdr->vacceleration[0]));
        vAcECI.push_back(std::move(fdr->vacceleration[1]));
        vAcECI.push_back(std::move(fdr->vacceleration[2]));

        vAcBody.push_back(fdr->countup_time);
        vAcBody.push_back(std::move(fdr->vacceleration[3]));
        vAcBody.push_back(std::move(fdr->vacceleration[4]));
        vAcBody.push_back(std::move(fdr->vacceleration[5]));
    }
    wResults  *resAcECI = new wResults(vAcECI,
                                      QStringList{"Время","X","Y","Z"},
                                       stagesCount,tabAc);
    tabAc->addTab(resAcECI,"ECI");

    wResults  *resAcBody = new wResults(vAcBody,
                                      QStringList{"Время","Accx-body [m/s2]",
                                                  "Accy-body [m/s2]",
                                                  "Accz-body [m/s2]"},
                                        stagesCount,tabAc);
    tabAc->addTab(resAcBody,"body");

}

void Results::plotMass()
{
    for(auto &fdr: observers)
    {
        vmass.push_back(fdr->countup_time);
        vmass.push_back(std::move(fdr->mass));
        vmass.push_back(std::move(fdr->mass_prop));
    }
    wResults *resMass = new wResults(vmass,QStringList{"Время","Mass [kg]",
                                                       "Propellant Mass [kg]"},
                                     stagesCount,this);
    ui->tabWidget->addTab(resMass,"Масса");

}

void Results::plotAmtosphere()
{
    Environment env;
    QVector<double> temp,alt,denst,press;
    for(auto &fdr: observers)
    {
        for(int i = 0; i < fdr->countup_time.size();i++)
        {
             env.updateAltitude(fdr->position[i].LLH[2]);
             alt.push_back(fdr->position[i].LLH[2]);
             temp.push_back(env.atmosphere.getTemperature());
             denst.push_back(env.atmosphere.getDensity());
             press.push_back(env.atmosphere.getPressure());
        }
        vvv.push_back(alt);
        vvv.push_back(temp);
        vvv.push_back(denst);
        vvv.push_back(press);
    }

    wResults *res = new wResults(vvv,QStringList{"Высота","Температура",
                                                 "Плотность","Давление"},
                                 stagesCount,this);
    ui->tabWidget->addTab(res,"Атмосфера");
}
