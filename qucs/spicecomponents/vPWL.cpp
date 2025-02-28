/***************************************************************************
                         vPWL.cpp  -  description
                   --------------------------------------
    begin                    : Fri Mar 9 2007
    copyright              : (C) 2007 by Gunther Kraut
    email                     : gn.kraut@t-online.de
    spice4qucs code added  Thurs. 26 March 2015
    copyright              : (C) 2015 by Mike Brinson
    email                    : mbrin72043@yahoo.co.uk

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "vPWL.h"
#include "node.h"
#include "extsimkernels/spicecompat.h"


vPWL::vPWL()
{
  Description = QObject::tr("SPICE V(PWL):\nMultiple line ngspice or Xyce V specifications allowed using \"+\" continuation lines.\nLeave continuation lines blank when NOT in use. ");
  Simulator = spicecompat::simSpice;

  Ellipses.append(new qucs::Ellips(-12,-12, 24, 24, QPen(Qt::darkRed,3)));
  Texts.append(new Text(26, 6,"PWL",Qt::darkRed,12.0,0.0,-1.0));
  // pins
  Lines.append(new qucs::Line(-30,  0,-12,  0,QPen(Qt::darkBlue,2)));
  Lines.append(new qucs::Line( 30,  0, 12,  0,QPen(Qt::darkBlue,2)));
  // plus
  Lines.append(new qucs::Line( 18,  -5, 18, -11,QPen(Qt::red,2)));
  Lines.append(new qucs::Line( 21,  -8, 15,  -8,QPen(Qt::red,2)));
  // minus
  Lines.append(new qucs::Line(-18,  -5,-18, -11,QPen(Qt::black,2)));

  Ports.append(new Port( 30,  0));
  Ports.append(new Port(-30,  0));

  x1 = -30; y1 = -14;
  x2 =  30; y2 =  40;

  tx = x1+4;
  ty = y2+4;
  Model = "vPWL";
  SpiceModel = "V";
  Name  = "V";

  Props.append(new Property("PWL", "", true,"Piece-Wise Linear\nSpecification"));
 Props.append(new Property("Line_2", "", false,"+ continuation line 1"));
 Props.append(new Property("Line_3", "", false,"+ continuation line 2"));
 Props.append(new Property("Line_4", "", false,"+ continuation line 3"));
 Props.append(new Property("Line_5", "", false,"+ continuation line 4"));
 Props.append(new Property("Line_6", "", false,"+ continuation line 5"));
 Props.append(new Property("Line_7", "", false,"+ continuation line 6"));
 Props.append(new Property("Line_8", "", false,"+ continuation line 7"));
 Props.append(new Property("Line_9", "", false,"+ continuation line 8"));
 Props.append(new Property("Line_10", "", false,"+ continuation line 9"));
 
 
  rotate();  // fix historical flaw
}

vPWL::~vPWL()
{
}

Component* vPWL::newOne()
{
  return new vPWL();
}

Element* vPWL::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("V(PWL)");
  BitmapFile = (char *) "vPWL";

  if(getNewOne)  return new vPWL();
  return 0;
}

QString vPWL::netlist()
{
    return QString();
}

QString vPWL::spice_netlist(bool)
{
    QString s = spicecompat::check_refdes(Name,SpiceModel);
    for (Port *p1 : Ports) {
        QString nam = p1->Connection->Name;
        if (nam=="gnd") nam = "0";
        s += " "+ nam+" ";   // node names
    }


QString PWL= Props.at(0)->Value;
QString Line_2= Props.at(1)->Value;
QString Line_3= Props.at(2)->Value;
QString Line_4= Props.at(3)->Value;
QString Line_5= Props.at(4)->Value;
QString Line_6= Props.at(5)->Value;
QString Line_7= Props.at(6)->Value;
QString Line_8= Props.at(7)->Value;
QString Line_9= Props.at(8)->Value;
QString Line_10= Props.at(9)->Value;

    s += QString();
  
    if(  PWL.length()  > 0)        s += QString("%1").arg(PWL);
    if(  Line_2.length() > 0 )     s += QString("\n%1").arg(Line_2);
    if(  Line_3.length() > 0 )     s += QString("\n%1").arg(Line_3);
    if(  Line_4.length() > 0 )     s += QString("\n%1").arg(Line_4);
    if(  Line_5.length() > 0 )     s += QString("\n%1").arg(Line_5);
    if(  Line_6.length() > 0 )     s += QString("\n%1").arg(Line_6);
    if(  Line_7.length() > 0 )     s += QString("\n%1").arg(Line_7);
    if(  Line_8.length() > 0)      s += QString("\n%1").arg(Line_8);
    if(  Line_9.length() > 0 )     s += QString("\n%1").arg(Line_9);
    if(  Line_10.length() > 0 )    s += QString("\n%1").arg(Line_10);
    s += "\n";

    return s;
}
