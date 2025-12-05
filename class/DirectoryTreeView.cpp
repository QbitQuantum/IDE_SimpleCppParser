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
    FBasePath = "C:\\Users\\AxeMODE\\Desktop";
    FMemo = NULL;

    // Подключаем обработчики событий
    this->OnExpanding = &HandleExpanding;
    this->OnCollapsing = &HandleCollapsing;
    this->OnChange = &HandleChange;
    this->OnDblClick = &HandleDblClick;
}
//---------------------------------------------------------------------------
void TDirectoryTreeView::LoadDirectory(String Path)
{
    if (Path != "") {
        FBasePath = Path;
    }

    Items->BeginUpdate();
    Items->Clear();

    TTreeNode* root = Items->Add(NULL, FBasePath);
    Items->AddChild(root, ""); // Фиктивный узел для знака "+"
    
    Items->EndUpdate();
}
//---------------------------------------------------------------------------

void TDirectoryTreeView::Init()
{
    Top += 50;
    Height += 400;
    Left += 8;
    Width += 20;
}
//---------------------------------------------------------------------------

void TDirectoryTreeView::SetMemo(TMemo* Memo)
{
    FMemo = Memo;
}
//---------------------------------------------------------------------------

void __fastcall TDirectoryTreeView::AddSubItems(TTreeNode* Node, String Path)
{
    if (Node->Count > 0 && Node->Item[0]->Text != "") {
        return;
    }

    TSearchRec sr;
    if (FindFirst(Path + "\\*.*", faAnyFile, sr) == 0) {
        do {
            if (sr.Name != "." && sr.Name != "..") {
                String fullItemPath = Path + "\\" + sr.Name;

                if (DirectoryExists(fullItemPath)) {
                    TTreeNode* childNode = Items->AddChild(Node, sr.Name);
                    Items->AddChild(childNode, "");
                }
                else {
                    Items->AddChild(Node, sr.Name);
                }
            }
        } while (FindNext(sr) == 0);
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

    while (currentNode != NULL) {
        if (fullPath != "") {
            fullPath = currentNode->Text + "\\" + fullPath;
        }
        else {
            fullPath = currentNode->Text;
        }
        currentNode = currentNode->Parent;
    }

    if (Node->Count == 1 && Node->Item[0]->Text == "") {
        Node->Item[0]->Delete();
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

    while (parent != NULL) {
        fullPath = parent->Text + "\\" + fullPath;
        parent = parent->Parent;
    }

    return fullPath;
}
//---------------------------------------------------------------------------
