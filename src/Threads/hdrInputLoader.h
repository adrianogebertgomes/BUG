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

#ifndef LOADHDRINPUT_H
#define LOADHDRINPUT_H
#include <QThread>
#include <QImage>
#include "../Common/options.h"
#include "../Libpfs/pfs.h"

class hdrInputLoader : public QThread {
Q_OBJECT

public:
	hdrInputLoader(QString filename, int image_idx, QStringList dcrawOpts);
	~hdrInputLoader();
signals:
	void ldrReady(    QImage *ldrImage, int index, float expotime, QString new_fname, bool ldrtiff);
	void mdrReady(pfs::Frame *mdrImage, int index, float expotime, QString new_fname);
	void loadFailed(QString errormessage, int index);
protected:
	void run();
private:
	int image_idx;
	QString fname;
	QStringList dcrawOpts;
};
#endif
