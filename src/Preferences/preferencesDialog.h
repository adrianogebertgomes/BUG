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

#ifndef OPTIONS_IMPL_H
#define OPTIONS_IMPL_H

#include <QDialog>
#include <QMap>
#include "../generated_uic/ui_options.h"
#include "../Common/options.h"
#include "../Common/global.h"

class PreferenceDialog : public QDialog, private Ui::PreferencesDialog
{
Q_OBJECT
public:
	PreferenceDialog(QWidget *parent);
	~PreferenceDialog();
private:
	void change_color_of(QPushButton *,QColor *);
	void from_options_to_gui();
	QtpfsguiOptions *qtpfsgui_options;
	QColor infnancolor, negcolor;
	QMap<QString, int> fromIso639ToGuiIndex;
	QMap<int, QString> fromGuiIndexToIso639;
	QStringList sanitizeAISparams();
	QStringList sanitizeDCRAWparams();
private slots:
	void negative_clicked();
	void infnan_clicked();
	void ok_clicked();
	void updateLineEditString();
	void helpDcrawParamsButtonClicked();
	void enterWhatsThis();
};
#endif
