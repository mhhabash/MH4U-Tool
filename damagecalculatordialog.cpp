#include <iostream>
#include "damagecalculatordialog.hpp"
#include "ui_damagecalculatordialog.h"

damagecalculatordialog::damagecalculatordialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::damagecalculatordialog)
{
    ui->setupUi(this);

    // Set up the "None" radio buttons
    ui->AuXNoneRadioButton->setChecked(true);
    ui->CENoneRadioButton->setChecked(true);
    ui->gloveNoneRadioButton->setChecked(true);
    ui->elementNoneRadioButton->setChecked(true);
    ui->HHNoneRadioButton->setChecked(true);
    ui->dangerNoneRadioButton->setChecked(true);
    ui->kitchenNoneRadioButton->setChecked(true);
    ui->mightNoneRadioButton->setChecked(true);

    // When HHNoneRadioButton is checked, replayCheckBox is disabled
    ui->replayCheckBox->setDisabled(true);
}

damagecalculatordialog::~damagecalculatordialog()
{
    delete ui;
}

QSqlQuery runQuery(const QString& sql)
{
    QSqlQuery query(sql);
    query.exec();
    return query;
}


QSqlQuery damagecalculatordialog::createAndRunSpecificQuery(const QString& selectSQL)
{
    QString sql = "SELECT ";
    QSqlQuery result;

    sql.append(selectSQL);
    sql.append(" FROM ");
    sql.append(this->weaponType);
    sql.append(" WHERE name=\"");
    sql.append(this->weapon);
    sql.append("\"");
    result = runQuery(sql);
    result.next();

    return result;
}

void damagecalculatordialog::on_weapon_type_list_widget_clicked(const QModelIndex &index)
{
    QSqlQuery query;
    QString sql = "SELECT name FROM ";

    // Find out if the weapon_list_widget is empty, and if not empty it
    if (ui->weapon_list_widget->count() > 0)
        ui->weapon_list_widget->clear();

    // Find out if motionValueListWidget is empty, and if not empty it
    if (ui->motionValueListWidget->count() > 0)
        ui->motionValueListWidget->clear();

    // Update the contents of the weapon_list_widget
    this->weaponType = index.data(Qt::DisplayRole).toString();
    sql.append(this->weaponType);
    query = runQuery(sql);
    while(query.next())
        ui->weapon_list_widget->addItem(query.value(0).toString());

    // Update the contents of the motionValueListWidget
    // TODO: Finish this
    if (this->weaponType == "ChargeBlades")
    {
        QString attacks[] = {"Draw (Sword) [22]", "Upward Slash [12]", "Back Slash [17]",
                            "Round Slash [30]", "Charge [16]", "Charge 1 [30]",
                            "Charge 2 [20]", "Jump (Sword) [22]", "Draw (Axe) [41]",
                            "Forward Slam [44]", "Upward Swipe [28]", "Downward Chop [41]",
                            "Jump (Axe) [44]", "Burst 1 [14/23]", "Burst 2 [61/78]", "Burst 3 [40/80]",
                            "Ultra Burst [maxInt]"};
        int attacksSize = 17;
        for (int i = 0; i < attacksSize; ++i)
        {
            ui->motionValueListWidget->addItem(attacks[i]);
        }
    }
}

void damagecalculatordialog::on_sharpnessCheckBox_clicked()
{
    this->sharpness = ui->sharpnessCheckBox->isChecked();
}

void damagecalculatordialog::on_weapon_list_widget_clicked(const QModelIndex &index)
{
    this->weapon = index.data(Qt::DisplayRole).toString();
}

void damagecalculatordialog::on_HHNoneRadioButton_clicked()
{
    ui->replayCheckBox->setDisabled(true);
}

void damagecalculatordialog::on_attackUpSRadioButton_clicked()
{
    ui->replayCheckBox->setDisabled(false);
    if (!ui->attackUpSRadioButton->isChecked() && !ui->attackUpLRadioButton->isChecked() &&
            !ui->elementUpCheckBox->isChecked() && !ui->affinityUpCheckBox->isChecked())
        ui->replayCheckBox->setDisabled(true);
}

void damagecalculatordialog::on_attackUpLRadioButton_clicked()
{
    ui->replayCheckBox->setDisabled(false);
    if (!ui->attackUpSRadioButton->isChecked() && !ui->attackUpLRadioButton->isChecked() &&
            !ui->elementUpCheckBox->isChecked() && !ui->affinityUpCheckBox->isChecked())
        ui->replayCheckBox->setDisabled(true);
}

void damagecalculatordialog::on_affinityUpCheckBox_clicked()
{
    ui->replayCheckBox->setDisabled(false);
    if (!ui->attackUpSRadioButton->isChecked() && !ui->attackUpLRadioButton->isChecked() &&
            !ui->elementUpCheckBox->isChecked() && !ui->affinityUpCheckBox->isChecked())
        ui->replayCheckBox->setDisabled(true);
}

void damagecalculatordialog::on_elementUpCheckBox_clicked()
{
    ui->replayCheckBox->setDisabled(false);
    if (!ui->attackUpSRadioButton->isChecked() && !ui->attackUpLRadioButton->isChecked() &&
            !ui->elementUpCheckBox->isChecked() && !ui->affinityUpCheckBox->isChecked())
        ui->replayCheckBox->setDisabled(true);
}

void damagecalculatordialog::on_caclulatePushButton_clicked()
{
    std::map<std::string, std::map<std::string, float> > allModifiers;
    std::map<std::string, float> rawModifiers;
    std::map<std::string, float> eleModifiers;
    std::map<std::string, float> affinityModifiers;
    std::map<std::string, float> rawHitzoneModifiers;
    std::map<std::string, float> eleHitzoneModifiers;
    QSqlQuery query;

    // The following gets messy...
    // Need to get all raw modifiers
    query = createAndRunSpecificQuery("attack");
    rawModifiers["attack"] = query.value(0).toString().toFloat();

    if (ui->AuSRadioButton->isChecked())
        rawModifiers["AuX"] = 10;
    else if (ui->AuMRadioButton->isChecked())
        rawModifiers["AuX"] = 15;
    else if (ui->AuLRadioButton->isChecked())
        rawModifiers["AuX"] = 20;
    else if (ui->AuXLRadioButton->isChecked())
        rawModifiers["AuX"] = 25;
    else
        rawModifiers["AuX"] = 0;

    if (ui->AuSKitchenRadioButton->isChecked())
        rawModifiers["kitchen"] = 5;
    else if (ui->AuLKitchenRadioButton->isChecked())
        rawModifiers["kitchen"] = 7;
    else
        rawModifiers["kitchen"] = 0;

    if (ui->mightPillRadioButton->isChecked())
        rawModifiers["seed"] = 25;
    else if (ui->mightSeedRadioButton->isChecked())
        rawModifiers["seed"] = 10;
    else
        rawModifiers["seed"] = 0;

    if (ui->challenger1RadioButton->isChecked())
        rawModifiers["glove"] = 10;
    else if (ui->challenger2RadioButton->isChecked())
        rawModifiers["glove"] = 25;
    else if (ui->peakPerformanceRadioButton->isChecked())
        rawModifiers["glove"] = 20;
    else
        rawModifiers["glove"] = 0;

    if (ui->powerCharmCheckBox->isChecked())
        rawModifiers["powerCharm"] = 6;
    else
        rawModifiers["powerCharm"] = 0;

    if (ui->powerTalonCheckBox->isChecked())
        rawModifiers["powerTalon"] = 9;
    else
        rawModifiers["powerTalon"] = 0;

    if (ui->attackUpSRadioButton->isChecked())
        rawModifiers["HH"] = 1.10;
    else if (ui->attackUpLRadioButton->isChecked())
        rawModifiers["HH"] = 1.15;
    else
        rawModifiers["HH"] = 1;

    if (ui->replayCheckBox->isChecked() && rawModifiers["HH"] > 1)
        rawModifiers["replay"] = 0.05;
    else
        rawModifiers["replay"] = 0;

    if (ui->adrenaline2RadioButton->isChecked())
        rawModifiers["danger"] = 1.30;
    else if (ui->heroicsRadioButton->isChecked())
        rawModifiers["danger"] = 1.35;
    else
        rawModifiers["danger"] = 1;

    if (ui->faint1RadioButton->isChecked())
        rawModifiers["fortify"] = 1.1;
    else if (ui->faint2RadioButton->isChecked())
        rawModifiers["fortify"] = 1.2;
    else
        rawModifiers["fortify"] = 1;

    // Need to get all element modifiers
    query = createAndRunSpecificQuery("special attack");
    eleModifiers["specialAttack"] = query.value(0).toString().toFloat();

    if (ui->element1RadioButton->isChecked())
    {
        eleModifiers["elementAdd"] = 40;
        eleModifiers["elementMul"] =  1.05;
    }
    else if (ui->element2RadioButton->isChecked())
    {
        eleModifiers["elementAdd"] = 60;
        eleModifiers["elementMul"] = 1.10;
    }
    else if (ui->element3RadioButton->isChecked())
    {
        eleModifiers["elementAdd"] = 90;
        eleModifiers["elementMul"] = 1.15;
    }
    else
    {
        eleModifiers["elementAdd"] = 0;
        eleModifiers["elementMul"] = 1;
    }

    if (ui->elementAtkUpCheckBox->isChecked())
        eleModifiers["elementAtkUp"] = 1.1;
    else
        eleModifiers["elementAtkUp"] = 1;

    if (ui->elementUpCheckBox->isChecked())
        eleModifiers["elementUp"] = 1.1;
    else
        eleModifiers["elementUp"] = 1;

    if (ui->replayCheckBox->isChecked() && eleModifiers["elementUp"] > 1)
        eleModifiers["replay"] = 0.05;
    else
        eleModifiers["replay"] = 0;


    // Need to get all affinity modifiers
    query = createAndRunSpecificQuery("affinity");
    affinityModifiers["affinity"] = query.value(0).toString().toFloat();

    if (ui->criticalEye1RadioButton->isChecked())
        affinityModifiers["criticalEye"] = 0.10;
    else if (ui->criticalEye2RadioButton->isChecked())
        affinityModifiers["criticalEye"] = 0.15;
    else if (ui->criticalEye3RadioButton->isChecked())
        affinityModifiers["criticalEye"] = 0.20;
    else if (ui->criticalGodRadioButton->isChecked())
        affinityModifiers["criticalEye"] = 0.30;
    else
        affinityModifiers["criticalEye"] = 0;

    if (ui->latentPower1RadioButton->isChecked())
        affinityModifiers["glove"] = 0.30;
    else if (ui->latentPower2RadioButton->isChecked())
        affinityModifiers["glove"] = 0.50;
    else if (ui->challenger1RadioButton->isChecked())
        affinityModifiers["glove"] = 0.10;
    else if (ui->challenger2RadioButton->isChecked())
        affinityModifiers["glove"] = 0.20;
    else
        affinityModifiers["glove"] = 0;

    if (ui->criticalDrawCheckBox->isChecked())
        affinityModifiers["criticalDraw"] = 1;
    else
        affinityModifiers["criticalDraw"] = 0;

    if (ui->affinityUpCheckBox->isChecked())
        affinityModifiers["affinityUp"] = 0.3;
    else
        affinityModifiers["affinityUp"] = 0;
}

