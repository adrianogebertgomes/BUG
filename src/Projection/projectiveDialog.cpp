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

#include "projectiveDialog.h"

projectiveTransformDialog::projectiveTransformDialog(QWidget *parent,pfs::Frame *orig) : QDialog(parent),original(orig),transformed(NULL) {
	setupUi(this);

	projectionList.append(&(PolarProjection::singleton));
	projectionList.append(&(AngularProjection::singleton));
	projectionList.append(&(CylindricalProjection::singleton));
	projectionList.append(&(MirrorBallProjection::singleton));
	transforminfo = new TransformInfo();
	transforminfo->srcProjection=projectionList.at(0);
	transforminfo->dstProjection=projectionList.at(0);

	connect(okButton,SIGNAL(clicked()),this,SLOT(okClicked()));
	connect(sourceProjection,SIGNAL(activated(int)),this,SLOT(srcProjActivated(int)));
	connect(destProjection,SIGNAL(activated(int)),this,SLOT(dstProjActivated(int)));
	connect(bilinearCheckBox,SIGNAL(toggled(bool)),this,SLOT(bilinearToggled(bool)));
	connect(oversampleSpinBox,SIGNAL(valueChanged(int)),this,SLOT(oversampleChanged(int)));
	connect(XrotSpinBox,SIGNAL(valueChanged(int)),this,SLOT(XRotChanged(int)));
	connect(YrotSpinBox,SIGNAL(valueChanged(int)),this,SLOT(YRotChanged(int)));
	connect(ZrotSpinBox,SIGNAL(valueChanged(int)),this,SLOT(ZRotChanged(int)));
	connect(anglesSpinBox,SIGNAL(valueChanged(int)),this,SLOT(anglesAngularDestinationProj(int)));
}

projectiveTransformDialog::~projectiveTransformDialog() {
	delete transforminfo;
}

void projectiveTransformDialog::XRotChanged(int v) {
	transforminfo->xRotate=v;
}
void projectiveTransformDialog::YRotChanged(int v) {
	transforminfo->yRotate=v;
}
void projectiveTransformDialog::ZRotChanged(int v) {
	transforminfo->zRotate=v;
}
void projectiveTransformDialog::oversampleChanged(int v) {
	transforminfo->oversampleFactor=v;
}
void projectiveTransformDialog::bilinearToggled(bool v) {
	transforminfo->interpolate=v;
}
void projectiveTransformDialog::dstProjActivated(int gui_index) {
	transforminfo->dstProjection=projectionList.at(gui_index);
	bool transformIsAngular=transforminfo->dstProjection==&(AngularProjection::singleton);
	labelAngles->setEnabled(transformIsAngular);
	anglesSpinBox->setEnabled(transformIsAngular);
	if (transformIsAngular)
		((AngularProjection*)(transforminfo->dstProjection))->setAngle(anglesSpinBox->value());
}
void projectiveTransformDialog::srcProjActivated(int gui_index) {
	transforminfo->srcProjection=projectionList.at(gui_index);
}

void projectiveTransformDialog::anglesAngularDestinationProj(int v) {
	((AngularProjection*)(transforminfo->dstProjection))->setAngle(v);
}

void projectiveTransformDialog::okClicked() {
	qDebug("Projective Transformation from %s to %s", transforminfo->srcProjection->getName(), transforminfo->dstProjection->getName());

	pfs::DOMIO pfsio;
	pfs::Channel *R,*G,*B;
	original->getRGBChannels( R,G,B );
	int xSize=original->getWidth();
	int ySize=(int)(xSize / transforminfo->dstProjection->getSizeRatio());
	transformed = pfsio.createFrame( xSize,ySize );
	pfs::ChannelIterator *it = original->getChannels();
	while( it->hasNext() ) {
		pfs::Channel *originalCh = it->getNext();
		pfs::Channel *newCh = transformed->createChannel( originalCh->getName() );
		transformArray( originalCh, newCh, transforminfo );
	}
	pfs::copyTags( original, transformed );
	emit accept();
}
