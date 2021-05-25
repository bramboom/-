#include "quabrowser.h"
#include "stylesheet.h"
#include <opcuaclient/UaBrowser.h>

namespace EleSyOpcUaClient
{

    QUaBrowser::QUaBrowser(QWidget *parent)
        : QTreeWidget(parent)
    {
        this->setStyleSheet(StyleSheet::GetBlueLightStyleWidget());
        setSelectionMode(QAbstractItemView::SingleSelection);
        setDragEnabled(true);
        setDragDropMode(QAbstractItemView::DragOnly);
    }

    void QUaBrowser::setConnection(UaConnection::Ptr conn)
    {
        connection = conn;
        if(!isConnected())
        {
            status = uaBad;
        }
        else if(browserInit())
        {
            clear();
            addTopLevelItems(readCurrentDir(NODEID_OBJECTS));
        }
    }

    QList<QTreeWidgetItem*> QUaBrowser::readCurrentDir(const std::string& nodeId, BrowseDirection direction, ReferenceCategory refCategory)
    {
        browser->changeBrowsePosition(nodeId);
        std::vector<std::string> tmp;
        BrowseResults result;
        status = browser->browse(direction, refCategory, tmp, result);
        QList<QTreeWidgetItem*> items;
        if(status == uaGood)
        {
            for(size_t i = 0; i < tmp.size() && i < result.size(); ++i)
            {
                auto newChild = new QTreeWidgetItem;
                newChild->setText(0, QString::fromStdString(tmp[i]));
                if(result[i].nodeClass == UaNodeClass::cVariable)
                {
                    newChild->setIcon(0, varIcon);
                }
                items.append(newChild);
            }
            UaBytes contPoint, contPointNext;
            do
            {
                status = browser->browseNext(contPoint, tmp, result, contPointNext);
                contPoint = contPointNext;
                for(size_t i = 0; i < tmp.size(); ++i)
                {
                    auto newChild = new QTreeWidgetItem;
                    newChild->setText(0, QString::fromStdString(tmp[i]));
                    if(result[i].nodeClass == UaNodeClass::cVariable)
                    {
                        newChild->setIcon(0, varIcon);
                    }
                    items.append(newChild);
                }
            } while(!tmp.empty());
            browser->releasePoint(contPoint);
            for(auto &x : items)
            {
                x->addChildren(readCurrentDir(x->text(0).toStdString()));
            }
        }
        return items;
    }

    bool QUaBrowser::isConnected() const
    {
        return connection->isConnected();
    }

    bool QUaBrowser::browserInit()
    {
        status = createBrowser(connection, browser);
        return status == uaGood;
    }
}
