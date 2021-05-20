#pragma once
#include <QTreeWidget>
#include <opcuaclient/UaBrowser.h>

#include "util.h"

namespace EleSyOpcUaClient
{

    class QUaBrowser : public QTreeWidget
    {
    Q_OBJECT
    public:
        using Ptr = std::shared_ptr<QUaBrowser>;
        QUaBrowser(QWidget *parent = nullptr);

        void setConnection(UaConnection::Ptr conn);

        QList<QTreeWidgetItem*> readCurrentDir(const std::string &nodeId,
                                               BrowseDirection direction = BrowseDirection::forward,
                                               ReferenceCategory refCategory = ReferenceCategory::hierarchical);

        UaStatusCode lastError() const { return status; }
        bool isValid() const { return status == uaGood; }
        inline bool isConnected() const;
        void checkConnected();
        bool browserInit();

    private:
        UaConnection::Ptr connection;
        UaBrowser::Ptr    browser;
        UaStatusCode      status;
        QIcon             varIcon { QPixmap("../data.png") };
    signals:
        void directoryChanged(const QString &dirname);

     };

}

