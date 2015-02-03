#include <wx/wx.h>
#include <wx/stc/stc.h>
#include <wx/aui/aui.h>
#include <wx/filename.h>

#include "ride/mainwindow.h"
#include "ride/fileedit.h"

#include "ride/settingsdlg.h"

enum
{
  ID_SHOW_SETTINGS = wxID_HIGHEST + 1
};

#define RIDE_AUI_SETTINGS_ID "settings"

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_MENU(wxID_OPEN, MainWindow::OnOpen)
EVT_MENU(wxID_EXIT, MainWindow::OnExit)
EVT_MENU(wxID_ABOUT, MainWindow::OnAbout)
EVT_MENU(ID_SHOW_SETTINGS, MainWindow::ShowSettings)

EVT_MENU(wxID_SAVE, MainWindow::OnSave)
EVT_MENU(wxID_SAVEAS, MainWindow::OnSaveAs)

EVT_CLOSE(OnClose)

EVT_AUINOTEBOOK_PAGE_CLOSE(wxID_ANY, MainWindow::OnNotebookPageClose)
EVT_AUINOTEBOOK_PAGE_CLOSED(wxID_ANY, MainWindow::OnNotebookPageClosed)
wxEND_EVENT_TABLE()

enum {
  mySTC_TYPE_DEFAULT,
  mySTC_TYPE_COMMENT,
  mySTC_TYPE_COMMENT_LINE,
  mySTC_TYPE_COMMENT_DOC,
  mySTC_TYPE_NUMBER,
  mySTC_TYPE_WORD1,
  mySTC_TYPE_STRING,
  mySTC_TYPE_CHARACTER,
  mySTC_TYPE_UUID,
  mySTC_TYPE_PREPROCESSOR,
  mySTC_TYPE_OPERATOR,
  mySTC_TYPE_IDENTIFIER,
  mySTC_TYPE_STRING_EOL,
  mySTC_TYPE_REGEX,
  mySTC_TYPE_COMMENT_SPECIAL,
  mySTC_TYPE_WORD2,
  mySTC_TYPE_WORD3,
  mySTC_TYPE_ERROR
};

MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(NULL, wxID_ANY, title, pos, size)
{
  aui.SetManagedWindow(this);

  settings.load();

  wxMenu *menuFile = new wxMenu;
  menuFile->Append(wxID_OPEN, "&Open...\tCtrl-O", "Open a file");
  menuFile->Append(wxID_SAVE, "&Save...\tCtrl-S", "Save the file");
  menuFile->Append(wxID_SAVEAS, "Save &as...\tCtrl-Shift-S", "Save the file as a new file");
  menuFile->AppendSeparator();
  menuFile->Append(ID_SHOW_SETTINGS, "S&ettings...", "Change the settings of RIDE");
  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT);
  wxMenu *menuHelp = new wxMenu;
  menuHelp->Append(wxID_ABOUT);
  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append(menuFile, "&File");
  menuBar->Append(menuHelp, "&Help");
  SetMenuBar(menuBar);
  CreateStatusBar();
  SetStatusText("");
  createNotebook();

  settingsdlg = new SettingsDlg(this, this);
  aui.AddPane(settingsdlg, wxAuiPaneInfo().
    Name(wxT(RIDE_AUI_SETTINGS_ID)).Caption(wxT("Settings")).
    DestroyOnClose(false).CloseButton(false).
    Dockable(false).Float().Hide());

  aui.Update();
}

void MainWindow::createNotebook() {
  wxSize client_size = GetClientSize();
  wxAuiNotebook* ctrl = new wxAuiNotebook(this, wxID_ANY,
    wxPoint(client_size.x, client_size.y),
    wxSize(430, 200),
    wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER);
  ctrl->Freeze();
  ctrl->Thaw();
  notebook = ctrl;

  aui.AddPane(notebook, wxAuiPaneInfo().Name(wxT("notebook_content")).PaneBorder(true));
}

const Settings& MainWindow::getSettings() const {
  return settings;
}

MainWindow::~MainWindow() {
  aui.UnInit();
}

void MainWindow::OnExit(wxCommandEvent& event)
{
  Close(true);
}

void MainWindow::OnAbout(wxCommandEvent& event)
{
  wxMessageBox("Ride is a Rust IDE. It's named after concatenating R from rust and IDE.\nFor more information: https://github.com/madeso/ride", "About Ride", wxOK | wxICON_INFORMATION);
}

void MainWindow::OnOpen(wxCommandEvent& event)
{
  wxFileDialog
    openFileDialog(this, _("Open file"), "", "",
    FILE_PATTERN, wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE );
  if (openFileDialog.ShowModal() == wxID_CANCEL)
    return;

  wxArrayString paths;
  openFileDialog.GetPaths(paths);
  for (wxArrayString::iterator path = paths.begin(); path != paths.end(); ++path) {
    wxFileName w(*path);
    w.Normalize();
    new FileEdit(notebook, this, "", w.GetFullPath());
  }
}

FileEdit* MainWindow::getSelectedEditorNull() {
  const int selected = notebook->GetSelection();
  wxWindow* window = notebook->GetPage(selected);
  if (window->IsKindOf(CLASSINFO(FileEdit))) {
    FileEdit* edit = reinterpret_cast<FileEdit*>(window);
    return edit;
  }
  else {
    return NULL;
  }
}

void MainWindow::OnSave(wxCommandEvent& event) {
  FileEdit* selected = getSelectedEditorNull();
  if (selected == NULL) return;
  selected->save();
}

void MainWindow::OnSaveAs(wxCommandEvent& event) {
  FileEdit* selected = getSelectedEditorNull();
  if (selected == NULL) return;
  selected->saveAs();
}

void MainWindow::OnNotebookPageClose(wxAuiNotebookEvent& evt) {
  wxWindow* window = notebook->GetPage(evt.GetSelection());
  if (window->IsKindOf(CLASSINFO(FileEdit))) {
    FileEdit* edit = reinterpret_cast<FileEdit*>(window);
    if (edit->canClose(true) == false) {
      evt.Veto();
    }
  }
}

void MainWindow::OnClose(wxCloseEvent& evt) {
  for (unsigned int i = 0; i < notebook->GetPageCount(); ++i) {
    wxWindow* window = notebook->GetPage(i);
    if (window->IsKindOf(CLASSINFO(FileEdit))) {
      FileEdit* edit = reinterpret_cast<FileEdit*>(window);
      const bool canAbort = evt.CanVeto();
      if (edit->canClose(canAbort) == false) {
        evt.Veto();
        return;
      }
    }
  }

  evt.Skip();
}

void MainWindow::setSettings(const Settings& settings) {
  assert(this);
  this->settings = settings;
  updateAllEdits();
}

void MainWindow::OnNotebookPageClosed(wxAuiNotebookEvent& evt) {
}

void MainWindow::updateAllEdits() {
  // todo
}

void MainWindow::ShowSettings(wxCommandEvent& event) {
  settingsdlg->PrepareForShow();
  wxAuiPaneInfo& sett = aui.GetPane(wxT(RIDE_AUI_SETTINGS_ID)).Float().Show();
  aui.Update();
}

void MainWindow::hideSettingsDlg() {
  aui.GetPane(wxT(RIDE_AUI_SETTINGS_ID)).Float().Hide();
  aui.Update();
}
