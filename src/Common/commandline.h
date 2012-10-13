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

#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <QString>
#include <QProcess>
#include <QDir>
#include "../HdrCreation/HdrCreationManager.h"

class CommandLineInterfaceManager : public QObject {
Q_OBJECT
public:
	CommandLineInterfaceManager(const int argc, char **argv);
private:
	enum operation_mode {
		CREATE_HDR_MODE,
		LOAD_HDR_MODE,
		UNKNOWN_MODE
	} operation_mode;
	
	enum align_mode {
		AIS_ALIGN,
		MTB_ALIGN,
		NO_ALIGN,
	} align_mode;

	const int argc;
	char **argv;
	
	float toFloatWithErrMsg(const QString &str);
	int toIntWithErrMsg(const QString &str);
	void startTonemap();

	QList<float> ev;
	HdrCreationManager *hdrCreationManager;
	QString saveHdrFilename;
	QString saveLdrFilename;
	QtpfsguiOptions *qtpfsgui_options;
	pfs::Frame *HDR;
	void saveHDR();
	void printHelp(char *progname);
	tonemapping_options *tmopts;
	bool verbose;
private slots:
	void finishedLoadingInputFiles(QStringList);
	void errorWhileLoading(QString errormessage);
	void ais_failed(QProcess::ProcessError);
	void createHDR();
	void loadFinished(pfs::Frame*,QString);
	void parseArgs();
	void tonemapTerminated(const QImage&,tonemapping_options*);

signals:
	void startParsing();
	void finishedParsing();
};

#endif
