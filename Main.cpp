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

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
#include "Bosch_VAG_TDI_v41.cpp"
#include "Bosch_VAG_TDI_v41_2002.cpp"
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnSrcBrowseClick(TObject *Sender)
{
  if (OpenDialog->Execute())
  {
    if (FileExists(OpenDialog->FileName))
    {
      editSrcFilename->Text=OpenDialog->FileName;
      editDstFilename->Text=ChangeFileExt(OpenDialog->FileName,".chkfix");
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormCreate(TObject *Sender)
{
  if ((file_buffer=(unsigned char*) malloc (0x80000)) == NULL)
    Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnChkFixClick(TObject *Sender)
{
  FILE *f_handle;

  // Verify if checksum type is selected
  if ((!rbTDI41->Checked) && (!rbTDI41_2002->Checked))
  {
    StatusBar->SimpleText="Please select checksum type. Aborted!";
    return;
  }

  // Load source file for checksum calculation
  if ((f_handle = fopen(editSrcFilename->Text.c_str(), "rb")) != NULL)
  {
    fseek(f_handle, 0L, SEEK_END);
    file_size = ftell(f_handle);
    rewind(f_handle);

    if (file_size != 0x80000)
    {
      StatusBar->SimpleText="Wrong file size. Aborted!";
      return;
    }

    fread(file_buffer, file_size, 1, f_handle);
    fclose (f_handle);
  }
  else
  {
    StatusBar->SimpleText="Can't open source file. Aborted!";
    return;
  }

  // Calculate new checksum
  if (rbTDI41->Checked)
    tdi41_checksum_search();
  else
    tdi41_2002_checksum_search();

  // Save file with updated checksum
  if ((f_handle = fopen(editDstFilename->Text.c_str(), "wb")) != NULL)
  {
    fwrite(file_buffer, file_size, 1, f_handle);
    fclose (f_handle);
  }
  else
  {
    StatusBar->SimpleText="Can't create destination file. Aborted!";
    return;
  }

  // Update status
  editSrcFilename->Text="";
  editDstFilename->Text="";
  
  StatusBar->SimpleText="Found " + IntToStr(chk_found) + " checksums, updated " + IntToStr(chk_fixed) + " checksums.";
}
//---------------------------------------------------------------------------
