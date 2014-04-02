/*
 * Copyright (c) 2013-2014 Thomas Isaac Lightburn
 *
 *
 * This file is part of OpenKJ.
 *
 * OpenKJ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <QFontDialog>
#include <QColorDialog>

extern KhSettings *settings;


SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    createIcons();
    QStringList screens = getMonitors();
    ui->listWidgetMonitors->addItems(screens);
//    settings = new KhSettings(this);
    ui->checkBoxShowCdgWindow->setChecked(settings->showCdgWindow());
    ui->groupBoxMonitors->setChecked(settings->cdgWindowFullscreen());
    ui->listWidgetMonitors->setEnabled(settings->showCdgWindow());
    ui->groupBoxMonitors->setEnabled(settings->showCdgWindow());
    ui->listWidgetMonitors->item(settings->cdgWindowFullScreenMonitor())->setSelected(true);
    ui->spinBoxTickerHeight->setValue(settings->tickerHeight());
    ui->horizontalSliderTickerSpeed->setValue(settings->tickerSpeed());
    QPalette txtpalette = ui->pushButtonTextColor->palette();
    txtpalette.setColor(QPalette::Button, settings->tickerTextColor());
    ui->pushButtonTextColor->setPalette(txtpalette);
    QPalette bgpalette = ui->pushButtonBgColor->palette();
    bgpalette.setColor(QPalette::Button, settings->tickerBgColor());
    ui->pushButtonBgColor->setPalette(bgpalette);
    if (settings->tickerFullRotation())
    {
        ui->radioButtonFullRotation->setChecked(true);
        ui->spinBoxTickerSingers->setEnabled(false);
    }
    else
    {
        ui->radioButtonPartialRotation->setChecked(true);
        ui->spinBoxTickerSingers->setEnabled(true);
    }
    ui->spinBoxTickerSingers->setValue(settings->tickerShowNumSingers());
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

QStringList SettingsDialog::getMonitors()
{
    QStringList screenStrings;
    for (int i=0; i < QApplication::desktop()->screenCount(); i++)
    {
        int sWidth = QApplication::desktop()->screenGeometry(i).width();
        int sHeight = QApplication::desktop()->screenGeometry(i).height();
        screenStrings << "Monitor " + QString::number(i) + " - " + QString::number(sWidth) + "x" + QString::number(sHeight);
    }
    return screenStrings;
}


void SettingsDialog::createIcons()
{
    QListWidgetItem *audioButton = new QListWidgetItem(ui->listWidget);
    audioButton->setIcon(QIcon(":/icons/Icons/audio-card.png"));
    audioButton->setText(tr("Audio"));
    audioButton->setTextAlignment(Qt::AlignHCenter);
    audioButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    QListWidgetItem *videoButton = new QListWidgetItem(ui->listWidget);
    videoButton->setIcon(QIcon(":/icons/Icons/video-display.png"));
    videoButton->setText(tr("Video"));
    videoButton->setTextAlignment(Qt::AlignHCenter);
    videoButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

void SettingsDialog::on_btnClose_clicked()
{
    close();
}

void SettingsDialog::on_checkBoxShowCdgWindow_stateChanged(int arg1)
{
    settings->setShowCdgWindow(arg1);
    emit showCdgWindowChanged(arg1);
    ui->listWidgetMonitors->setEnabled(arg1);
    ui->groupBoxMonitors->setEnabled(arg1);
}

void SettingsDialog::on_groupBoxMonitors_toggled(bool arg1)
{
    settings->setCdgWindowFullscreen(arg1);
    emit cdgWindowFullScreenChanged(arg1);
}

void SettingsDialog::on_listWidgetMonitors_itemSelectionChanged()
{
    int selMonitor = ui->listWidgetMonitors->selectionModel()->selectedIndexes().at(0).row();
    settings->setCdgWindowFullscreenMonitor(selMonitor);
    emit cdgWindowFullScreenMonitorChanged(selMonitor);
}

void SettingsDialog::on_pushButtonFont_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, settings->tickerFont(), this, "Select ticker font");
    if (ok)
    {
        settings->setTickerFont(font);
    }
}

void SettingsDialog::on_spinBoxTickerHeight_valueChanged(int arg1)
{
    settings->setTickerHeight(arg1);
}

void SettingsDialog::on_horizontalSliderTickerSpeed_valueChanged(int value)
{
    settings->setTickerSpeed(value);
}

void SettingsDialog::on_pushButtonTextColor_clicked()
{
    QColor color = QColorDialog::getColor(settings->tickerTextColor(),this,"Select ticker text color");
    if (color.isValid())
    {
        settings->setTickerTextColor(color);
        QPalette palette = ui->pushButtonTextColor->palette();
        palette.setColor(ui->pushButtonTextColor->backgroundRole(), color);
        ui->pushButtonTextColor->setPalette(palette);
    }

//    QColor color = QColorDialog::getColor()
}

void SettingsDialog::on_pushButtonBgColor_clicked()
{
    QColor color = QColorDialog::getColor(settings->tickerTextColor(),this,"Select ticker background color");
    if (color.isValid())
    {
        settings->setTickerBgColor(color);
        QPalette palette = ui->pushButtonBgColor->palette();
        palette.setColor(ui->pushButtonBgColor->backgroundRole(), color);
        ui->pushButtonBgColor->setPalette(palette);
    }
}

void SettingsDialog::on_radioButtonFullRotation_toggled(bool checked)
{
    settings->setTickerFullRotation(checked);
    ui->spinBoxTickerSingers->setEnabled(!checked);
}



void SettingsDialog::on_spinBoxTickerSingers_valueChanged(int arg1)
{
    settings->setTickerShowNumSingers(arg1);
}
