//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "DirectoryTreeView.hpp"
#include <System.IOUtils.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TDirectoryTreeView::TDirectoryTreeView(TComponent* Owner)
    : TTreeView(Owner)
{

    // Подключаем обработчики событий
    this->OnExpanding = &HandleExpanding;
    this->OnCollapsing = &HandleCollapsing;
    this->OnChange = &HandleChange;
    this->OnDblClick = &HandleDblClick;
}
//---------------------------------------------------------------------------
void TDirectoryTreeView::LoadDirectory(String Path)
{
    if (Path == "")
        return;

    FBasePath = Path;
    Items->Clear();

    Items->BeginUpdate();
    Items->AddChild(Items->Add(nullptr, FBasePath), ""); // Фиктивный узел для знака "+"
    Items->EndUpdate();
}
//---------------------------------------------------------------------------

void TDirectoryTreeView::Init()
{
    Top += 50;
    Height += 400;
    Left += 8;
    Width += 250;
}
//---------------------------------------------------------------------------

void TDirectoryTreeView::SetMemo(TMemo* Memo)
{
    FMemo = Memo;
}
//---------------------------------------------------------------------------

void __fastcall TDirectoryTreeView::AddSubItems(TTreeNode* Node, String Path)
{
    if (Node->Count == 1 && Node->Item[0]->Text == "")
        Node->Item[0]->Delete();

    TSearchRec sr;
    String fullItemPath = Path + "\\*.*";
    if (FindFirst(fullItemPath, faAnyFile, sr) == 0) {
        while (FindNext(sr) == 0) {
            if (sr.Name == "." || sr.Name == "..")
                continue;
            (sr.Attr & faDirectory) ?
                Items->AddChild(Items->AddChild(Node, sr.Name), "") :
                Items->AddChild(Node, sr.Name);
        };
        FindClose(sr);
    }
}
//---------------------------------------------------------------------------

// Обработчик изменения выбора
void __fastcall TDirectoryTreeView::HandleChange(TObject* Sender, TTreeNode* Node)
{
    CurrentNode = Node;
}
//---------------------------------------------------------------------------

// Обработчик выбора элемента
void __fastcall TDirectoryTreeView::HandleDblClick(TObject* Sender)
{

    if (!FMemo || !CurrentNode) return;

    String fullPath = GetFullPath(CurrentNode);
    // Проверяем, является ли выбранный элемент файлом
    if (FileExists(fullPath)) {
        try {
            // Загружаем содержимое файла в Memo
            FMemo->Lines->Text = TFile::ReadAllText(fullPath, TEncoding::UTF8);
        }
        catch (Exception& e) {
            FMemo->Lines->Text = "Ошибка при чтении файла: " + e.Message;
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TDirectoryTreeView::HandleExpanding(TObject* Sender, TTreeNode* Node, bool& AllowExpansion)
{
    Items->BeginUpdate();

    String fullPath = "";
    TTreeNode* currentNode = Node;

    while (currentNode) {
        fullPath = currentNode->Text + "\\" + fullPath;
        currentNode = currentNode->Parent;
    }


    AddSubItems(Node, fullPath);

    Items->EndUpdate();
    AllowExpansion = true;
}
//---------------------------------------------------------------------------
void __fastcall TDirectoryTreeView::HandleCollapsing(TObject* Sender, TTreeNode* Node, bool& AllowCollapse)
{
    Items->BeginUpdate();
    Node->DeleteChildren();
    Items->AddChild(Node, "");
    Items->EndUpdate();
    AllowCollapse = true;
}
//---------------------------------------------------------------------------

// Получение полного пути к выбранному узлу
String __fastcall TDirectoryTreeView::GetFullPath(TTreeNode* Node)
{
    if (!Node) return "";

    String fullPath = Node->Text;
    TTreeNode* parent = Node->Parent;

    while (parent) {
        fullPath = parent->Text + "\\" + fullPath;
        parent = parent->Parent;
    }

    return fullPath;
}
//---------------------------------------------------------------------------
