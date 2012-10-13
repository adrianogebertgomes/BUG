/**
 * @file tmo_pattanaik00.h
 * @brief Time-dependent Visual Adaptation Model, [Pattanaik2000]
 *
 * Time-Dependent Visual Adaptation for Realistic Image Display
 * S.N. Pattanaik, J. Tumblin, H. Yee, and D.P. Greenberg
 * In Proceedings of ACM SIGGRAPH 2000
 *
 * 
 * This file is a part of Qtpfsgui package.
 * ---------------------------------------------------------------------- 
 * Copyright (C) 2003,2004 Grzegorz Krawczyk
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
 * @author Grzegorz Krawczyk, <krawczyk@mpi-sb.mpg.de>
 * @author Giuseppe Rota <grota@users.sourceforge.net>
 *
 * $Id: tmo_pattanaik00.h,v 1.2 2004/09/22 10:00:28 krawczyk Exp $
 */


#ifndef _tmo_pattanaik00_h_
#define _tmo_pattanaik00_h_

#include "../../Libpfs/pfs.h"

class VisualAdaptationModel;

/**
 * @brief: Tone mapping algorithm [Pattanaik2000]
 *
 * @param R red channel
 * @param G green channel
 * @param B blue channel
 * @param Y luminance channel
 * @param am pointer to adaptation model
 * @param local false: use global version, true: use local version
 */
void tmo_pattanaik00( pfs::Array2D* R, pfs::Array2D* G, pfs::Array2D* B, 
  pfs::Array2D* Y, VisualAdaptationModel* am, bool local=false );

pfs::Frame* pfstmo_pattanaik00 (pfs::Frame* inpfsframe, bool _local,float _multiplier, float _Acone, float _Arod, bool autoY );

/**
 * @brief Time-dependent Visual Adaptation Model
 *
 * Class for storing state of visual adaptation model.
 */
class VisualAdaptationModel
{
private:
  /// cone's adaptation level
  float Acone;

  /// rod's adaptation level
  float Arod;

  /// cone's bleaching term
  float Bcone;

  /// rod's bleaching term
  float Brod;

  /// calculate logarithmic average of Y
  float calculateLogAvgLuminance( pfs::Array2D* Y );

public:

  /**
   * @brief Constructor
   * Sets default adaptation state (office light conditions)
   */
  VisualAdaptationModel();

  /**
   * @brief Calculate adaptation level according to Visual Adaptation Model
   *
   * @param Gcone goal adaptation value for cones
   * @param Grod goal adaptation value for rods
   * @param dt time [s] that passed after last calculation of adaptation
   */
  void calculateAdaptation(float Gcone, float Grod, float dt);

  /**
   * @brief Calculate adaptation level according to Visual Adaptation Model
   *
   * @param Y luminance map of HDR image
   * @param dt time [s] that passed after last calculation of adaptation
   */
  void calculateAdaptation(pfs::Array2D *Y, float dt);

  /**
   * @brief Set adaptation level to given values
   *
   * @param Gcone goal adaptation value for cones
   * @param Grod goal adaptation value for rods
   */
  void setAdaptation(float Gcone, float Grod);

  /**
   * @brief Set adaptation level appropriate for given lumiance map
   *
   * @param Y luminance map of HDR image
   */
  void setAdaptation(pfs::Array2D *Y);

  /// Get cone adaptation level
  float getAcone()
  { return Acone; };

  /// Get rod adaptation level
  float getArod()
  { return Arod; };

  /// Get cone bleaching term
  float getBcone()
  { return Bcone; };

  /// Get rod bleaching term
  float getBrod()
  { return Brod; };

};


#endif /* _tmo_pattanaik00_h_ */
