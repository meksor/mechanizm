#pragma once

#include "project.h"
#include "gui/sequence/sequence_timeline_widget.h"
#include "gui/sequence/sequence_waveform_widget.h"
#include "sequence.h"
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

namespace mechanizm {

class SequenceInfo : public QWidget {
  Q_OBJECT
public:
  SequenceInfo(QWidget *parent = nullptr);

  void onProjectChanged(mechanizm::Project *p);
  void onSequenceSelected(mechanizm::Sequence *);
  void onSourcesChanged(std::vector<mechanizm::Source *> sources);
  void onAudioSourceChanged(int index);
  void importAudioSource();

signals:
private:
  void refreshForm();
  void refreshAudioSources();
  mechanizm::Source *findAudioSource(mechanizm::id_t sourceId) const;

  mechanizm::Project *project = nullptr;
  mechanizm::Sequence *sequence = nullptr;

  QVBoxLayout *layout = nullptr;
  QFormLayout *flay;
  QLineEdit *nameEdit;
  QLineEdit *midiSourceEdit;
  QComboBox *audioSourceEdit;
  QPushButton *importAudioButton;
  mechanizm::SequenceTimelineWidget *timelineWidget;
  mechanizm::SequenceWaveformWidget *waveformWidget;
  std::vector<mechanizm::Source *> audioSources;
};

} // namespace mechanizm