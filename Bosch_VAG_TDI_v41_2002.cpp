//---------------------------------------------------------------------------
//
// Bosch VAG TDI v4.1 - 2002 Checksum Algorithm
// Version 1.0
//
// Copyright 2007-2012 MTX Electronics
// Web: www.mtx-electronics.com
// Email: info@mtx-electronics.com
//
//---------------------------------------------------------------------------
//
// This file is part of VAG Checksum Fix.
//
//    VAG Checksum Fix is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    VAG Checksum Fix is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with VAG Checksum Fix.  If not, see <http://www.gnu.org/licenses/>.
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Search buffer for checksums and update
//---------------------------------------------------------------------------

void __fastcall TfrmMain::tdi41_2002_checksum_search(void)
{
	unsigned int seed_1, seed_2;
	unsigned short seed_1_msb, seed_1_lsb, seed_2_lsb, seed_2_msb;

  unsigned int chk_oldvalue, chk_value, chk_start_addr, chk_end_addr, chk_store_addr;

  chk_found=2;
  chk_fixed=0;

  // Find seed 1
  seed_1=tdi41_2002_checksum_calculate(0x14000 , 0x4bffe , 0x8631 , 0xefcd, 0 , 0 , true);
	seed_1_msb = (unsigned short)(seed_1 >> 16);
  seed_1_lsb = (unsigned short)seed_1;

  // Find seed 2
  seed_2 = tdi41_2002_checksum_calculate(0, 0x7ffe, 0, 0, 0, 0, true);
  seed_2_msb = (unsigned short)(seed_2 >> 16);
  seed_2_lsb = (unsigned short)seed_2;
  
  // checksum 1
  chk_oldvalue = ((unsigned int)file_buffer[0xffff] << 24)
               + ((unsigned int)file_buffer[0xfffe] << 16)
               + ((unsigned int)file_buffer[0xfffd] << 8)
               + (unsigned int)file_buffer[0xfffc];

	chk_value = tdi41_2002_checksum_calculate(0x8000, 0xfffb, seed_2_lsb, seed_2_msb, 0x4531, 0x3550, false);

  if (chk_oldvalue != chk_value)
  {
    file_buffer[0xfffc] = chk_value & 0x000000ff;
    file_buffer[0xfffd] = (chk_value >> 8) & 0x000000ff;
    file_buffer[0xfffe] = (chk_value >> 16) & 0x000000ff;
    file_buffer[0xffff] = (chk_value >> 24) & 0x000000ff;

    chk_fixed++;
  }

  // Checksum 2
  chk_oldvalue = ((unsigned int)file_buffer[0x13fff] << 24)
               + ((unsigned int)file_buffer[0x13ffe] << 16)
               + ((unsigned int)file_buffer[0x13ffd] << 8)
               + (unsigned int)file_buffer[0x13ffc];

  chk_value = tdi41_2002_checksum_calculate(0x10000, 0x13ffb, 0, 0, 0x8631, 0xefcd, false);

  if (chk_oldvalue != chk_value)
  {
    file_buffer[0x13ffc] = chk_value & 0x000000ff;
    file_buffer[0x13ffd] = (chk_value >> 8) & 0x000000ff;
    file_buffer[0x13ffe] = (chk_value >> 16) & 0x000000ff;
    file_buffer[0x13fff] = (chk_value >> 24) & 0x000000ff;

    chk_fixed++;
  }

  // Checksum blocks loop
  chk_store_addr = 0x4fffb;
  do
  {
    if (( file_buffer[chk_store_addr + 13] == 0x56 ) &&
        ( file_buffer[chk_store_addr + 14] == 0x34 ) &&
        ( file_buffer[chk_store_addr + 15] == 0x2e ) &&
        ( file_buffer[chk_store_addr + 16] == 0x31 ))
    {
      // Checksum
      chk_start_addr = chk_store_addr - 0x3ffb;
      chk_end_addr = chk_store_addr;

      chk_oldvalue = ((unsigned int)file_buffer[chk_store_addr + 4] << 24)
                   + ((unsigned int)file_buffer[chk_store_addr + 3] << 16)
                   + ((unsigned int)file_buffer[chk_store_addr + 2] << 8)
                   + (unsigned int)file_buffer[chk_store_addr + 1];

      chk_value = tdi41_2002_checksum_calculate(chk_start_addr, chk_end_addr, seed_1_lsb, seed_1_msb, seed_1_lsb, seed_1_msb, false);

      if (chk_oldvalue != chk_value)
      {
        file_buffer[chk_store_addr + 1] = chk_value & 0x000000ff;
        file_buffer[chk_store_addr + 2] = (chk_value >> 8) & 0x000000ff;
        file_buffer[chk_store_addr + 3] = (chk_value >> 16) & 0x000000ff;
        file_buffer[chk_store_addr + 4] = (chk_value >> 24) & 0x000000ff;

        chk_fixed++;
      }

      // Checksum
      chk_start_addr = chk_store_addr + 5;
      chk_end_addr = chk_store_addr + 0xb80;

      chk_oldvalue = ((unsigned int)file_buffer[chk_store_addr + 2948] << 24)
                   + ((unsigned int)file_buffer[chk_store_addr + 2947] << 16)
                   + ((unsigned int)file_buffer[chk_store_addr + 2946] << 8)
                   + (unsigned int)file_buffer[chk_store_addr + 2945];

      chk_value = tdi41_2002_checksum_calculate(chk_start_addr, chk_end_addr, seed_1_lsb, seed_1_msb, seed_1_lsb, seed_1_msb, false);
      
      if (chk_oldvalue != chk_value)
      {
        file_buffer[chk_store_addr + 2945] = chk_value & 0x000000ff;
        file_buffer[chk_store_addr + 2946] = (chk_value >> 8) & 0x000000ff;
        file_buffer[chk_store_addr + 2947] = (chk_value >> 16) & 0x000000ff;
        file_buffer[chk_store_addr + 2948] = (chk_value >> 24) & 0x000000ff;

        chk_fixed++;
      }

      // Checksum
      chk_start_addr = chk_store_addr + 0xb85;
      chk_end_addr = chk_store_addr + 0xc000;

      chk_oldvalue = ((unsigned int)file_buffer[chk_store_addr + 49156] << 24)
                   + ((unsigned int)file_buffer[chk_store_addr + 49155] << 16)
                   + ((unsigned int)file_buffer[chk_store_addr + 49154] << 8)
                   + (unsigned int)file_buffer[chk_store_addr + 49153];

      chk_value = tdi41_2002_checksum_calculate(chk_start_addr, chk_end_addr, seed_1_lsb, seed_1_msb, seed_1_lsb, seed_1_msb, false);

      if (chk_oldvalue != chk_value)
      {
        file_buffer[chk_store_addr + 49153] = chk_value & 0x000000ff;
        file_buffer[chk_store_addr + 49154] = (chk_value >> 8) & 0x000000ff;
        file_buffer[chk_store_addr + 49155] = (chk_value >> 16) & 0x000000ff;
        file_buffer[chk_store_addr + 49156] = (chk_value >> 24) & 0x000000ff;

        chk_fixed++;
      }
      
      chk_found+=3;
    }
    
    chk_store_addr += 0x10000;
  } while ( chk_store_addr + 5 < file_size );
}

//---------------------------------------------------------------------------
// Calculate new checksum for a block of data
//---------------------------------------------------------------------------

unsigned int __fastcall TfrmMain::tdi41_2002_checksum_calculate(unsigned int chk_start_addr, unsigned int chk_end_addr, unsigned short seed_a, unsigned short seed_b, unsigned short seed_c, unsigned short seed_d, bool first_pass)
{
  unsigned int count=chk_start_addr / 2;
  unsigned int end_count=chk_end_addr / 2;
  unsigned int buffer_addr=chk_start_addr;
  unsigned int checksum, var_6, var_7=0;

  unsigned short var_1=0, var_2=0, var_3, var_4, var_5;
   
  if ( count != end_count )
  {
    var_1 = seed_a;
    var_2 = seed_b;
    
    if ( chk_start_addr == 0x8000 )
    {
      var_1 = var_1 ^ 0xD565;
      var_2 += 0x308a;
    }

    do
    {
      var_1 ^= ((unsigned short)file_buffer[buffer_addr + 1] << 8) + (unsigned short)file_buffer[buffer_addr];
      var_3 = var_2 & 0xf;
      ++count;
      buffer_addr += 2;
      var_4 = 0;
      
      if ( var_2 & 0xf )
      {
        do
        {
          var_4 = var_1 >> 15;
          var_1 = ( var_1 * 2) + var_4;
          
          --var_3;
        } while ( var_3 );
      }

      var_2 -= var_4 + ((unsigned short)file_buffer[buffer_addr + 1] << 8) + (unsigned short)file_buffer[buffer_addr];
      var_2 = var_1 ^ var_2;

      buffer_addr += 2;
      ++count;
      
      if ( count > end_count )
        break;
        
      var_5 = ((unsigned short)file_buffer[buffer_addr + 1] << 8) + (unsigned short)file_buffer[buffer_addr];
      buffer_addr += 4;
      var_1 += 0xffff - var_5 + 0xdaad;
      var_6 = (unsigned short)file_buffer[buffer_addr - 1] << 8;
      var_2 ^= (unsigned short)var_6 + (unsigned short)file_buffer[buffer_addr - 2];
      var_4 = var_1 & 0xf;
      count +=2;
      
      if ( var_1 & 0xf )
      {
        do
        {
          var_6 = (var_6 | 0xffff) & var_2;
          var_6 <<= 15;
          var_2 = (var_2 >> 1) + var_6;

          --var_4;
        } while ( var_4 );
      }
    } while ( count <= end_count );
  }
  
  if ( !chk_start_addr )
  {
    var_1 -= 0x79cf;
    var_2 -= 0x1033;
  }
  
  if ( !first_pass )
  {
    var_5 = seed_d;
    var_1 -= seed_c;
    var_6 = (seed_c | 0xffff) & 0xdaad;
    var_1 += var_6 - 1;
    var_7 = var_7 & 0xffff;
    
    for ( count = seed_c & 0xf; count; var_5 = ((unsigned int)var_5 >> 15) + var_7 )
    {
      --count;
      var_7 = (var_7 | 0xffff) & var_5;
      var_7 *= 2;
    }
    
    checksum = var_1 + ((var_5 ^ var_2) << 16);
  }
  else
  {
    checksum = var_1 + (var_2 << 16);
  }

  return (checksum);
}
