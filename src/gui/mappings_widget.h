#pragma once

#include <QWidget>
#include <QKeyEvent>
#include <QListWidget>
#include <QHBoxLayout>

#include "mapping.h"

namespace mechanizm {

    class MappingsWidget : public QWidget {
        Q_OBJECT
    public:
        typedef std::unique_ptr<MappingsWidget> unique_ptr;

        explicit MappingsWidget(QWidget *parent = 0);
        virtual ~MappingsWidget() {};

        void setMappings(std::vector<mechanizm::Mapping::shared_ptr> c);
        mechanizm::Mapping::shared_ptr currentMapping();

    protected:
        virtual void keyPressEvent(QKeyEvent *event) override;

    signals:
        void mappingEditorRequested(mechanizm::Mapping::shared_ptr c);

    private:
        void updateList();
   
        std::vector<mechanizm::Mapping::shared_ptr> mappings;

        std::unique_ptr<QListWidget> list;
        std::unique_ptr<QHBoxLayout> hbox;
    };

}
