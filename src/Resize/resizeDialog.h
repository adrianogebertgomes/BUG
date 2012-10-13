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

#ifndef RESIZEDIALOG_IMPL_H
#define RESIZEDIALOG_IMPL_H

#include <QDialog>
#include "../generated_uic/ui_resizedialog.h"
#include "../Libpfs/pfs.h"

class ResizeDialog : public QDialog, private Ui::ResizeDialog
{
Q_OBJECT
public:
	ResizeDialog(QWidget *parent,pfs::Frame *orig);
	~ResizeDialog();
	pfs::Frame* getResizedFrame();
public slots:
	void scaledPressed();
	void switch_px_percentage(int);
	void update_heightSpinBox();
	void update_widthSpinBox();
	void defaultpressed();
private:
	pfs::Frame *original;
	pfs::Frame *resized;
	int orig_width,orig_height;
	int resized_width,resized_height;
	void updatelabel();
	int rh_from_rw();
	int rw_from_rh();
	bool from_other_spinbox;
};

#endif
