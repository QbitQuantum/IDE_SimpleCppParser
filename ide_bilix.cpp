//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ide_bilix.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner): TForm(Owner)
{
	TreeView1 = new TDirectoryTreeView(Owner);
	TreeView1->Parent = this;
    TreeView1->Init();
	TreeView1->SetMemo(Memo1);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
	String path;
	if (SelectDirectory("", "C:\\Users\\AxeMODE\\Desktop", path)) {
		TreeView1->LoadDirectory(path);
	}
}
//---------------------------------------------------------------------------
