//---------------------------------------------------------------------------
//
// Bosch VAG TDI v4.1 Checksum Algorithm
// Version 1.1
//
// Copyright 2008-2012 MTX Electronics
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

void __fastcall TfrmMain::tdi41_checksum_search(void)
{
  bool first_pass = true;
  unsigned int chk_oldvalue, chk_value, chk_start_addr, chk_end_addr;
  unsigned int chk_array[12] = {0x10000, 0x14000, 0x4C000, 0x50000, 0x50B80, 0x5C000, 0x60000, 0x60B80, 0x6C000, 0x70000, 0x70B80, 0x7C000};
  unsigned short seed_a = 0, seed_b = 0;

  chk_found = 0;
  chk_fixed = 0;

  for (; chk_found < 11; chk_found++)
  {
    chk_start_addr=chk_array[chk_found];
    chk_end_addr=chk_array[chk_found + 1];

    if (!first_pass)
    {
      seed_a |= 0x8631;
      seed_b |= 0xEFCD;
    }

    chk_oldvalue = ((unsigned int)file_buffer[chk_end_addr - 1] << 24)
                 + ((unsigned int)file_buffer[chk_end_addr - 2] << 16)
                 + ((unsigned int)file_buffer[chk_end_addr - 3] << 8)
                 + (unsigned int)file_buffer[chk_end_addr - 4];

    chk_value = tdi41_checksum_calculate(chk_start_addr, chk_end_addr - 4, seed_a, seed_b);

    if (chk_oldvalue != chk_value)
    {
      file_buffer[chk_end_addr - 4] = chk_value & 0x000000ff;
      file_buffer[chk_end_addr - 3] = (chk_value >> 8) & 0x000000ff;
      file_buffer[chk_end_addr - 2] = (chk_value >> 16) & 0x000000ff;
      file_buffer[chk_end_addr - 1] = (chk_value >> 24) & 0x000000ff;

      chk_fixed++;
    }

    first_pass = false;
  }
}

//---------------------------------------------------------------------------
// Calculate new checksum for a block of data
//---------------------------------------------------------------------------

unsigned int __fastcall TfrmMain::tdi41_checksum_calculate(unsigned int chk_start_addr, unsigned int chk_end_addr, unsigned short seed_a, unsigned short seed_b)
{
  unsigned short var_1;
  unsigned char var_2;

	do
  {
    var_2 = 0;
    seed_a ^= (((unsigned short)file_buffer[chk_start_addr + 1] << 8) + (unsigned short)file_buffer[chk_start_addr]);
    chk_start_addr += 2;

    if ((seed_b & 0xF) > 0)
    {
      var_1= seed_a >> (16 - (seed_b & 0xF));
      seed_a <<= (seed_b & 0xF);
      seed_a |= var_1;

      var_2 = seed_a & 1;
    }

    seed_b -= (((unsigned short)file_buffer[chk_start_addr + 1] << 8) + (unsigned short)file_buffer[chk_start_addr]);
    seed_b -= var_2;
    chk_start_addr += 2;
    seed_b ^= seed_a;

    if ( chk_start_addr == chk_end_addr )
      break;

    seed_a -= (((unsigned short)file_buffer[chk_start_addr + 1] << 8) + (unsigned short)file_buffer[chk_start_addr]);
    chk_start_addr += 2;
    seed_a += 0xDAAC;
    seed_b ^= (((unsigned short)file_buffer[chk_start_addr + 1] << 8) + (unsigned short)file_buffer[chk_start_addr]);
    chk_start_addr += 2;

    if ((seed_a & 0xF) > 0)
    {
      var_1=seed_b << ( 16 - (seed_a & 0xF));
      seed_b >>= (seed_a & 0xF);
      seed_b |= var_1;
    }
  }
  while (chk_start_addr != chk_end_addr);

  seed_a -= 0x8631;
  seed_a += 0xDAAC;
  seed_b ^= 0xDF9B;

	return (((unsigned int)seed_b << 16) + seed_a);
}