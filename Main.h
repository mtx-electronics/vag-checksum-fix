//---------------------------------------------------------------------------
//
// VAG Checksum Fix
// Version 1.0
//
// Copyright 2012 MTX Electronics
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

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>

#include <stdio.h>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
  TGroupBox *GroupBox1;
  TRadioButton *rbTDI41;
  TRadioButton *rbTDI41_2002;
  TLabel *Label1;
  TLabel *Label2;
  TEdit *editSrcFilename;
  TEdit *editDstFilename;
  TButton *btnSrcBrowse;
  TButton *btnDstBrowse;
  TButton *btnChkFix;
  TStatusBar *StatusBar;
  TOpenDialog *OpenDialog;
  TSaveDialog *SaveDialog;
  TImage *Image1;
  void __fastcall btnSrcBrowseClick(TObject *Sender);
  void __fastcall FormCreate(TObject *Sender);
  void __fastcall btnChkFixClick(TObject *Sender);

private:	// User declarations
  unsigned char *file_buffer;
  unsigned int file_size;
  unsigned short chk_found, chk_fixed;

  void __fastcall tdi41_checksum_search(void);
  unsigned int __fastcall tdi41_checksum_calculate(unsigned int chk_start_addr, unsigned int chk_end_addr, unsigned short seed_1, unsigned short seed_2);

  void __fastcall tdi41_2002_checksum_search(void);
  unsigned int __fastcall tdi41_2002_checksum_calculate(unsigned int chk_start_addr, unsigned int chk_end_addr, unsigned short seed_a, unsigned short seed_b, unsigned short seed_c, unsigned short seed_d, bool first_pass);

public:		// User declarations
  __fastcall TfrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
