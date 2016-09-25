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


#ifndef _DS1620_WIBT_
#define _DS1620_WIBT_

class DS1620 {
public:

    DS1620(char dq, char clk, char rst);

    /// set temp-low threshold
    void setTL(short tl);
    /// get temp-low threshold
    short getTL();
    /// set temp-high threshold
    void setTH(short th);
    /// get temp-high threshold
    short getTH();
    /// set config byte
    void setConfig(unsigned char c);
    /// get config byte
    unsigned char getConfig();
    
    /// initiate a conversion; required in one-shot mode
    void convert();
    /// get last conversion
    short getTemp();
    /// is conversion complete?
    bool done();

    // bits in the config byte
    static const char DONE=0x80;
    static const char THF=0x40;
    static const char TLF=0x20;
    static const char NVB=0x10;
    static const char CBIT=0x08;
    static const char CPU=0x02;
    static const char SHOT1=0x01;

private:

    /// write to a register
    void write(char addr, short value, int bits=9);
    /// read from a register
    short read(char addr, char bits=9);
    /// raw writing of bits
    void raw_write(short c, char bits=8);
    /// wait until it's 15ms since last call - last flash writes complete
    void wait_write();
    /// is the non-volatile ram busy?
    bool nv_busy();

    const char DQ, CLK, RST;

    static const int CONVERT_TIMEOUT=15;
    static const short INVALID_TEMP=-1000;
    static const int WRITE_TIMEOUT=20;

    static const char READ_TEMP=0xAA;
    static const char WRITE_TH=0x01;
    static const char WRITE_TL=0x02;
    static const char READ_TH=0xA1;
    static const char READ_TL=0xA2;
    static const char CONVERT=0xEE;
    static const char WRITE_CONF=0x0C;
    static const char READ_CONF=0xAC;

};

#endif
