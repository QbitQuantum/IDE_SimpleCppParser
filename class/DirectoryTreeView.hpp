//---------------------------------------------------------------------------
#ifndef DirectoryTreeViewH
#define DirectoryTreeViewH
//---------------------------------------------------------------------------
#include <Vcl.ComCtrls.hpp>
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
class TDirectoryTreeView : public TTreeView
{
private:
    TTreeNode* CurrentNode;
    String FBasePath;
    TMemo* FMemo;
    void __fastcall AddSubItems(TTreeNode* Node, String Path);
    void __fastcall HandleExpanding(TObject* Sender, TTreeNode* Node, bool& AllowExpansion);
    void __fastcall HandleCollapsing(TObject* Sender, TTreeNode* Node, bool& AllowCollapse);
    void __fastcall HandleChange(TObject* Sender, TTreeNode* Node);
    void __fastcall HandleDblClick(TObject* Sender);

    String __fastcall GetFullPath(TTreeNode* Node);

public:
    __fastcall TDirectoryTreeView(TComponent* Owner);

    void LoadDirectory(String Path = "");
    void Init();
    void SetMemo(TMemo * Memo);

    __property String BasePath = { read = FBasePath, write = FBasePath };
};
//---------------------------------------------------------------------------
#endif