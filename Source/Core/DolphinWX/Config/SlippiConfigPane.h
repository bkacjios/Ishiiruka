// Copyright 2015 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <wx/arrstr.h>
#include <wx/panel.h>
#include <wx/textctrl.h>

enum TEXIDevices : int;

class wxButton;
class wxCheckBox;
class wxChoice;
class wxDirPickerCtrl;
class wxSpinCtrl;
class wxString;
class wxStaticText;
class wxTextCtrl;

class SlippiConfigPane final : public wxPanel
{
public:
	SlippiConfigPane(wxWindow* parent, wxWindowID id);

private:
	struct ItemFrequency
	{
		u8 value;
		wxString name;
	};
	std::vector<ItemFrequency> m_item_frequency;
	struct Items
	{
		u8 value;
		wxString name;
	};
	std::vector<ItemFrequency> m_item_list_1;
	std::vector<ItemFrequency> m_item_list_2;
	void InitializeGUI();
	void LoadGUIValues();
	void BindEvents();

	void OnSlippiItemFrequencyChanged(wxCommandEvent &event);
	void OnSlippiItemSelectionChanged(wxCommandEvent &event);

	void OnReplaySavingToggle(wxCommandEvent& event);
	void OnReplayMonthFoldersToggle(wxCommandEvent& event);
	void OnReplayDirChanged(wxCommandEvent& event);
	void OnDelayFramesChanged(wxCommandEvent &event);

	wxCheckBox* m_replay_enable_checkbox;
	wxDirPickerCtrl* m_replay_directory_picker;
	wxCheckBox* m_replay_month_folders_checkbox;
	wxStaticText* m_slippi_delay_frames_txt;
	wxSpinCtrl *m_slippi_delay_frames_ctrl;

	wxArrayString m_slippi_item_frequency_string;
	wxRadioBox *m_slippi_item_frequency;

	u8 m_slippi_item_frequency_none;
	u8 m_slippi_item_frequency_very_low;
	u8 m_slippi_item_frequency_low;
	u8 m_slippi_item_frequency_medium;
	u8 m_slippi_item_frequency_high;
	u8 m_slippi_item_frequency_very_high;

	wxCheckBox *m_slippi_items_food;
	wxCheckBox *m_slippi_items_maxim_tomato;
	wxCheckBox *m_slippi_items_heart_container;
	wxCheckBox *m_slippi_items_warp_star;
	wxCheckBox *m_slippi_items_ray_gun;
	wxCheckBox *m_slippi_items_super_scope;
	wxCheckBox *m_slippi_items_fire_flower;
	wxCheckBox *m_slippi_items_lips_stick;
	wxCheckBox *m_slippi_items_star_rod;
	wxCheckBox *m_slippi_items_beam_sword;
	wxCheckBox *m_slippi_items_homerun_bat;
	wxCheckBox *m_slippi_items_fan;
	wxCheckBox *m_slippi_items_hammer;
	wxCheckBox *m_slippi_items_green_shell;
	wxCheckBox *m_slippi_items_red_shell;
	wxCheckBox *m_slippi_items_flipper;

	wxCheckBox *m_slippi_items_freezie;
	wxCheckBox *m_slippi_items_mr_saturn;
	wxCheckBox *m_slippi_items_pokeball;
	wxCheckBox *m_slippi_items_bobomb;
	wxCheckBox *m_slippi_items_motionsensor_bomb;
	wxCheckBox *m_slippi_items_super_mushroom;
	wxCheckBox *m_slippi_items_poison_mushroom;
	wxCheckBox *m_slippi_items_starman;
	wxCheckBox *m_slippi_items_parasol;
	wxCheckBox *m_slippi_items_screw_attack;
	wxCheckBox *m_slippi_items_metal_box;
	wxCheckBox *m_slippi_items_bunny_hood;
	wxCheckBox *m_slippi_items_cloaking_device;
	wxCheckBox *m_slippi_items_barrel_cannon;
	wxCheckBox *m_slippi_items_party_ball;
};
