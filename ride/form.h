#ifndef FORM_H
#define FORM_H

#include "ride/wx.h"
#include <string>
#include <google/protobuf/stubs/common.h>
class wxEditableListBox;

#define RETURN_COMBOBOX_VALUE(TYPE, VALUE) assert(ride::TYPE##_IsValid(VALUE)); return static_cast<ride::TYPE>(VALUE)
#define DIALOG_DATA(ROOT, FUN, UI, SETNAME) do { if( togui ) { ToGui(ROOT.FUN(), UI); } else { ROOT.set_##FUN(ToData##SETNAME(UI)); } } while(false)
#define DIALOG_DATAX(ROOT, FUN, UI) do { if( togui ) { ToGui(ROOT.FUN(), UI); } else { ROOT.set_allocated_##FUN(Allocate(ToData(UI))); } } while(false)

template<typename T>
T* Allocate(const T& t) {
  return new T(t);
}

void ToGui(bool data, wxCheckBox* gui);
bool ToData(wxCheckBox* gui);

void ToGui(google::protobuf::int32 data, wxTextCtrl* gui);
google::protobuf::int32 ToData_I32(wxTextCtrl* gui);

void ToGui(std::string data, wxListBox* gui);
std::string ToData(wxListBox* gui);

void ToGui(const wxString& data, wxTextCtrl* gui);
wxString ToData_Str(wxTextCtrl* gui);

void ToGui(const std::vector<wxString>& data, wxEditableListBox * gui);
std::vector<wxString> ToData(wxEditableListBox * gui);

#endif // FORM_H