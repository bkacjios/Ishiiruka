// Copyright 2018 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <QWidget>

class QCheckBox;
class QLineEdit;
class QSpinBox;

class SlippiPane : public QWidget
{
  Q_OBJECT
public:
  explicit SlippiPane();

private:
  void BrowseReplay();

  void CreateWidgets();
  void ConnectWidgets();

  void LoadSettings();
  void SaveSettings();

  QCheckBox* m_save_slippi_replays;
  QCheckBox* m_save_monthly_subfolders;
  QLineEdit* m_slippi_replay_path;

  QSpinBox* m_slippi_delay_frames;
};
