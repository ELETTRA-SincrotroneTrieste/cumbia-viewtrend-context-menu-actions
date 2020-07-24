#ifndef CUMBIACOPYSOURCECTXMENUACTION_H
#define CUMBIACOPYSOURCECTXMENUACTION_H

#include <cucontextmenuactionsplugin_i.h>
#include <QAction>
#include <QList>
#include <QObject>

class CuViewTrendContextMenuActionPluginPrivate;

class CuViewTrendContextMenuActionPlugin : public QObject, public CuContextMenuActionsPlugin_I
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QGenericPluginFactoryInterface" FILE "cumbia-viewtrend-context-menu-actions.json")

public:
    explicit CuViewTrendContextMenuActionPlugin(QObject *parent = nullptr);
    ~CuViewTrendContextMenuActionPlugin();

    Q_INTERFACES(CuContextMenuActionsPlugin_I)

    // CuContextMenuActionsPlugin_I interface
public:
    void setup(QWidget *widget, const CuContext *cuctx);
    QList<QAction *> getActions() const;
    int order() const;

protected slots:
    void onActionTriggered();

private:
    CuViewTrendContextMenuActionPluginPrivate *d;
};






#endif // CUMBIACOPYSOURCECTXMENUACTION_H
