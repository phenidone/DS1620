/* -*- C++ -*- */
/*
    Copyright (C) 2012-2016 William Brodie-Tyrrell
    william@brodie-tyrrell.org
  
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of   
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
*/


#include "DS1620.h"
#include <Arduino.h>

DS1620::DS1620(char dq, char clk, char rst)
    : DQ(dq), CLK(clk), RST(rst)
{
    pinMode(DQ, INPUT);
    digitalWrite(CLK, 1);
    digitalWrite(RST, 0);
    pinMode(RST, OUTPUT);
    pinMode(CLK, OUTPUT);
}

void DS1620::raw_write(short c, char bits)
{
  pinMode(DQ, OUTPUT);  
  digitalWrite(RST, 1);
  
  for(int i=0;i<bits;++i){
    digitalWrite(CLK, 0);
    digitalWrite(DQ, c & 1);
    digitalWrite(CLK, 1);
    c >>= 1;
  }  
}

void DS1620::write(char addr, short value, int bits)
{
  digitalWrite(CLK, 1);
  digitalWrite(RST, 0);
  raw_write(addr);
  raw_write(value, bits);
  digitalWrite(RST, 0);
}

short DS1620::read(char addr, char bits)
{
  digitalWrite(CLK, 1);
  digitalWrite(RST, 0);
  raw_write(addr);

  pinMode(DQ, INPUT);

  short result=0;

  for(char i=0;i<bits;++i){
    digitalWrite(CLK, 0);
    // result >>= 1;
    // result |= digitalRead(DQ) << (bits-1);
    result |= short(digitalRead(DQ)) << i;
    digitalWrite(CLK, 1);
  }
  
  digitalWrite(RST, 0);
  
  return result;
}

void DS1620::setTL(short tl)
{
    wait_write();
    write(WRITE_TL, tl);        
}

short DS1620::getTL()
{
    return read(READ_TL);
}

void DS1620::setTH(short th)
{
    wait_write();
    write(WRITE_TH, th);
}

short DS1620::getTH()
{
    return read(READ_TH);
}

void DS1620::setConfig(unsigned char c)
{
    wait_write();
    write(WRITE_CONF, c, 8);
}

unsigned char DS1620::getConfig()
{
    return (unsigned char) (read(READ_CONF, 8) & 0xFF);
}
    
void DS1620::convert()
{
    write(CONVERT, 0, 0);
}

short DS1620::getTemp()
{
    return read(READ_TEMP);
}

bool DS1620::done()
{
    return getConfig() & DONE;
}

bool DS1620::nv_busy()
{
    return getConfig() & NVB;
}

void DS1620::wait_write()
{    
    for(int del=0;del<WRITE_TIMEOUT && nv_busy();++del){
        delay(1);
    }
}
