// Copyright 2018 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "DolphinQt/Settings/SlippiPane.h"

#include <QCheckBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSpinBox>

#include <utility>

#include "Common/CommonPaths.h"
#include "Common/Config/Config.h"
#include "Common/FileUtil.h"

#include "Core/Config/MainSettings.h"
#include "Core/ConfigManager.h"
#include "Core/Core.h"

#include "DolphinQt/Config/Mapping/MappingWindow.h"
#include "DolphinQt/QtUtils/ModalMessageBox.h"

enum
{
  SLOT_A_INDEX,
  SLOT_B_INDEX,
  SLOT_SP1_INDEX,
  SLOT_COUNT
};

SlippiPane::SlippiPane()
{
  CreateWidgets();
  LoadSettings();
  ConnectWidgets();
}

void SlippiPane::BrowseReplay()
{
  QString dir = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(
      this, tr("Select Replay Path"),
      QString::fromStdString(Config::Get(Config::MAIN_SLIPPI_REPLAYS_PATH))));
  if (!dir.isEmpty())
  {
    m_slippi_replay_path->setText(dir);
    Config::SetBase(Config::MAIN_SLIPPI_REPLAYS_PATH, dir.toStdString());
  }
}

void SlippiPane::CreateWidgets()
{
  QVBoxLayout* layout = new QVBoxLayout(this);

  // IPL Settings
  QGroupBox* replay_box = new QGroupBox(tr("Slippi Replay Settings"), this);
  QGridLayout* replay_layout = new QGridLayout(replay_box);
  replay_box->setLayout(replay_layout);

  m_save_slippi_replays = new QCheckBox(tr("Save Slippi Replays"), replay_box);
  m_save_slippi_replays->setToolTip(
      tr("Enable this to make Slippi automatically save .slp recordings of your games."));

  m_save_monthly_subfolders = new QCheckBox(tr("Save Replays to Monthly Subfolders"), replay_box);
  m_save_monthly_subfolders->setToolTip(
      tr("Enable this to save your replays into subfolders by month (YYYY-MM)."));

  m_slippi_replay_path = new QLineEdit(QString::fromStdString(File::GetUserPath(D_USER_IDX)));
  m_slippi_replay_path->setToolTip(tr("Choose where your Slippi replay files are saved."));

  connect(m_slippi_replay_path, &QLineEdit::editingFinished, [=] {
    Config::SetBase(Config::MAIN_SLIPPI_REPLAYS_PATH, m_slippi_replay_path->text().toStdString());
  });

  QPushButton* replay_path_open = new QPushButton(QStringLiteral("..."));
  connect(replay_path_open, &QPushButton::clicked, this, &SlippiPane::BrowseReplay);

  replay_layout->addWidget(m_save_slippi_replays, 0, 0, 1, 3);
  replay_layout->addWidget(m_save_monthly_subfolders, 1, 0, 1, 3);

  replay_layout->addWidget(new QLabel(tr("Replay folder:")), 2, 0);
  replay_layout->addWidget(m_slippi_replay_path, 2, 1);
  replay_layout->addWidget(replay_path_open, 2, 2);

  QGroupBox* online_box = new QGroupBox(tr("Slippi Online Settings"));
  QGridLayout* online_layout = new QGridLayout(online_box);
  online_box->setLayout(online_layout);

  m_slippi_delay_frames = new QSpinBox();
  m_slippi_delay_frames->setMinimum(1);
  m_slippi_delay_frames->setMaximum(9);
  m_slippi_delay_frames->setToolTip(tr("Leave this at 2 unless consistently playing on 120+ ping. "
                                       "Increasing this can cause unplayable input delay, and "
                                       "lowering it can cause visual artifacts/lag."));

  online_layout->addWidget(new QLabel(tr("Delay Frames:")), 0, 0);
  online_layout->addWidget(m_slippi_delay_frames, 0, 1);

  layout->addWidget(replay_box);
  layout->addWidget(online_box);

  layout->addStretch();

  setLayout(layout);
}

void SlippiPane::ConnectWidgets()
{
  // IPL Settings
  connect(m_save_slippi_replays, &QCheckBox::stateChanged, this, &SlippiPane::SaveSettings);
  connect(m_save_monthly_subfolders, &QCheckBox::stateChanged, this, &SlippiPane::SaveSettings);
  connect(m_slippi_delay_frames, qOverload<int>(&QSpinBox::valueChanged), this,
          &SlippiPane::SaveSettings);
}

void SlippiPane::LoadSettings()
{
  const SConfig& params = SConfig::GetInstance();

  // IPL Settings
  m_save_slippi_replays->setChecked(params.m_slippiSaveReplays);
  m_save_monthly_subfolders->setChecked(params.m_slippiReplayMonthFolders);
  m_slippi_delay_frames->setValue(params.m_slippiOnlineDelay);
  m_slippi_replay_path->setText(QString::fromStdString(params.m_strSlippiReplayDir));
}

void SlippiPane::SaveSettings()
{
  Config::ConfigChangeCallbackGuard config_guard;

  SConfig& params = SConfig::GetInstance();

  // IPL Settings
  params.m_slippiSaveReplays = m_save_slippi_replays->isChecked();
  Config::SetBaseOrCurrent(Config::MAIN_SLIPPI_SAVE_REPLAYS, m_save_slippi_replays->isChecked());
  params.m_slippiReplayMonthFolders = m_save_monthly_subfolders->isChecked();
  Config::SetBaseOrCurrent(Config::MAIN_SLIPPI_REPLAYS_MONTLY_SUBFOLDERS,
                           m_save_monthly_subfolders->isChecked());
  params.m_slippiOnlineDelay = m_slippi_delay_frames->value();
  Config::SetBaseOrCurrent(Config::MAIN_SLIPPI_DELAY_FRAMES, m_slippi_delay_frames->value());
  params.m_strSlippiReplayDir = m_slippi_replay_path->text().toStdString();
  Config::SetBaseOrCurrent(Config::MAIN_SLIPPI_REPLAYS_PATH,
                           m_slippi_replay_path->text().toStdString());

  LoadSettings();
}
