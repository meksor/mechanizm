#include "gui/sequence/sequence_info.h"
#include "source.h"
#include <QFileDialog>
#include "sequence.h"
#include <QMessageBox>
#include <QSignalBlocker>

namespace mechanizm {

SequenceInfo::SequenceInfo(QWidget *parent) : QWidget(parent) {
  layout = new QVBoxLayout(this);
  flay = new QFormLayout();
  nameEdit = new QLineEdit();
  midiSourceEdit = new QLineEdit();
  midiSourceEdit->setReadOnly(true);
  audioSourceEdit = new QComboBox();
  importAudioButton = new QPushButton(tr("Import Audio..."));
  timelineWidget = new mechanizm::SequenceTimelineWidget();
  waveformWidget = new mechanizm::SequenceWaveformWidget();

  timelineWidget->setFixedHeight(110);
  waveformWidget->setFixedHeight(130);

  flay->addRow(tr("Name:"), nameEdit);
  flay->addRow(tr("MIDI Source:"), midiSourceEdit);
  flay->addRow(tr("Audio Source:"), audioSourceEdit);
  flay->addRow(QString(), importAudioButton);

  layout->addLayout(flay);
  layout->addWidget(timelineWidget);
  layout->addWidget(waveformWidget);
  layout->addStretch();

  connect(audioSourceEdit, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this, &SequenceInfo::onAudioSourceChanged);
  connect(importAudioButton, &QPushButton::clicked, this,
          &SequenceInfo::importAudioSource);

  refreshForm();
}

void SequenceInfo::onProjectChanged(mechanizm::Project *p) {
  project = p;
  audioSources.clear();
  if (project != nullptr) {
    connect(project, &Project::sourcesChanged, this,
            &SequenceInfo::onSourcesChanged, Qt::UniqueConnection);
    onSourcesChanged(project->sources);
  } else {
    refreshAudioSources();
  }
}

void SequenceInfo::onSequenceSelected(mechanizm::Sequence *s) {
  disconnect(nameEdit, &QLineEdit::textChanged, nullptr, nullptr);
  if (sequence != nullptr) {
    disconnect(sequence, &mechanizm::Sequence::updated, this, nullptr);
  }

  sequence = s;
  if (sequence != nullptr) {
    connect(nameEdit, &QLineEdit::textChanged, sequence,
            &mechanizm::Sequence::setName);
    connect(sequence, &mechanizm::Sequence::updated, this,
            &SequenceInfo::refreshForm, Qt::UniqueConnection);
  }

  refreshForm();
}

void SequenceInfo::onSourcesChanged(std::vector<mechanizm::Source *> sources) {
  audioSources.clear();
  for (auto *source : sources) {
    if (source != nullptr && source->type == mechanizm::Source::Type::AUDIO) {
      audioSources.push_back(source);
    }
  }
  refreshAudioSources();
  refreshForm();
}

mechanizm::Source *SequenceInfo::findAudioSource(
    mechanizm::id_t sourceId) const {
  auto it = std::find_if(audioSources.begin(), audioSources.end(),
                         [sourceId](mechanizm::Source *source) {
                           return source != nullptr && source->id == sourceId;
                         });
  return (it == audioSources.end()) ? nullptr : *it;
}

void SequenceInfo::refreshAudioSources() {
  const QSignalBlocker blocker(audioSourceEdit);

  audioSourceEdit->clear();
  audioSourceEdit->addItem(
      tr("None"),
      QVariant::fromValue(static_cast<qulonglong>(
          mechanizm::Sequence::invalidSourceId())));
  for (auto *source : audioSources) {
    audioSourceEdit->addItem(QString::fromStdString(source->name),
                             QVariant::fromValue(
                                 static_cast<qulonglong>(source->id)));
  }
}

void SequenceInfo::refreshForm() {
  const QSignalBlocker nameBlocker(nameEdit);
  const QSignalBlocker audioBlocker(audioSourceEdit);

  const bool hasSequence = sequence != nullptr;
  nameEdit->setEnabled(hasSequence);
  audioSourceEdit->setEnabled(hasSequence);
  importAudioButton->setEnabled(hasSequence && project != nullptr);

  if (!hasSequence) {
    nameEdit->clear();
    midiSourceEdit->clear();
    audioSourceEdit->setCurrentIndex(0);
    timelineWidget->setSequence(nullptr);
    waveformWidget->setSequence(nullptr);
    return;
  }

  nameEdit->setText(QString::fromStdString(sequence->name));
  midiSourceEdit->setText(sequence->source == nullptr
                              ? tr("Missing")
                              : QString::fromStdString(sequence->source->name));

  const mechanizm::id_t audioId = sequence->audioSource == nullptr
                                      ? sequence->audioSourceId
                                      : sequence->audioSource->id;
  const int selectedIndex = audioSourceEdit->findData(
      QVariant::fromValue(static_cast<qulonglong>(
        audioId == mechanizm::Sequence::invalidSourceId()
          ? mechanizm::Sequence::invalidSourceId()
          : audioId)));
  audioSourceEdit->setCurrentIndex(selectedIndex >= 0 ? selectedIndex : 0);

  timelineWidget->setSequence(sequence);
  waveformWidget->setSequence(sequence);
}

void SequenceInfo::onAudioSourceChanged(int index) {
  if (sequence == nullptr || index < 0) {
    return;
  }

  const mechanizm::id_t selectedSourceId =
      audioSourceEdit->itemData(index).toULongLong();
  if (selectedSourceId == mechanizm::Sequence::invalidSourceId()) {
    sequence->setAudioSource(nullptr);
  } else {
    sequence->setAudioSource(findAudioSource(selectedSourceId));
  }
  refreshForm();
}

void SequenceInfo::importAudioSource() {
  if (project == nullptr || sequence == nullptr) {
    return;
  }

  const QString filePath = QFileDialog::getOpenFileName(
      this, tr("Import Audio Source"), QString(),
      tr("Audio Files (*.wav *.mp3 *.ogg *.flac *.m4a *.aac);;All Files (*)"));
  if (filePath.isEmpty()) {
    return;
  }

  mechanizm::id_t id = mechanizm::Source::getNextId(project->sources);
  auto *source = new mechanizm::Source(id, filePath.toStdString());
  if (source->type != mechanizm::Source::Type::AUDIO) {
    QMessageBox::warning(this, tr("Import Audio Source"),
                         tr("The selected file could not be imported as audio."));
    delete source;
    return;
  }

  project->addSource(source);
  sequence->setAudioSource(source);
  refreshForm();
}

} // namespace mechanizm
