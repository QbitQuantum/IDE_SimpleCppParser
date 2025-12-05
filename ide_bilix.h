//---------------------------------------------------------------------------

#ifndef ide_bilixH
#define ide_bilixH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.FileCtrl.hpp>
#include <Vcl.Dialogs.hpp>
#include "DirectoryTreeView.hpp"
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TButton *Button1;
	TMemo *Memo1;
	void __fastcall Button1Click(TObject *Sender);

private:	// User declarations
    TDirectoryTreeView *TreeView1;
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
