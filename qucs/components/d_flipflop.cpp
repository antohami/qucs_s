/***************************************************************************
                              d_flipflop.cpp
                             ----------------
    begin                : Fri Jan 06 2006
    copyright            : (C) 2006 by Michael Margraf
    email                : michael.margraf@alumni.tu-berlin.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "d_flipflop.h"
#include "node.h"
#include "misc.h"
#include "extsimkernels/spicecompat.h"

D_FlipFlop::D_FlipFlop()
{
  Type = isComponent;
  Description = QObject::tr("D flip flop with asynchronous reset");

  Props.append(new Property("t", "0", false, QObject::tr("delay time")));

  Rects.append(new qucs::Rect(-20, -20, 40, 40, QPen(Qt::darkBlue,2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin)));

  Lines.append(new qucs::Line(-30,-10,-20,-10,QPen(Qt::darkBlue,2)));
  Lines.append(new qucs::Line(-30, 10,-20, 10,QPen(Qt::darkBlue,2)));
  Lines.append(new qucs::Line( 30,-10, 20,-10,QPen(Qt::darkBlue,2)));
  Lines.append(new qucs::Line(  0, 20,  0, 30,QPen(Qt::darkBlue,2)));

  Texts.append(new Text(-18,-18, "D", Qt::darkBlue, 12.0));
  Texts.append(new Text(  8,-18, "Q", Qt::darkBlue, 12.0));
  Texts.append(new Text( -3.5,  8, "R", Qt::darkBlue, 9.0));
  Polylines.append(new qucs::Polyline(
    std::vector<QPointF>{{-20, 6}, {-12, 10}, {-20, 14}}, QPen(Qt::darkBlue,2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin)
  ));

  Ports.append(new Port(-30,-10));  // D
  Ports.append(new Port(-30, 10));  // Clock
  Ports.append(new Port( 30,-10));  // Q
  Ports.append(new Port(  0, 30));  // Reset

  x1 = -30; y1 = -24;
  x2 =  30; y2 =  30;
  tx = x1+4;
  ty = y2+4;
  Model = "DFF";
  Name  = "Y";
  SpiceModel = "A";
}

// -------------------------------------------------------
QString D_FlipFlop::vhdlCode(int NumPorts)
{
  QString s = "";
  if(NumPorts <= 0) { // no truth table simulation ?
    QString td = Props.at(0)->Value;     // delay time
    if(!misc::VHDL_Delay(td, Name)) return td; // time has not VHDL format
    s += td;
  }
  s += ";\n";

  s = "  " + Name + " : process (" +
      Ports.at(0)->Connection->Name + ", " +
      Ports.at(1)->Connection->Name + ")\n  begin\n    if (" +
      Ports.at(3)->Connection->Name + "='1') then  " +
      Ports.at(2)->Connection->Name + " <= '0'" + s +"    elsif (" +
      Ports.at(1)->Connection->Name + "='1' and " +
      Ports.at(1)->Connection->Name + "'event) then\n      " +
      Ports.at(2)->Connection->Name + " <= " +
      Ports.at(0)->Connection->Name + s + "    end if;\n  end process;\n\n";
  return s;
}

// -------------------------------------------------------
QString D_FlipFlop::verilogCode(int NumPorts)
{
  QString t = "";
  if(NumPorts <= 0) { // no truth table simulation ?
    QString td = Props.at(0)->Value;        // delay time
    if(!misc::Verilog_Delay(td, Name)) return td; // time has not VHDL format
    if(!td.isEmpty()) t = "   " + td  + ";\n";
  }
  
  QString s = "";
  QString q = Ports.at(2)->Connection->Name;
  QString d = Ports.at(0)->Connection->Name;
  QString r = Ports.at(3)->Connection->Name;
  QString c = Ports.at(1)->Connection->Name;
  QString v = "net_reg" + Name + q;
  
  s = "\n  // " + Name + " D-flipflop\n" +
    "  assign  " + q + " = " + v + ";\n" +
    "  reg     " + v + " = 0;\n" +
    "  always @ (" + c + " or " + r + ") begin\n" + t +
    "    if (" + r + ") " + v + " <= 0;\n" +
    "    else if (~" + r + " && " + c + ") " + v + " <= " + d + ";\n" +
    "  end\n\n";
  return s;
}

// -------------------------------------------------------
Component* D_FlipFlop::newOne()
{
  return new D_FlipFlop();
}

// -------------------------------------------------------
Element* D_FlipFlop::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("D-FlipFlop");
  BitmapFile = (char *) "dflipflop";

  if(getNewOne)  return new D_FlipFlop();
  return 0;
}

QString D_FlipFlop::spice_netlist(bool isXyce)
{
    if (isXyce) return QString();

    QString s = SpiceModel + Name;
    QString tmp_model = "model_" + Name;
    QString td = spicecompat::normalize_value(getProperty("t")->Value);

    QString SET   = "0";
    QString QB    = "QB_" + Name;
    QString D     = spicecompat::normalize_node_name(Ports.at(0)->Connection->Name);
    QString CLK   = spicecompat::normalize_node_name(Ports.at(1)->Connection->Name);
    QString Q     = spicecompat::normalize_node_name(Ports.at(2)->Connection->Name);
    QString RESET = spicecompat::normalize_node_name(Ports.at(3)->Connection->Name);

    s += " " + D + " " + CLK + " " + SET + " " + RESET + " " + Q + " " + QB;

    s += " " + tmp_model + "\n";
    s += QString(".model %1 d_dff(clk_delay=%2 set_delay=%2 reset_delay=%2 rise_delay=%2 fall_delay=%2)\n")
            .arg(tmp_model).arg(td);
    s += QString("C%1 QB_%1 0 1e-9 \n").arg(Name); // capacitor load for unused QB pin

    return s;
}
