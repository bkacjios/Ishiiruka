// Copyright 2015 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "DolphinWX/Config/SlippiConfigPane.h"

#include <cassert>
#include <string>

#include <wx/button.h>
#include <wx/radiobox.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/filepicker.h>
#include <wx/gbsizer.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>

#include "Common/Common.h"
#include "Common/CommonPaths.h"
#include "Common/FileUtil.h"
#include "Core/ConfigManager.h"
#include "Core/Core.h"
#include "Core/HW/EXI.h"
#include "Core/HW/GCMemcard.h"
#include "Core/HW/GCPad.h"
#include "Core/NetPlayProto.h"
#include "DolphinWX/Config/ConfigMain.h"
#include "DolphinWX/Input/MicButtonConfigDiag.h"
#include "DolphinWX/WxEventUtils.h"
#include "DolphinWX/WxUtils.h"

SlippiConfigPane::SlippiConfigPane(wxWindow* parent, wxWindowID id) : wxPanel(parent, id)
{
	m_item_frequency = {
	    {(u8)0, _("Very Low")},
	    {(u8)1, _("Low")},
	    {(u8)2, _("Medium")},
	    {(u8)3, _("High")},
	    {(u8)4, _("Very High")},
	    {(u8)-1, _("None")},
	};
	InitializeGUI();
	LoadGUIValues();
	BindEvents();
}

void SlippiConfigPane::InitializeGUI()
{

#ifndef IS_PLAYBACK
	// Slippi settings
	m_replay_enable_checkbox = new wxCheckBox(this, wxID_ANY, _("Save Slippi Replays"));
	m_replay_enable_checkbox->SetToolTip(
		_("Enable this to make Slippi automatically save .slp recordings of your games."));

	m_replay_month_folders_checkbox =
		new wxCheckBox(this, wxID_ANY, _("Save Replays to Monthly Subfolders"));
	m_replay_month_folders_checkbox->SetToolTip(
		_("Enable this to save your replays into subfolders by month (YYYY-MM)."));

	m_replay_directory_picker = new wxDirPickerCtrl(this, wxID_ANY, wxEmptyString, 
		_("Slippi Replay Folder:"), wxDefaultPosition, wxDefaultSize, 
		wxDIRP_USE_TEXTCTRL | wxDIRP_SMALL);
	m_replay_directory_picker->SetToolTip(
		_("Choose where your Slippi replay files are saved."));

	// Online settings
	m_slippi_delay_frames_txt = new wxStaticText(this, wxID_ANY, _("Delay Frames:"));
	m_slippi_delay_frames_ctrl = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1));
	m_slippi_delay_frames_ctrl->SetToolTip(_(
		"Leave this at 2 unless consistently playing on 120+ ping. "
		"Increasing this can cause unplayable input delay, and lowering it can cause visual artifacts/lag."));
	m_slippi_delay_frames_ctrl->SetRange(1, 9);

	for (const ItemFrequency &item_freq : m_item_frequency)
		m_slippi_item_frequency_string.Add(item_freq.name);

	m_slippi_item_frequency = new wxRadioBox(this, wxID_ANY, _("Item Frequency"), wxDefaultPosition, wxDefaultSize,
	                                         m_slippi_item_frequency_string, 0, wxRA_SPECIFY_COLS);

	m_slippi_items_food = new wxCheckBox(this, wxID_ANY, _("Food"));
	m_slippi_items_maxim_tomato = new wxCheckBox(this, wxID_ANY, _("Maxim Tomato"));
	m_slippi_items_heart_container = new wxCheckBox(this, wxID_ANY, _("Heart Container"));
	m_slippi_items_warp_star = new wxCheckBox(this, wxID_ANY, _("Warp Star"));
	m_slippi_items_ray_gun = new wxCheckBox(this, wxID_ANY, _("Ray Gun"));
	m_slippi_items_super_scope = new wxCheckBox(this, wxID_ANY, _("Super Scope"));
	m_slippi_items_fire_flower = new wxCheckBox(this, wxID_ANY, _("Fire Flower"));
	m_slippi_items_lips_stick = new wxCheckBox(this, wxID_ANY, _("Lip's Stick"));
	m_slippi_items_star_rod = new wxCheckBox(this, wxID_ANY, _("Star Rod"));
	m_slippi_items_beam_sword = new wxCheckBox(this, wxID_ANY, _("Beam Sword"));
	m_slippi_items_homerun_bat = new wxCheckBox(this, wxID_ANY, _("Home-Run Bat"));
	m_slippi_items_fan = new wxCheckBox(this, wxID_ANY, _("Fan"));
	m_slippi_items_hammer = new wxCheckBox(this, wxID_ANY, _("Hammer"));
	m_slippi_items_green_shell = new wxCheckBox(this, wxID_ANY, _("Green Shell"));
	m_slippi_items_red_shell = new wxCheckBox(this, wxID_ANY, _("Red Shell"));
	m_slippi_items_flipper = new wxCheckBox(this, wxID_ANY, _("Flipper"));

	m_slippi_items_freezie = new wxCheckBox(this, wxID_ANY, _("Freezie"));
	m_slippi_items_mr_saturn = new wxCheckBox(this, wxID_ANY, _("Mr. Saturn"));
	m_slippi_items_pokeball = new wxCheckBox(this, wxID_ANY, _("Poké Ball"));
	m_slippi_items_bobomb = new wxCheckBox(this, wxID_ANY, _("Bob-omb"));
	m_slippi_items_motionsensor_bomb = new wxCheckBox(this, wxID_ANY, _("Motion-Sensor Bomb"));
	m_slippi_items_super_mushroom = new wxCheckBox(this, wxID_ANY, _("Super Mushroom"));
	m_slippi_items_poison_mushroom = new wxCheckBox(this, wxID_ANY, _("Poison Mushroom"));
	m_slippi_items_starman = new wxCheckBox(this, wxID_ANY, _("Starman"));
	m_slippi_items_parasol = new wxCheckBox(this, wxID_ANY, _("Parasol"));
	m_slippi_items_screw_attack = new wxCheckBox(this, wxID_ANY, _("Screw Attack"));
	m_slippi_items_metal_box = new wxCheckBox(this, wxID_ANY, _("Metal Box"));
	m_slippi_items_bunny_hood = new wxCheckBox(this, wxID_ANY, _("Bunny Hood"));
	m_slippi_items_cloaking_device = new wxCheckBox(this, wxID_ANY, _("Cloaking Device"));
	m_slippi_items_barrel_cannon = new wxCheckBox(this, wxID_ANY, _("Barrel Cannon"));
	m_slippi_items_party_ball = new wxCheckBox(this, wxID_ANY, _("Party Ball"));
	
#endif
	const int space5 = FromDIP(5);
	const int space10 = FromDIP(10);

#ifndef IS_PLAYBACK
	wxGridBagSizer* const sSlippiReplaySettings = new wxGridBagSizer(space5, space5);
	sSlippiReplaySettings->Add(m_replay_enable_checkbox, wxGBPosition(0, 0), wxGBSpan(1, 2));
	sSlippiReplaySettings->Add(m_replay_month_folders_checkbox, wxGBPosition(1, 0), wxGBSpan(1, 2),
		wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	sSlippiReplaySettings->Add(new wxStaticText(this, wxID_ANY, _("Replay folder:")), wxGBPosition(2, 0),
		wxDefaultSpan, wxALIGN_CENTER_VERTICAL);
	sSlippiReplaySettings->Add(m_replay_directory_picker, wxGBPosition(2, 1), wxDefaultSpan, wxEXPAND);
	sSlippiReplaySettings->AddGrowableCol(1);

	wxStaticBoxSizer* const sbSlippiReplaySettings =
		new wxStaticBoxSizer(wxVERTICAL, this, _("Slippi Replay Settings"));
	sbSlippiReplaySettings->AddSpacer(space5);
	sbSlippiReplaySettings->Add(sSlippiReplaySettings, 0, wxEXPAND | wxLEFT | wxRIGHT, space5);
	sbSlippiReplaySettings->AddSpacer(space5);

	wxGridBagSizer* const sSlippiOnlineSettings = new wxGridBagSizer(space5, space5);
	sSlippiOnlineSettings->Add(m_slippi_delay_frames_txt, wxGBPosition(0, 0), wxDefaultSpan, wxALIGN_CENTER_VERTICAL);
	sSlippiOnlineSettings->Add(m_slippi_delay_frames_ctrl, wxGBPosition(0, 1), wxDefaultSpan, wxALIGN_LEFT);

	wxStaticBoxSizer *const sbSlippiOnlineSettings =
	    new wxStaticBoxSizer(wxVERTICAL, this, _("Slippi Online Settings"));
	sbSlippiOnlineSettings->AddSpacer(space5);
	sbSlippiOnlineSettings->Add(sSlippiOnlineSettings, 0, wxEXPAND | wxLEFT | wxRIGHT, space5);
	sbSlippiOnlineSettings->AddSpacer(space5);

	wxGridBagSizer *const sSlippiItemSelector = new wxGridBagSizer(space5, space5);
	sSlippiItemSelector->Add(m_slippi_items_food, wxGBPosition(0, 0));
	sSlippiItemSelector->Add(m_slippi_items_maxim_tomato, wxGBPosition(1, 0));
	sSlippiItemSelector->Add(m_slippi_items_heart_container, wxGBPosition(2, 0));
	sSlippiItemSelector->Add(m_slippi_items_warp_star, wxGBPosition(3, 0));
	sSlippiItemSelector->Add(m_slippi_items_ray_gun, wxGBPosition(4, 0));
	sSlippiItemSelector->Add(m_slippi_items_super_scope, wxGBPosition(5, 0));
	sSlippiItemSelector->Add(m_slippi_items_fire_flower, wxGBPosition(6, 0));
	sSlippiItemSelector->Add(m_slippi_items_lips_stick, wxGBPosition(7, 0));
	sSlippiItemSelector->Add(m_slippi_items_star_rod, wxGBPosition(8, 0));
	sSlippiItemSelector->Add(m_slippi_items_beam_sword, wxGBPosition(9, 0));
	sSlippiItemSelector->Add(m_slippi_items_homerun_bat, wxGBPosition(10, 0));
	sSlippiItemSelector->Add(m_slippi_items_fan, wxGBPosition(11, 0));
	sSlippiItemSelector->Add(m_slippi_items_hammer, wxGBPosition(12, 0));
	sSlippiItemSelector->Add(m_slippi_items_green_shell, wxGBPosition(13, 0));
	sSlippiItemSelector->Add(m_slippi_items_red_shell, wxGBPosition(14, 0));
	sSlippiItemSelector->Add(m_slippi_items_flipper, wxGBPosition(15, 0));

	sSlippiItemSelector->Add(m_slippi_items_freezie, wxGBPosition(0, 1));
	sSlippiItemSelector->Add(m_slippi_items_mr_saturn, wxGBPosition(1, 1));
	sSlippiItemSelector->Add(m_slippi_items_pokeball, wxGBPosition(2, 1));
	sSlippiItemSelector->Add(m_slippi_items_bobomb, wxGBPosition(3, 1));
	sSlippiItemSelector->Add(m_slippi_items_motionsensor_bomb, wxGBPosition(4, 1));
	sSlippiItemSelector->Add(m_slippi_items_super_mushroom, wxGBPosition(5, 1));
	sSlippiItemSelector->Add(m_slippi_items_poison_mushroom, wxGBPosition(6, 1));
	sSlippiItemSelector->Add(m_slippi_items_starman, wxGBPosition(7, 1));
	sSlippiItemSelector->Add(m_slippi_items_parasol, wxGBPosition(8, 1));
	sSlippiItemSelector->Add(m_slippi_items_screw_attack, wxGBPosition(9, 1));
	sSlippiItemSelector->Add(m_slippi_items_metal_box, wxGBPosition(10, 1));
	sSlippiItemSelector->Add(m_slippi_items_bunny_hood, wxGBPosition(11, 1));
	sSlippiItemSelector->Add(m_slippi_items_cloaking_device, wxGBPosition(12, 1));
	sSlippiItemSelector->Add(m_slippi_items_barrel_cannon, wxGBPosition(13, 1));
	sSlippiItemSelector->Add(m_slippi_items_party_ball, wxGBPosition(14, 1));

	wxStaticBoxSizer *const sbSlippiItemSettings =
	    new wxStaticBoxSizer(wxVERTICAL, this, _("Item Settings"));
	sbSlippiItemSettings->AddSpacer(space5);
	sbSlippiItemSettings->Add(m_slippi_item_frequency, 0, wxEXPAND | wxLEFT | wxRIGHT, space5);
	sbSlippiItemSettings->AddSpacer(space5);
	sbSlippiItemSettings->Add(sSlippiItemSelector, 0, wxEXPAND | wxLEFT | wxRIGHT, space5);
	sbSlippiItemSettings->AddSpacer(space5);
#endif
	wxBoxSizer* const main_sizer = new wxBoxSizer(wxVERTICAL);
#ifndef IS_PLAYBACK
	main_sizer->AddSpacer(space5);
	main_sizer->Add(sbSlippiReplaySettings, 0, wxEXPAND | wxLEFT | wxRIGHT, space5);
	main_sizer->AddSpacer(space5);
	main_sizer->Add(sbSlippiOnlineSettings, 0, wxEXPAND | wxLEFT | wxRIGHT, space5);
	main_sizer->AddSpacer(space5);
	main_sizer->Add(sbSlippiItemSettings, 0, wxEXPAND | wxLEFT | wxRIGHT, space5);
	main_sizer->AddSpacer(space5);
#endif
	SetSizer(main_sizer);
}

void SlippiConfigPane::LoadGUIValues()
{
	const SConfig& startup_params = SConfig::GetInstance();

#if HAVE_PORTAUDIO
#endif


#ifndef IS_PLAYBACK
	bool enableReplays = startup_params.m_slippiSaveReplays;

	m_replay_enable_checkbox->SetValue(enableReplays);
	m_replay_month_folders_checkbox->SetValue(startup_params.m_slippiReplayMonthFolders);
	m_replay_directory_picker->SetPath(StrToWxStr(startup_params.m_strSlippiReplayDir));

	if (!enableReplays) {
		m_replay_month_folders_checkbox->Hide();
	}

	m_slippi_delay_frames_ctrl->SetValue(startup_params.m_slippiOnlineDelay);

	for (size_t i = 0; i < m_item_frequency.size(); ++i)
	{
		if (m_item_frequency[i].value == startup_params.m_slippiItemFrequency)
			m_slippi_item_frequency->SetSelection(i);
	}

	u32 selection1 = startup_params.m_slippiItemSelection1;
	u32 selection2 = startup_params.m_slippiItemSelection2;

	m_slippi_items_party_ball->SetValue(selection2 & 0x10);
	m_slippi_items_barrel_cannon->SetValue(selection2 & 0x20);
	m_slippi_items_bobomb->SetValue(selection2 & 0x40);
	m_slippi_items_mr_saturn->SetValue(selection2 & 0x80);
	m_slippi_items_heart_container->SetValue(selection2 & 0x100);
	m_slippi_items_maxim_tomato->SetValue(selection2 & 0x200);
	m_slippi_items_starman->SetValue(selection2 & 0x400);
	m_slippi_items_homerun_bat->SetValue(selection2 & 0x800);
	m_slippi_items_beam_sword->SetValue(selection2 & 0x1000);
	m_slippi_items_parasol->SetValue(selection2 & 0x2000);
	m_slippi_items_green_shell->SetValue(selection2 & 0x4000);
	m_slippi_items_red_shell->SetValue(selection2 & 0x8000);
	m_slippi_items_ray_gun->SetValue(selection2 & 0x10000);
	m_slippi_items_freezie->SetValue(selection2 & 0x20000);
	m_slippi_items_food->SetValue(selection2 & 0x40000);
	m_slippi_items_motionsensor_bomb->SetValue(selection2 & 0x80000);
	m_slippi_items_flipper->SetValue(selection2 & 0x100000);
	m_slippi_items_super_scope->SetValue(selection2 & 0x200000);
	m_slippi_items_star_rod->SetValue(selection2 & 0x400000);
	m_slippi_items_lips_stick->SetValue(selection2 & 0x800000);
	m_slippi_items_fan->SetValue(selection2 & 0x1000000);
	m_slippi_items_fire_flower->SetValue(selection2 & 0x2000000);
	m_slippi_items_super_mushroom->SetValue(selection2 & 0x4000000);
	m_slippi_items_poison_mushroom->SetValue(selection2 & 0x8000000);
	m_slippi_items_hammer->SetValue(selection2 & 0x10000000);
	m_slippi_items_warp_star->SetValue(selection2 & 0x20000000);
	m_slippi_items_screw_attack->SetValue(selection2 & 0x40000000);
	m_slippi_items_bunny_hood->SetValue(selection2 & 0x80000000);

	m_slippi_items_metal_box->SetValue(selection1 & 0x1);
	m_slippi_items_cloaking_device->SetValue(selection1 & 0x2);
	m_slippi_items_pokeball->SetValue(selection1 & 0x4);
#endif
}

void SlippiConfigPane::BindEvents()
{
#ifndef IS_PLAYBACK
	m_replay_enable_checkbox->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnReplaySavingToggle, this);

	m_replay_month_folders_checkbox->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnReplayMonthFoldersToggle,
		this);

	m_replay_directory_picker->Bind(wxEVT_DIRPICKER_CHANGED, &SlippiConfigPane::OnReplayDirChanged, this);

	m_slippi_delay_frames_ctrl->Bind(wxEVT_SPINCTRL, &SlippiConfigPane::OnDelayFramesChanged, this);

	m_slippi_item_frequency->Bind(wxEVT_RADIOBOX, &SlippiConfigPane::OnSlippiItemFrequencyChanged, this);

	m_slippi_items_food->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_maxim_tomato->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_heart_container->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_warp_star->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_ray_gun->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_super_scope->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_fire_flower->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_lips_stick->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_star_rod->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_beam_sword->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_homerun_bat->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_fan->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_hammer->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_green_shell->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_red_shell->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_flipper->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);

	m_slippi_items_freezie->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_mr_saturn->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_pokeball->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_bobomb->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_motionsensor_bomb->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_super_mushroom->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_poison_mushroom->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_starman->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_parasol->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_screw_attack->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_metal_box->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_bunny_hood->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_cloaking_device->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_barrel_cannon->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);
	m_slippi_items_party_ball->Bind(wxEVT_CHECKBOX, &SlippiConfigPane::OnSlippiItemSelectionChanged, this);

#endif
}

void SlippiConfigPane::OnSlippiItemFrequencyChanged(wxCommandEvent &event) {
	SConfig::GetInstance().m_slippiItemFrequency = m_item_frequency.at(event.GetSelection()).value;
}

void SlippiConfigPane::OnSlippiItemSelectionChanged(wxCommandEvent &event)
{
	u32 selection1 = 0;
	u32 selection2 = 0;

	if (m_slippi_items_party_ball->IsChecked())
		selection2 |= 0x10;

	if (m_slippi_items_barrel_cannon->IsChecked())
		selection2 |= 0x20;

	if (m_slippi_items_bobomb->IsChecked())
		selection2 |= 0x40;

	if (m_slippi_items_mr_saturn->IsChecked())
		selection2 |= 0x80;

	if (m_slippi_items_heart_container->IsChecked())
		selection2 |= 0x100;

	if (m_slippi_items_maxim_tomato->IsChecked())
		selection2 |= 0x200;

	if (m_slippi_items_starman->IsChecked())
		selection2 |= 0x400;

	if (m_slippi_items_homerun_bat->IsChecked())
		selection2 |= 0x800;

	if (m_slippi_items_beam_sword->IsChecked())
		selection2 |= 0x1000;

	if (m_slippi_items_parasol->IsChecked())
		selection2 |= 0x2000;

	if (m_slippi_items_green_shell->IsChecked())
		selection2 |= 0x4000;

	if (m_slippi_items_red_shell->IsChecked())
		selection2 |= 0x8000;

	if (m_slippi_items_ray_gun->IsChecked())
		selection2 |= 0x10000;

	if (m_slippi_items_freezie->IsChecked())
		selection2 |= 0x20000;

	if (m_slippi_items_food->IsChecked())
		selection2 |= 0x40000;

	if (m_slippi_items_motionsensor_bomb->IsChecked())
		selection2 |= 0x80000;

	if (m_slippi_items_flipper->IsChecked())
		selection2 |= 0x100000;

	if (m_slippi_items_super_scope->IsChecked())
		selection2 |= 0x200000;

	if (m_slippi_items_star_rod->IsChecked())
		selection2 |= 0x400000;

	if (m_slippi_items_lips_stick->IsChecked())
		selection2 |= 0x800000;

	if (m_slippi_items_fan->IsChecked())
		selection2 |= 0x1000000;

	if (m_slippi_items_fire_flower->IsChecked())
		selection2 |= 0x2000000;

	if (m_slippi_items_super_mushroom->IsChecked())
		selection2 |= 0x4000000;

	if (m_slippi_items_poison_mushroom->IsChecked())
		selection2 |= 0x8000000;

	if (m_slippi_items_hammer->IsChecked())
		selection2 |= 0x10000000;

	if (m_slippi_items_warp_star->IsChecked())
		selection2 |= 0x20000000;

	if (m_slippi_items_screw_attack->IsChecked())
		selection2 |= 0x40000000;

	if (m_slippi_items_bunny_hood->IsChecked())
		selection2 |= 0x80000000;

	if (m_slippi_items_metal_box->IsChecked())
		selection1 |= 0x1;

	if (m_slippi_items_cloaking_device->IsChecked())
		selection1 |= 0x2;

	if (m_slippi_items_pokeball->IsChecked())
		selection1 |= 0x4;

	SConfig::GetInstance().m_slippiItemSelection1 = selection1;
	SConfig::GetInstance().m_slippiItemSelection2 = selection2;

	// (0x00000000 00000010): Party Ball
	// (0x00000000 00000020): Barrel Cannon
	// (0x00000000 00000040): Bob-omb
	// (0x00000000 00000080): Mr. Saturn
	// (0x00000000 00000100): Heart Container
	// (0x00000000 00000200): Maxim Tomato
	// (0x00000000 00000400): Starman
	// (0x00000000 00000800): Home-Run Bat
	// (0x00000000 00001000): Beam Sword
	// (0x00000000 00002000): Parasol
	// (0x00000000 00004000): Green Shell
	// (0x00000000 00008000): Red Shell
	// (0x00000000 00010000): Ray Gun
	// (0x00000000 00020000): Freezie
	// (0x00000000 00040000): Food
	// (0x00000000 00080000): Motion-Sensor Bomb
	// (0x00000000 00100000): Flipper
	// (0x00000000 00200000): Super Scope
	// (0x00000000 00400000): Star Rod
	// (0x00000000 00800000): Lips's Stick
	// (0x00000000 01000000): Fan
	// (0x00000000 02000000): Fire Flower
	// (0x00000000 04000000): Super Mushroom
	// (0x00000000 08000000): Poison Mushroom
	// (0x00000000 10000000): Hammer
	// (0x00000000 20000000): Warp Star
	// (0x00000000 40000000): Screw Attack
	// (0x00000000 80000000): Bunny Hood
	// (0x00000001 00000000): Metal Box
	// (0x00000002 00000000): Cloaking Device
	// (0x00000004 00000000): Pokeball
}

void SlippiConfigPane::OnReplaySavingToggle(wxCommandEvent& event)
{
	bool enableReplays = m_replay_enable_checkbox->IsChecked();

	SConfig::GetInstance().m_slippiSaveReplays = enableReplays;

	if (enableReplays) {
		m_replay_month_folders_checkbox->Show();
	} else {
		m_replay_month_folders_checkbox->SetValue(false);
		m_replay_month_folders_checkbox->Hide();
		SConfig::GetInstance().m_slippiReplayMonthFolders = false;
	}
}

void SlippiConfigPane::OnReplayMonthFoldersToggle(wxCommandEvent& event)
{
	SConfig::GetInstance().m_slippiReplayMonthFolders =
		m_replay_enable_checkbox->IsChecked() &&
		m_replay_month_folders_checkbox->IsChecked();
}

void SlippiConfigPane::OnReplayDirChanged(wxCommandEvent& event)
{
	SConfig::GetInstance().m_strSlippiReplayDir =
		WxStrToStr(m_replay_directory_picker->GetPath());
}

void SlippiConfigPane::OnDelayFramesChanged(wxCommandEvent &event)
{
	SConfig::GetInstance().m_slippiOnlineDelay = m_slippi_delay_frames_ctrl->GetValue();
}
