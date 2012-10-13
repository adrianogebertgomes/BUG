/**
 * This file is a part of Qtpfsgui package.
 * ----------------------------------------------------------------------
 * Copyright (C) 2006,2007 Giuseppe Rota
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * ----------------------------------------------------------------------
 *
 * @author Giuseppe Rota <grota@users.sourceforge.net>
 */

#include <QColorDialog>
#include <QFileDialog>
#include <QWhatsThis>
#include <QMessageBox>
#include "preferencesDialog.h"
#include "../Common/config.h"
#include "../generated_uic/ui_documentation.h"

PreferenceDialog::PreferenceDialog(QWidget *p) : QDialog(p) {
	setupUi(this);

	fromIso639ToGuiIndex["cs"]=0;
	fromIso639ToGuiIndex["en"]=1;
	fromIso639ToGuiIndex["fr"]=2;
	fromIso639ToGuiIndex["de"]=3;
	fromIso639ToGuiIndex["id"]=4;
	fromIso639ToGuiIndex["it"]=5;
	fromIso639ToGuiIndex["pl"]=6;
	fromIso639ToGuiIndex["pt"]=7;
	fromIso639ToGuiIndex["ru"]=8;
	fromIso639ToGuiIndex["es"]=9;
	fromIso639ToGuiIndex["tr"]=10;
	fromIso639ToGuiIndex["hu"]=11;

	fromGuiIndexToIso639[0]="cs";
	fromGuiIndexToIso639[1]="en";
	fromGuiIndexToIso639[2]="fr";
	fromGuiIndexToIso639[3]="de";
	fromGuiIndexToIso639[4]="id";
	fromGuiIndexToIso639[5]="it";
	fromGuiIndexToIso639[6]="pl";
	fromGuiIndexToIso639[7]="pt";
	fromGuiIndexToIso639[8]="ru";
	fromGuiIndexToIso639[9]="es";
	fromGuiIndexToIso639[10]="tr";
	fromGuiIndexToIso639[11]="hu";

	qtpfsgui_options=QtpfsguiOptions::getInstance();
	negcolor=qtpfsgui_options->negcolor;
	infnancolor=qtpfsgui_options->naninfcolor;

	from_options_to_gui(); //update the gui in order to show the options
	connect(negativeColorButton,SIGNAL(clicked()),this,SLOT(negative_clicked()));
	connect(ifnanColorButton,SIGNAL(clicked()),this,SLOT(infnan_clicked()));
	connect(okButton,SIGNAL(clicked()),this,SLOT(ok_clicked()));
	connect(chooseCachePathButton,SIGNAL(clicked()),this,SLOT(updateLineEditString()));
	connect(helpDcrawParamsButton,SIGNAL(clicked()),this,SLOT(helpDcrawParamsButtonClicked()));
	connect(whatsThisButton,SIGNAL(clicked()),this,SLOT(enterWhatsThis()));

	Qt::ToolButtonStyle style = (Qt::ToolButtonStyle)settings.value(KEY_TOOLBAR_MODE,Qt::ToolButtonTextUnderIcon).toInt();
	whatsThisButton->setToolButtonStyle(style);
}

void PreferenceDialog::negative_clicked() {
	negcolor = QColorDialog::getColor(negcolor, this);
	change_color_of(negativeColorButton,&negcolor);
}
void PreferenceDialog::infnan_clicked() {
	infnancolor = QColorDialog::getColor(infnancolor, this);
	change_color_of(ifnanColorButton,&infnancolor);
}

void PreferenceDialog::change_color_of(QPushButton *button, QColor *newcolor) {
	if (newcolor->isValid()) {
		button->setStyleSheet(QString("background: rgb(%1,%2,%3)").arg(newcolor->red()).arg(newcolor->green()).arg(newcolor->blue()));
	}
}

QStringList PreferenceDialog::sanitizeDCRAWparams() {
	bool dcraw_opt_was_ok=false;
	QStringList temp_dcraw_options=dcrawParamsLineEdit->text().split(" ",QString::SkipEmptyParts);
	int idx_T=temp_dcraw_options.indexOf("-T");
	//if we don't have -T
	if (idx_T==-1) {
		temp_dcraw_options+="-T";
	} else {
		dcraw_opt_was_ok=true;
	}
	if (!dcraw_opt_was_ok) {
		QMessageBox::information(this,tr("Option -T..."),tr("Qtpfsgui requires dcraw to be executed with the \"-T\" option. Commandline options have been corrected."));
	}
	return temp_dcraw_options;

}

QStringList PreferenceDialog::sanitizeAISparams() {
	bool align_opt_was_ok=false;
	//check if we have '-a "aligned_"'
	QStringList temp_ais_options=aisParamsLineEdit->text().split(" ",QString::SkipEmptyParts);
	int idx_a=temp_ais_options.indexOf("-a");
	//if we don't have -a
	if (idx_a==-1) {
// 		qDebug("missing, adding");
		temp_ais_options+="-a";
		temp_ais_options+="aligned_";
	}
	//if we have -a at the very end (without the prefix)
	else if (idx_a==temp_ais_options.size()-1) {
		temp_ais_options+="aligned_";
// 		qDebug("-a at end, adding aligned_");
	}
	//if we have -a in the middle without the prefix
	else if ( (idx_a!=-1 && temp_ais_options.at(idx_a+1) != "aligned_") ) {
// 		qDebug("-a in the middle without the prefix after");
		if (!temp_ais_options.at(idx_a+1).startsWith("-")) {
// 			qDebug("next is bad prefix, removing");
			temp_ais_options.removeAt(idx_a+1);
		}
// 		qDebug("now adding");
		temp_ais_options.insert(idx_a+1,"aligned_");
	} else {
		align_opt_was_ok=true;
	}
	if (!align_opt_was_ok) {
		QMessageBox::information(this,tr("Option -a..."),tr("Qtpfsgui requires align_image_stack to be executed with the \"-a aligned_\" option. Commandline options have been corrected."));
	}
	return temp_ais_options;
}

void PreferenceDialog::ok_clicked() {
	if (qtpfsgui_options->gui_lang!=fromGuiIndexToIso639[languageComboBox->currentIndex()])
		QMessageBox::information(this,tr("Please restart..."),tr("Please restart Qtpfsgui to use the new language (%1).").arg(languageComboBox->currentText()));
	qtpfsgui_options->gui_lang=fromGuiIndexToIso639[languageComboBox->currentIndex()];
	settings.setValue(KEY_GUI_LANG,qtpfsgui_options->gui_lang);

	settings.beginGroup(GROUP_EXTERNALTOOLS);
		qtpfsgui_options->dcraw_options=sanitizeDCRAWparams();
		settings.setValue(KEY_EXTERNAL_DCRAW_OPTIONS,qtpfsgui_options->dcraw_options);

		qtpfsgui_options->align_image_stack_options=sanitizeAISparams();
		settings.setValue(KEY_EXTERNAL_AIS_OPTIONS,qtpfsgui_options->align_image_stack_options);
	settings.endGroup();

	settings.beginGroup(GROUP_HDRVISUALIZATION);
		if(negcolor.rgba() != qtpfsgui_options->negcolor) {
			qtpfsgui_options->negcolor=negcolor.rgba();
			settings.setValue(KEY_NEGCOLOR,negcolor.rgba());
		}
		if(infnancolor.rgba() != qtpfsgui_options->naninfcolor) {
			qtpfsgui_options->naninfcolor=infnancolor.rgba();
			settings.setValue(KEY_NANINFCOLOR,infnancolor.rgba());
		}
	settings.endGroup();

	settings.beginGroup(GROUP_TONEMAPPING);
		if (lineEditTempPath->text() != qtpfsgui_options->tempfilespath) {
			qtpfsgui_options->tempfilespath=lineEditTempPath->text();
			settings.setValue(KEY_TEMP_RESULT_PATH,lineEditTempPath->text());
		}
		if (batchLdrFormatComboBox->currentText() != qtpfsgui_options->batch_ldr_format) {
			qtpfsgui_options->batch_ldr_format=batchLdrFormatComboBox->currentText();
			settings.setValue(KEY_BATCH_LDR_FORMAT,batchLdrFormatComboBox->currentText());
		}
		if (numThreadspinBox->value() != qtpfsgui_options->num_threads) {
			qtpfsgui_options->num_threads=numThreadspinBox->value();
			settings.setValue(KEY_NUM_BATCH_THREADS,numThreadspinBox->value());
		}
	settings.endGroup();

	settings.beginGroup(GROUP_TIFF);
		if (logLuvRadioButton->isChecked() != qtpfsgui_options->saveLogLuvTiff) {
			qtpfsgui_options->saveLogLuvTiff=logLuvRadioButton->isChecked();
			settings.setValue(KEY_SAVE_LOGLUV,logLuvRadioButton->isChecked());
		}
	settings.endGroup();

	accept();
}

void PreferenceDialog::from_options_to_gui() {
	//language: if by any chance qtpfsgui_options->gui_lang does NOT contain one of the valid 2 chars
	//codes which are key for the fromIso639ToGuiIndex QMap, provide the default "en"
	if (!fromIso639ToGuiIndex.contains(qtpfsgui_options->gui_lang))
		qtpfsgui_options->gui_lang="en";
	languageComboBox->setCurrentIndex(fromIso639ToGuiIndex.value(qtpfsgui_options->gui_lang));
	lineEditTempPath->setText(qtpfsgui_options->tempfilespath);
	if (qtpfsgui_options->batch_ldr_format=="JPEG")
		batchLdrFormatComboBox->setCurrentIndex(0);
	else if (qtpfsgui_options->batch_ldr_format=="PNG")
		batchLdrFormatComboBox->setCurrentIndex(1);
	else if (qtpfsgui_options->batch_ldr_format=="PPM")
		batchLdrFormatComboBox->setCurrentIndex(2);
	else if (qtpfsgui_options->batch_ldr_format=="PBM")
		batchLdrFormatComboBox->setCurrentIndex(3);
	else if (qtpfsgui_options->batch_ldr_format=="BMP")
		batchLdrFormatComboBox->setCurrentIndex(4);
	numThreadspinBox->setValue(qtpfsgui_options->num_threads);
	dcrawParamsLineEdit->setText(qtpfsgui_options->dcraw_options.join(" "));
	aisParamsLineEdit->setText(qtpfsgui_options->align_image_stack_options.join(" "));
	logLuvRadioButton->setChecked(qtpfsgui_options->saveLogLuvTiff);
	floatTiffRadioButton->setChecked(!qtpfsgui_options->saveLogLuvTiff);
	change_color_of(negativeColorButton,&negcolor);
	change_color_of(ifnanColorButton,&infnancolor);
}

PreferenceDialog::~PreferenceDialog() {
}

void PreferenceDialog::updateLineEditString() {
	QString dir=QFileDialog::getExistingDirectory(
	this,
	tr("Choose a directory"),
	QDir::currentPath(),
	QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
	if (!dir.isEmpty()) {
		lineEditTempPath->setText(dir);
	}
}

void PreferenceDialog::helpDcrawParamsButtonClicked() {
	QDialog *help=new QDialog();
	help->setAttribute(Qt::WA_DeleteOnClose);
	Ui::HelpDialog ui;
	ui.setupUi(help);
	ui.tb->setSearchPaths(QStringList("/usr/share/qtpfsgui/html") << "/usr/local/share/qtpfsgui/html" << "./html");
	ui.tb->setSource(QUrl("dcraw.html"));
	help->exec();
}

void PreferenceDialog::enterWhatsThis() {
	QWhatsThis::enterWhatsThisMode();
}
