/**
 * This file is a part of Qtpfsgui package.
 * ----------------------------------------------------------------------
 * Copyright (C) 2007 Giuseppe Rota
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

#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <image.hpp>
#include <exif.hpp>
#include <iostream>
#include "transplant.h"
#include "../generated_uic/ui_documentation.h"
#include "../Exif/exif_operations.h"
#include "../Common/global.h"
#include "../Common/config.h"

TransplantExifDialog::TransplantExifDialog(QWidget *p) : QDialog(p), start_left(-1), stop_left(-1), start_right(-1), stop_right(-1), done(false) {
	setupUi(this);
	Log_Widget->setWordWrap(true);
	connect(moveup_left_button,	SIGNAL(clicked()),this,SLOT(moveup_left()));
	connect(moveup_right_button,	SIGNAL(clicked()),this,SLOT(moveup_right()));
	connect(movedown_left_button,	SIGNAL(clicked()),this,SLOT(movedown_left()));
	connect(movedown_right_button,	SIGNAL(clicked()),this,SLOT(movedown_right()));
	connect(removeleft,		SIGNAL(clicked()),this,SLOT(remove_left()));
	connect(removeright,		SIGNAL(clicked()),this,SLOT(remove_right()));
	connect(addleft,		SIGNAL(clicked()),this,SLOT(append_left()));
	connect(addright,		SIGNAL(clicked()),this,SLOT(append_right()));
	connect(TransplantButton,	SIGNAL(clicked()),this,SLOT(transplant_requested()));
	connect(HelpButton,		SIGNAL(clicked()),this,SLOT(help_requested()));

	connect(filterComboBox, SIGNAL(activated(int)), this, SLOT(filterComboBoxActivated(int)));
	connect(filterLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(filterChanged(const QString&)));
	full_Log_Model=new QStringListModel();
	log_filter=new QSortFilterProxyModel(this);
	log_filter->setDynamicSortFilter(true);
	log_filter->setSourceModel(full_Log_Model);
	Log_Widget->setModel(log_filter);

	RecentDirEXIFfrom=settings.value(KEY_RECENT_PATH_EXIF_FROM,QDir::currentPath()).toString();
	RecentDirEXIFto=settings.value(KEY_RECENT_PATH_EXIF_TO,QDir::currentPath()).toString();
}

TransplantExifDialog::~TransplantExifDialog() {
	leftlist->clear();
	rightlist->clear();
}

void TransplantExifDialog::help_requested() {
	QDialog *help=new QDialog();
	help->setAttribute(Qt::WA_DeleteOnClose);
	Ui::HelpDialog ui;
	ui.setupUi(help);
	ui.tb->setSearchPaths(QStringList("/usr/share/qtpfsgui/html") << "/usr/local/share/qtpfsgui/html" << "./html");
	ui.tb->setSource(QUrl("manual.html#copyexif"));
	help->exec();
}

void TransplantExifDialog::updateinterval(bool left) {
	if (left) {
		start_left=leftlist->count();
		stop_left=-1;
		for (int i=0; i<leftlist->count(); i++) {
			if (leftlist->isItemSelected(leftlist->item(i))) {
				start_left= (start_left>i) ? i : start_left;
				stop_left= (stop_left<i) ? i : stop_left;
			}
		}
// 		qDebug("L %d-%d",start_left,stop_left);
	} else {
		start_right=rightlist->count();
		stop_right=-1;
		for (int i=0; i<rightlist->count(); i++) {
			if (rightlist->isItemSelected(rightlist->item(i))) {
				start_right= (start_right>i) ? i : start_right;
				stop_right= (stop_right<i) ? i : stop_right;
			}
		}
// 		qDebug("R %d-%d",start_right,stop_right);
	}
}

void TransplantExifDialog::moveup_left() {
	updateinterval(true);
	if (leftlist->count()==0 || start_left==-1 || stop_left==-1 || start_left==0)
		return;
	//"VIEW"
	//copy the before-first element to the past-end of the selection
	leftlist->insertItem(stop_left+1,QFileInfo(from.at(start_left-1)).fileName());
	//remove the before-first element
	leftlist->takeItem(start_left-1);
	//"MODEL"
	from.insert(stop_left+1,from.at(start_left-1));
	from.removeAt(start_left-1);
	start_left--;
	stop_left--;
}

void TransplantExifDialog::moveup_right() {
	updateinterval(false);
	if (rightlist->count()==0 || start_right==-1 || stop_right==-1 || start_right==0)
		return;
	//"VIEW"
	//copy the before-first element to the past-end of the selection
	rightlist->insertItem(stop_right+1,QFileInfo(to.at(start_right-1)).fileName());
	//remove the before-first element
	rightlist->takeItem(start_right-1);
	//"MODEL"
	to.insert(stop_right+1,to.at(start_right-1));
	to.removeAt(start_right-1);
	start_right--;
	stop_right--;
}

void TransplantExifDialog::movedown_left() {
	updateinterval(true);
	if (leftlist->count()==0 || start_left==-1 || stop_left==-1 || stop_left==leftlist->count()-1)
		return;
	//"VIEW"
	//copy the past-end to the before-first element of the selection
	leftlist->insertItem(start_left,QFileInfo(from.at(stop_left+1)).fileName());
	//remove the past-end
	leftlist->takeItem(stop_left+2);
	//"MODEL"
	from.insert(start_left,from.at(stop_left+1));
	from.removeAt(stop_left+2);
	start_left++;
	stop_left++;
}

void TransplantExifDialog::movedown_right() {
	updateinterval(false);
	if (rightlist->count()==0 || start_right==-1 || stop_right==-1 || stop_right==rightlist->count()-1)
		return;
	//"VIEW"
	//copy the past-end to the before-first element of the selection
	rightlist->insertItem(start_right,QFileInfo(to.at(stop_right+1)).fileName());
	//remove the past-end
	rightlist->takeItem(stop_right+2);
	//"MODEL"
	to.insert(start_right,to.at(stop_right+1));
	to.removeAt(stop_right+2);
	start_right++;
	stop_right++;
}

void TransplantExifDialog::remove_left() {
	updateinterval(true);
	if (leftlist->count()==0 || start_left==-1 || stop_left==-1)
		return;
	for (int i=stop_left-start_left+1; i>0; i--) {
		leftlist->takeItem(start_left);
		from.removeAt(start_left);
	}
	start_left=stop_left=-1;
	TransplantButton->setEnabled( leftlist->count()==rightlist->count() && rightlist->count()!=0 );
}

void TransplantExifDialog::remove_right() {
	updateinterval(false);
	if (rightlist->count()==0 || start_right==-1 || stop_right==-1)
		return;
	for (int i=stop_right-start_right+1; i>0; i--) {
		rightlist->takeItem(start_right);
		to.removeAt(start_right);
	}
	start_right=stop_right=-1;
	TransplantButton->setEnabled( leftlist->count()==rightlist->count() && rightlist->count()!=0 );
}

void TransplantExifDialog::append_left() {
	QString filetypes = tr("All Supported formats (*.jpeg *.jpg *.tif *.tiff *.crw *.cr2 *.nef *.dng *.mrw *.orf *.kdc *.dcr *.arw *.ptx *.pef *.x3f *.raw *.sr2)");
	QStringList files = QFileDialog::getOpenFileNames(this, tr("Select the input images"), RecentDirEXIFfrom, filetypes );
	if (!files.isEmpty()) {
		QFileInfo qfi(files.at(0));
		// if the new dir, the one just chosen by the user, is different from the one stored in the settings, update the settings.
		if (RecentDirEXIFfrom != qfi.path()) {
			// update internal field variable
			RecentDirEXIFfrom=qfi.path();
			settings.setValue(KEY_RECENT_PATH_EXIF_FROM, RecentDirEXIFfrom);
		}
		QStringList::Iterator it = files.begin();
		while( it != files.end() ) {
			QFileInfo *qfi=new QFileInfo(*it);
			leftlist->addItem(qfi->fileName()); //fill graphical list
			++it;
			delete qfi;
		}
		from+=files; // add the new files to the "model"
		TransplantButton->setEnabled( leftlist->count()==rightlist->count() && rightlist->count()!=0 );
	}
}

void TransplantExifDialog::append_right() {
	QString filetypes = tr("All Supported formats")
	+ " (*.jpeg *.jpg "
#if EXIV2_TEST_VERSION(0,18,0)
 + "*.tif *.tiff "
#endif
	+ "*.crw *.orf *.kdc *.dcr *.ptx *.x3f)";
	QStringList files = QFileDialog::getOpenFileNames(this, tr("Select the input images"), RecentDirEXIFto, filetypes );
	if (!files.isEmpty()) {
		QFileInfo qfi(files.at(0));
		// if the new dir, the one just chosen by the user, is different from the one stored in the settings, update the settings.
		if (RecentDirEXIFto != qfi.path()) {
			// update internal field variable
			RecentDirEXIFto=qfi.path();
			settings.setValue(KEY_RECENT_PATH_EXIF_TO, RecentDirEXIFto);
		}
		QStringList::Iterator it = files.begin();
		while( it != files.end() ) {
			QFileInfo *qfi=new QFileInfo(*it);
			rightlist->addItem(qfi->fileName()); //fill graphical list
			++it;
			delete qfi;
		}
		to+=files; // add the new files to the "model"
		TransplantButton->setEnabled( leftlist->count()==rightlist->count() && rightlist->count()!=0 );
	}
}

void TransplantExifDialog::transplant_requested() {
	if (done) {
		accept();
		return;
	}

	progressBar->setMaximum(leftlist->count());
	//initialize string iterators to the beginning of the lists.
	QStringList::const_iterator i_source = from.constBegin();
	QStringList::const_iterator i_dest = to.constBegin();

	int index=0;
	//for all the input files
	for (; i_source != from.constEnd(); ++i_source, ++i_dest) {
		try {
			add_log_message(*i_source + "-->" + *i_dest);
			//ExifOperations methods want a std::string, we need to use the QFile::encodeName(QString).constData() trick to cope with local 8-bit encoding determined by the user's locale.
			ExifOperations::copyExifData(QFile::encodeName((*i_source)).constData(), QFile::encodeName((*i_dest)).constData(), checkBox_dont_overwrite->isChecked());
			rightlist->item(index)->setBackground(QBrush("#a0ff87"));
		} catch (Exiv2::AnyError& e) {
			add_log_message("ERROR:" + QString::fromStdString(e.what()) );
			rightlist->item(index)->setBackground(QBrush("#ff743d"));
		}
		progressBar->setValue(progressBar->value()+1); // increment progressbar
		index++;
	}

	done=true;
	TransplantButton->setText(tr("&Done."));
	moveup_left_button->setDisabled(true);
	moveup_right_button->setDisabled(true);
	movedown_left_button->setDisabled(true);
	movedown_right_button->setDisabled(true);
	removeleft->setDisabled(true);
	removeright->setDisabled(true);
	addleft->setDisabled(true);
	addright->setDisabled(true);
	cancelbutton->setDisabled(true);
}

void TransplantExifDialog::add_log_message(const QString& message) {
	full_Log_Model->insertRows(full_Log_Model->rowCount(),1);
	full_Log_Model->setData(full_Log_Model->index(full_Log_Model->rowCount()-1), message, Qt::DisplayRole);
	Log_Widget->scrollToBottom();
}

void TransplantExifDialog::filterChanged(const QString& newtext) {
	bool no_text=newtext.isEmpty();
	filterComboBox->setEnabled(no_text);
	filterLabel1->setEnabled(no_text);
	clearTextToolButton->setEnabled(!no_text);
	if (no_text)
		filterComboBoxActivated(filterComboBox->currentIndex());
	else
		log_filter->setFilterRegExp(QRegExp(newtext, Qt::CaseInsensitive, QRegExp::RegExp));
}

void TransplantExifDialog::filterComboBoxActivated(int index) {
	QString regexp;
	switch (index) {
	case 0:
		regexp=".*";
		break;
	case 1:
		regexp="error";
		break;
	}
	log_filter->setFilterRegExp(QRegExp(regexp, Qt::CaseInsensitive, QRegExp::RegExp));
}












