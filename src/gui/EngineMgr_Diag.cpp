/////////////////////////////////////////////////////////////////////////////
// Name:        EngineMgr_Diag.cpp
// Purpose:     Manager of Engines
// Author:      Matthew Gong
// Created:     05/03/2005
// Copyright:   (c) Matthew Gong
// Licence:     GPL licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
// #pragma implementation
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

// -*- C++ -*- generated by wxGlade 0.3.5.1 on Tue May  3 16:09:10 2005

#include <wx/dir.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/fileconf.h>

#include "EngineMgr_Diag.h"
#include "Engine_Diag.h"
#include "Turbine_Diag.h"
#include "Commander.h"

#ifndef __WXMSW__
#include "add.xpm"
#include "del.xpm"
#include "dupl.xpm"
#include "detail.xpm"
#include "rocket_engine.xpm"
#include "piston_engine.xpm"
#include "turbine_engine.xpm"
#include "propeller.xpm"
#include "nozzle.xpm"
#include "direct.xpm"
#include "electric_engine.xpm"
#endif

BEGIN_EVENT_TABLE(EngineMgrDialog, wxDialog)
  EVT_LIST_ITEM_ACTIVATED(ENGINE_LIST, EngineMgrDialog::OnDoubleClickList) 
  EVT_LIST_ITEM_SELECTED(ENGINE_LIST, EngineMgrDialog::OnSelectList)
  EVT_LIST_ITEM_DESELECTED(ENGINE_LIST, EngineMgrDialog::OnDeselectList)
  EVT_TEXT_ENTER(ENGINE_NAME, EngineMgrDialog::OnEnterName)
  EVT_BUTTON(wxID_APPLY, EngineMgrDialog::OnApply)
  EVT_BUTTON(ENGINE_DEL, EngineMgrDialog::OnDel)
  EVT_BUTTON(ENGINE_ADD, EngineMgrDialog::OnAdd)
  EVT_BUTTON(ENGINE_DUP, EngineMgrDialog::OnDup)
  EVT_BUTTON(ENGINE_DETAIL, EngineMgrDialog::OnDetail)
END_EVENT_TABLE()

EngineMgrDialog::EngineMgrDialog(const wxString & eng, wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
    wxDialog(parent, id, title, pos, size, style),
    config(NULL)
{
    // begin wxGlade: EngineMgrDialog::EngineMgrDialog
    engine_list = new wxListCtrl(this, ENGINE_LIST, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxSUNKEN_BORDER|wxLC_SINGLE_SEL);
    label_symbol = new wxStaticText(this, -1, _("Symbol:"));
    text_ctrl_symbol = new wxTextCtrl(this, -1, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    label_type = new wxStaticText(this, -1, _("Type:"));
    text_ctrl_type = new wxTextCtrl(this, -1, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    label_name = new wxStaticText(this, -1, _("Name:"));
    text_ctrl_name = new wxTextCtrl(this, ENGINE_NAME, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    label_dscp = new wxStaticText(this, -1, _("Description:"));
    text_ctrl_dscp = new wxTextCtrl(this, ENGINE_DSCP, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    bitmap_button_add = new wxBitmapButton(this, ENGINE_ADD, wxBITMAP(add));
    bitmap_button_del = new wxBitmapButton(this, ENGINE_DEL, wxBITMAP(del));
    bitmap_button_dup = new wxBitmapButton(this, ENGINE_DUP, wxBITMAP(dupl));
    btn_detail = new wxButton(this, ENGINE_DETAIL, _("Detail"));
    btn_apply = new wxButton(this, wxID_APPLY, _("Apply"));
    btn_close = new wxButton(this, wxID_OK, _("OK"));

    set_properties();
    do_layout();
    // end wxGlade

    after_construct();

    long i = -1;
    i = engine_list->FindItem(i, eng);
    if ( i == -1 )
    {
      text_ctrl_symbol->SetValue(wxEmptyString);
      text_ctrl_type->SetValue(wxEmptyString);
      text_ctrl_name->SetValue(wxEmptyString);
      text_ctrl_dscp->SetValue(wxEmptyString);  
    }
    else
    {
      wxListItem item;
      item.SetId(i);
      item.SetColumn(0);
      item.SetText(wxEmptyString);
      item.SetState(wxLIST_STATE_SELECTED|wxLIST_STATE_FOCUSED);
      engine_list->GetItem(item);
      wxString sac = item.GetText();
      engine_list->SetItemState(i, (~engine_list->GetItemState(i, wxLIST_STATE_SELECTED) ) & wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
      text_ctrl_symbol->SetValue(sac);
      wxString tmp;
      if ( config->Read( sac + wxT("/type"), &tmp) )
        text_ctrl_type->SetValue(tmp);
      if ( config->Read( sac + wxT("/name"), &tmp) )
        text_ctrl_name->SetValue(tmp);
      if ( config->Read( sac + wxT("/description"), &tmp) )
        text_ctrl_dscp->SetValue(tmp);
    }
}

EngineMgrDialog::~EngineMgrDialog()
{
  before_destruct();
}

void EngineMgrDialog::set_properties()
{
    // begin wxGlade: EngineMgrDialog::set_properties
    engine_list->SetMinSize(wxSize(640, 150));
    engine_list->SetFocus();
    bitmap_button_add->SetSize(bitmap_button_add->GetBestSize());
    bitmap_button_del->SetSize(bitmap_button_del->GetBestSize());
    bitmap_button_dup->SetSize(bitmap_button_dup->GetBestSize());
    // end wxGlade
    bitmap_button_add->SetToolTip(_("Add new one"));
    bitmap_button_del->SetToolTip(_("Delete the selected one"));
    bitmap_button_dup->SetToolTip(_("Copy from the selected one"));
    btn_detail->SetToolTip(_("Show the detail"));
}


void EngineMgrDialog::do_layout()
{
    // begin wxGlade: EngineMgrDialog::do_layout
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_3 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_7 = new wxBoxSizer(wxHORIZONTAL);
    sizer_1->Add(engine_list, 2, wxALL|wxEXPAND, 5);
    sizer_7->Add(bitmap_button_add, 0, wxALIGN_CENTER_VERTICAL, 5);
    sizer_7->Add(bitmap_button_del, 0, wxALIGN_CENTER_VERTICAL, 5);
    sizer_7->Add(bitmap_button_dup, 0, wxALIGN_CENTER_VERTICAL, 5);
    sizer_7->Add(btn_detail, 1, wxALIGN_CENTER_VERTICAL, 5);
    sizer_7->Add(btn_apply, 0, wxALIGN_CENTER_VERTICAL, 5);
    sizer_7->Add(btn_close, 0, wxALIGN_CENTER_VERTICAL, 5);
    sizer_1->Add(sizer_7, 0, wxTOP|wxBOTTOM|wxEXPAND, 5);
    sizer_3->Add(label_symbol, 0, wxLEFT|wxRIGHT, 5);
    sizer_3->Add(text_ctrl_symbol, 1, wxRIGHT, 5);
    sizer_3->Add(label_type, 0, wxLEFT|wxRIGHT, 5);
    sizer_3->Add(text_ctrl_type, 1, wxRIGHT, 5);
    sizer_3->Add(label_name, 0, wxLEFT|wxRIGHT, 5);
    sizer_3->Add(text_ctrl_name, 1, wxRIGHT, 5);
    sizer_1->Add(sizer_3, 0, wxTOP|wxBOTTOM|wxEXPAND, 5);
    sizer_1->Add(label_dscp, 0, wxALL, 5);
    sizer_1->Add(text_ctrl_dscp, 1, wxALL|wxEXPAND, 5);
    SetAutoLayout(true);
    SetSizer(sizer_1);
    sizer_1->Fit(this);
    sizer_1->SetSizeHints(this);
    Layout();
    // end wxGlade
}

void EngineMgrDialog::InitEngineCfg()
{
  wxFileConfig * config = new wxFileConfig(wxT("EngineMgr"), wxT("nuaa"), wxT("EngineMgr.ini"), wxT("EngineMgr.ini"), wxCONFIG_USE_LOCAL_FILE|wxCONFIG_USE_RELATIVE_PATH);

  //first, delete all data of the lost engines.
  {
    long id = 732;
    wxString tmp;
    wxArrayString losts;
    bool flag = config->GetFirstEntry(tmp, id);
    while (flag)
    {
      losts.Add(tmp);
      flag = config->GetNextEntry(tmp, id);
    }
    for (int i=0; i<losts.Count(); ++i)
    {
      config->DeleteEntry(losts[i]);
    }    
  }
  {
    wxArrayString lost_Engines;
    long id = 74l;
    wxString engine_id;
    bool flag = config->GetFirstGroup(engine_id, id);
    while (flag)
    {
      wxString ca = wxGetApp().engine_dir + wxGetApp().Delimited + engine_id;
      if (!::wxFileExists(ca+wxT(".xml")))
        lost_Engines.Add(engine_id);
      flag = config->GetNextGroup(engine_id, id);
    }
    for (int i=0; i<lost_Engines.Count(); ++i)
    {
      config->DeleteGroup(lost_Engines[i]);
    }
  }

  //add missing engine(s)
  {
    wxDir dir(wxGetApp().engine_dir);
    wxString filename;
    wxString fullpath;
    bool flag = dir.GetFirst(&filename, wxT("*.xml"), wxDIR_FILES);
    while (flag)
    {
      fullpath = wxGetApp().engine_dir + wxGetApp().Delimited + filename;
      filename = filename.BeforeLast('.');
      if (!config->HasGroup(filename))
      {
        bool flag = true;
        JSBSim::FGXMLParse * controls_file_parser = new JSBSim::FGXMLParse;
        try
        {
          readXML (fullpath.c_str(), *controls_file_parser);
        }
        catch (...)
        {
          flag = false;
          ::wxMessageBox(wxString(_("Can not parse file ")) + fullpath, _("Info"), wxOK | wxICON_INFORMATION, NULL);
        }

        JSBSim::Element * document=NULL;
        if (flag)
        {
          document = controls_file_parser->GetDocument();
          wxString type = document->GetName();
          wxString prefix = filename + wxT("/");
          config->Write(prefix + wxT("type"), type);
          config->Write(prefix + wxT("name"), filename);
          config->Write(prefix + wxT("description"), wxT("NO INFO"));
        }
        delete controls_file_parser;
      }
      flag = dir.GetNext(&filename);
    }
  }
  
  delete config;
}

void EngineMgrDialog::after_construct()
{
  InitEngineCfg();
  
  config = new wxFileConfig(wxT("EngineMgr"), wxT("nuaa"), wxT("EngineMgr.ini"), wxT("EngineMgr.ini"), wxCONFIG_USE_LOCAL_FILE|wxCONFIG_USE_RELATIVE_PATH);

  wxBitmap bmp(wxBITMAP(piston_engine));
  wxImageList * images = new wxImageList(bmp.GetWidth(), bmp.GetHeight());
  images->Add(bmp); // 0
  images->Add(wxBITMAP(turbine_engine));
  images->Add(wxBITMAP(rocket_engine));
  images->Add(wxBITMAP(electric_engine));
  images->Add(wxBITMAP(propeller));
  images->Add(wxBITMAP(nozzle));  // 5
  images->Add(wxBITMAP(direct));

  engine_list->AssignImageList(images, wxIMAGE_LIST_SMALL);
  wxListItem itemCol;
  itemCol.SetText(_("Symbol"));
  itemCol.SetImage(-1);
  engine_list->InsertColumn(0, itemCol);
  itemCol.SetText(_("Name"));
  itemCol.SetImage(-1);
  engine_list->InsertColumn(1, itemCol);
  itemCol.SetText(_("Type"));
  itemCol.SetImage(-1);
  engine_list->InsertColumn(2, itemCol);
  itemCol.SetText(_("Description"));
  itemCol.SetImage(-1);
  engine_list->InsertColumn(3, itemCol);

  //init engine_list
  {
    long id = 75l;
    wxString engine_id;
    engine_list->DeleteAllItems();
    bool flag = config->GetFirstGroup(engine_id, id);
    long no = 0;
    wxString tmp;
    while (flag)
    {
      config->Read( engine_id + wxT("/type"), &tmp);
      wxListItem item;
      item.SetId(no++);
      item.SetColumn(0);
      item.SetText(engine_id);
      if (tmp == wxT("piston_engine"))
      {
        item.SetImage(0);
      }
      else if (tmp == wxT("turbine_engine"))
      {
        item.SetImage(1);      
      }
      else if (tmp == wxT("rocket_engine"))
      {
        item.SetImage(2);      
      }
      else if (tmp == wxT("electric_engine"))
      {
        item.SetImage(3);      
      }
      else if (tmp == wxT("propeller"))
      {
        item.SetImage(4);      
      }
      else if (tmp == wxT("nozzle"))
      {
        item.SetImage(5);      
      }
      else if (tmp == wxT("direct"))
      {
        item.SetImage(6);      
      }
      else
      {
        item.SetImage(-1);      
      }
      engine_list->InsertItem(item);
      item.SetColumn(1);
      item.SetText(config->Read( engine_id + wxT("/name"), engine_id));
      item.SetImage(-1);
      engine_list->SetItem(item);
      item.SetColumn(2);
      item.SetText(config->Read( engine_id + wxT("/type"), wxEmptyString));
      item.SetImage(-1);
      engine_list->SetItem(item);
      item.SetColumn(3);
      item.SetText(config->Read( engine_id + wxT("/description"), wxT("NO INFO")));
      item.SetImage(-1);
      engine_list->SetItem(item);

      flag = config->GetNextGroup(engine_id, id);
    }
    engine_list->SetColumnWidth( 0, wxLIST_AUTOSIZE );
    engine_list->SetColumnWidth( 1, wxLIST_AUTOSIZE );
    engine_list->SetColumnWidth( 2, wxLIST_AUTOSIZE );
    engine_list->SetColumnWidth( 3, wxLIST_AUTOSIZE );
  }
}

void EngineMgrDialog::before_destruct()
{
    long item = -1;
    item = engine_list->GetNextItem(item,
                                    wxLIST_NEXT_ALL,
                                    wxLIST_STATE_SELECTED);
    if ( item != -1 )
    { 
      wxString sac = text_ctrl_symbol->GetValue();
      config->Write( sac + wxT("/name"), text_ctrl_name->GetValue());
      config->Write( sac + wxT("/description"), text_ctrl_dscp->GetValue());
    }

    delete config;
}

void EngineMgrDialog::OnDeselectList(wxListEvent & event)
{
  long i = event.GetIndex();
  apply(i);
}

void EngineMgrDialog::OnSelectList(wxListEvent & event)
{
  long i = event.GetIndex();
  if ( i == -1 )
    return;
  
  wxString osac = text_ctrl_symbol->GetValue();
  if (!osac.IsEmpty())
  {
    config->Write( osac + wxT("/name"), text_ctrl_name->GetValue());
    config->Write( osac + wxT("/description"), text_ctrl_dscp->GetValue());
    engine_list->SetColumnWidth( 1, wxLIST_AUTOSIZE );
    engine_list->SetColumnWidth( 2, wxLIST_AUTOSIZE );
  }
  
  wxListItem item;
  item.SetId(i);
  item.SetColumn(0);
  item.SetText(wxEmptyString);
  engine_list->GetItem(item);
  wxString sac = item.GetText();
  text_ctrl_symbol->SetValue(sac);
  wxString tmp;
  if ( config->Read(sac + wxT("/type"), &tmp) )
    text_ctrl_type->SetValue(tmp);
  if ( config->Read(sac + wxT("/name"), &tmp) )
    text_ctrl_name->SetValue(tmp);
  if ( config->Read(sac + wxT("/description"), &tmp) )
    text_ctrl_dscp->SetValue(tmp);
}

void EngineMgrDialog::OnEnterName(wxCommandEvent & event)
{
  long i = -1;
  i = engine_list->GetNextItem(i,
                                 wxLIST_NEXT_ALL,
                                 wxLIST_STATE_SELECTED);
  if ( i == -1 )
    return;
  
  wxListItem item;
  item.SetId(i);
  item.SetColumn(0);
  item.SetText(wxEmptyString);
  engine_list->GetItem(item);
  wxString sac = item.GetText();
  config->Write(sac + wxT("/name"), text_ctrl_name->GetValue());
  item.SetColumn(1);
  item.SetText(text_ctrl_name->GetValue());
  engine_list->SetItem(item);
  engine_list->SetColumnWidth( 1, wxLIST_AUTOSIZE );
}

void EngineMgrDialog::apply(const long & i)
{
  if ( i == -1 )
    return;
  
  wxListItem item;
  item.SetId(i);
  item.SetColumn(0);
  item.SetText(wxEmptyString);
  engine_list->GetItem(item);
  wxString sac = item.GetText();
  config->Write(sac + wxT("/name"), text_ctrl_name->GetValue());
  item.SetColumn(1);
  item.SetText(text_ctrl_name->GetValue());
  engine_list->SetItem(item);
  config->Write(sac + wxT("/description"), text_ctrl_dscp->GetValue());
  item.SetColumn(3);
  item.SetText(text_ctrl_dscp->GetValue());
  engine_list->SetItem(item);
  engine_list->SetColumnWidth( 1, wxLIST_AUTOSIZE );
  engine_list->SetColumnWidth( 3, wxLIST_AUTOSIZE );
}

void EngineMgrDialog::OnApply(wxCommandEvent & event)
{
  long i = -1;
  i = engine_list->GetNextItem(i,
                                 wxLIST_NEXT_ALL,
                                 wxLIST_STATE_SELECTED);
  apply(i);
}

void EngineMgrDialog::OnDel(wxCommandEvent & event)
{
  long i = -1;
  i = engine_list->GetNextItem(i,
                                 wxLIST_NEXT_ALL,
                                 wxLIST_STATE_SELECTED);
  if ( i == -1)
  {
     ::wxMessageBox(_("Please select one engine first."), _("Info"), wxOK | wxICON_INFORMATION, this);
    return;
  }
  if ( ::wxMessageBox(_("Do you really want to delete it?"), _("Confirm"), wxYES_NO | wxICON_QUESTION, this) == wxNO )
    return;
  
  wxListItem item;
  item.SetId(i);
  item.SetColumn(0);
  item.SetText(wxEmptyString);
  engine_list->GetItem(item);
  wxString sac = item.GetText();
  wxString ca = wxGetApp().engine_dir + wxGetApp().Delimited + sac + wxT(".xml");
  
  if ( wxRemoveFile(ca) )
  {
    config->DeleteGroup(sac);
    engine_list->DeleteItem(i);
    text_ctrl_symbol->SetValue(wxEmptyString);
    text_ctrl_type->SetValue(wxEmptyString);
    text_ctrl_name->SetValue(wxEmptyString);
    text_ctrl_dscp->SetValue(wxEmptyString);  
  }
  else
  {
     ::wxMessageBox(_("Something is wrong! I can not delete it.\nPlease delete the directory by hand."), _("Error"), wxOK | wxICON_ERROR, this);
  }
}

void EngineMgrDialog::OnAdd(wxCommandEvent & event)
{
  wxArrayString aChoices;
  //aChoices.Add(wxT("piston_engine"));
  aChoices.Add(wxT("turbine_engine"));
  //aChoices.Add(wxT("rocket_engine"));
  //aChoices.Add(wxT("electric_engine"));
  //aChoices.Add(wxT("propeller"));
  //aChoices.Add(wxT("nozzle"));
  aChoices.Add(wxT("direct"));
  wxString choice = wxGetSingleChoice(_("Choose one kind of engines/thrusters"), _("Which kind of engines/thrusters do you want to create?"), aChoices, this);
  if (choice.IsEmpty())
    return;
  
  wxString symbol = ::wxGetTextFromUser(_("Please give me the symbol of new engine."), _("Input new engine symbol"), wxEmptyString, NULL);
  if (symbol.IsEmpty())
    return;
  
  wxString ca = wxGetApp().engine_dir + wxGetApp().Delimited + symbol + wxT(".xml");
  bool flag = false;

  if (choice == wxT("turbine_engine"))
  {
    flag = newTurbineEngine(ca);
  }
  else if (choice == wxT("direct"))
  {
    flag = newDirect(ca);
  }
  else
  {
    flag = false;
  }
  
  if ( flag )
  {
      wxString dscp(wxT("NO INFO"));
      config->Write(symbol + wxT("/type"), choice);
      config->Write(symbol + wxT("/name"), symbol);
      config->Write(symbol + wxT("/description"), dscp);
      
      long i = -1;
      wxListItem item;
      long no = engine_list->GetItemCount();
      item.SetId(no);
      item.SetColumn(0);
      const wxString &tmp = choice;
      if (tmp == wxT("piston_engine"))
      {
        item.SetImage(0);
      }
      else if (tmp == wxT("turbine_engine"))
      {
        item.SetImage(1);      
      }
      else if (tmp == wxT("rocket_engine"))
      {
        item.SetImage(2);      
      }
      else if (tmp == wxT("electric_engine"))
      {
        item.SetImage(3);      
      }
      else if (tmp == wxT("propeller"))
      {
        item.SetImage(4);      
      }
      else if (tmp == wxT("nozzle"))
      {
        item.SetImage(5);      
      }
      else if (tmp == wxT("direct"))
      {
        item.SetImage(6);      
      }
      else
      {
        item.SetImage(-1);      
      }
      item.SetText(symbol);
      i = engine_list->InsertItem(item);
      item.SetId(i);
      item.SetColumn(1);
      item.SetImage(-1);
      item.SetText(symbol);
      engine_list->SetItem(item);
      item.SetId(i);
      item.SetColumn(2);
      item.SetImage(-1);
      item.SetText(choice);
      engine_list->SetItem(item);
      item.SetId(i);
      item.SetColumn(3);
      item.SetImage(-1);
      item.SetText(dscp);
      engine_list->SetItem(item);

      engine_list->SetColumnWidth( 0, wxLIST_AUTOSIZE );
      engine_list->SetColumnWidth( 1, wxLIST_AUTOSIZE );
      engine_list->SetColumnWidth( 2, wxLIST_AUTOSIZE );
      engine_list->SetColumnWidth( 3, wxLIST_AUTOSIZE );

      engine_list->EnsureVisible(i);
      engine_list->SetItemState(i, (~engine_list->GetItemState(i, wxLIST_STATE_SELECTED) ) & wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);

      text_ctrl_symbol->SetValue(symbol);
      text_ctrl_name->SetValue(symbol);
      text_ctrl_dscp->SetValue(dscp);
      text_ctrl_type->SetValue(choice);
  }
  else
  {
      ::wxMessageBox(_("Something is wrong! I can not create the file. Please check it."), _("Error"), wxOK | wxICON_ERROR, this);
  }
}

void EngineMgrDialog::OnDup(wxCommandEvent & event)
{
  long i = -1;
  i = engine_list->GetNextItem(i,
                                 wxLIST_NEXT_ALL,
                                 wxLIST_STATE_SELECTED);
  if ( i == -1 )
  {
     ::wxMessageBox(_("Please select one engine first."), _("Info"), wxOK | wxICON_INFORMATION, this);
    return;
  }
  
  wxListItem item;
  item.SetId(i);
  item.SetColumn(0);
  item.SetText(wxEmptyString);
  engine_list->GetItem(item);
  wxString san = item.GetText();
  wxString type = text_ctrl_type->GetValue();
  wxString tmp = _("Copy from ") + san;
  wxString symbol = ::wxGetTextFromUser(_("Please give me the symbol of the clone."), tmp, wxEmptyString, NULL);
  if (!symbol.IsEmpty())
  {
    wxString ca = wxGetApp().engine_dir + wxGetApp().Delimited + symbol + wxT(".xml");
    wxString sa = wxGetApp().engine_dir + wxGetApp().Delimited + san + wxT(".xml");
    if (!wxCopyFile(sa, ca))
    {
      ::wxMessageBox(_("Something is wrong! I can not copy the file. Please check it."), _("Error"), wxOK | wxICON_ERROR, this);      
    }
    else
    {
      wxString dscp(wxT("NO INFO"));
      config->Write(symbol + wxT("/type"), type);
      config->Write(symbol + wxT("/name"), symbol);
      config->Write(symbol + wxT("/description"), dscp);
      
      long i = -1;
      wxListItem item;

      long no = engine_list->GetItemCount();
      item.SetId(no);
      item.SetColumn(0);
      const wxString &tmp = type;
      if (tmp == wxT("piston_engine"))
      {
        item.SetImage(0);
      }
      else if (tmp == wxT("turbine_engine"))
      {
        item.SetImage(1);      
      }
      else if (tmp == wxT("rocket_engine"))
      {
        item.SetImage(2);      
      }
      else if (tmp == wxT("electric_engine"))
      {
        item.SetImage(3);      
      }
      else if (tmp == wxT("propeller"))
      {
        item.SetImage(4);      
      }
      else if (tmp == wxT("nozzle"))
      {
        item.SetImage(5);      
      }
      else if (tmp == wxT("direct"))
      {
        item.SetImage(6);      
      }
      else
      {
        item.SetImage(-1);      
      }
      item.SetText(symbol);
      i = engine_list->InsertItem(item);
      item.SetId(i);
      item.SetColumn(1);
      item.SetImage(-1);
      item.SetText(symbol);
      engine_list->SetItem(item);
      item.SetId(i);
      item.SetColumn(2);
      item.SetImage(-1);
      item.SetText(type);
      engine_list->SetItem(item);
      item.SetId(i);
      item.SetColumn(3);
      item.SetImage(-1);
      item.SetText(dscp);
      engine_list->SetItem(item);

      engine_list->SetColumnWidth( 0, wxLIST_AUTOSIZE );
      engine_list->SetColumnWidth( 1, wxLIST_AUTOSIZE );
      engine_list->SetColumnWidth( 2, wxLIST_AUTOSIZE );
      engine_list->SetColumnWidth( 3, wxLIST_AUTOSIZE );

      engine_list->EnsureVisible(i);
      engine_list->SetItemState(i, (~engine_list->GetItemState(i, wxLIST_STATE_SELECTED) ) & wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);

      text_ctrl_symbol->SetValue(symbol);
      text_ctrl_type->SetValue(type);
      text_ctrl_name->SetValue(symbol);
      text_ctrl_dscp->SetValue(dscp);    
    }
  }
}

void EngineMgrDialog::OnDoubleClickList(wxListEvent & event)
{
  wxCommandEvent e(wxEVT_COMMAND_BUTTON_CLICKED, ENGINE_DETAIL);
  e.SetEventObject( this );
  GetEventHandler()->ProcessEvent(e);
}

void EngineMgrDialog::OnDetail(wxCommandEvent & event)
{
  long i = -1;
  i = engine_list->GetNextItem(i,
                                 wxLIST_NEXT_ALL,
                                 wxLIST_STATE_SELECTED);
  if ( i == -1 )
    return;
  
  wxListItem item;
  item.SetId(i);
  item.SetColumn(0);
  item.SetText(wxEmptyString);
  engine_list->GetItem(item);
  wxString sac = item.GetText();
  wxString ca = wxGetApp().engine_dir + wxGetApp().Delimited + sac + wxT(".xml");
  wxString type = text_ctrl_type->GetValue();
  if ( ::wxFileExists(ca))
  {
    EngineDialog * dlg = NULL;
    if (type == wxT("turbine_engine"))
    {
      dlg = new TurbineDialog(this, -1, _("Turbine Engine Setup"));
    }
    else if (type == wxT("direct"))
    {
      wxMessageBox(_("Do not need to setup this kind of thruster."), _("Info"), wxOK | wxICON_INFORMATION, this);
      return;
    }
    else
    {
      wxMessageBox(_("Can not setup this kind of engine/thruster in GUI yet."), _("Warning"), wxOK | wxICON_INFORMATION, this);
      return;    
    }
    
    if (!dlg->Load(ca))
    {
      ::wxMessageBox(_("Something is wrong! I can not load proper file(") + ca + wxT(")."), _("Warning"), wxOK | wxICON_INFORMATION, this);
    }
    else if (dlg->ShowModal() == wxID_OK)
    {
      if (!dlg->Save())
      {
        ::wxMessageBox(_("Something is wrong! I can not save proper file(") + ca + _("). Please check it."), _("Error"), wxOK | wxICON_ERROR, this);
      }
    }
    if (dlg)
    {
      dlg->Destroy();
    }
  }
  else
  {
      ::wxMessageBox(_("Something is wrong! I can not find proper file(") + ca + _("). Please check it."), _("Error"), wxOK | wxICON_ERROR, this);  
  }
}


