/*
 *   Copyright (C) 2007 Matt Broadstone <mbroadst@gmail.com>
 *   Copyright (C) 2007 Aaron Seigo <aseigo@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License version 2 as
 *   published by the Free Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "corona.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QProcess>

#include <KLocale>
#include <KMenu>
#include <KWindowSystem>

#include "applet.h"
#include "dataengine.h"
#include "widgets/vboxlayout.h"

#ifdef ICON_DEMO
#include "widgets/icon.h"
#endif

#include "corona.h"

using namespace Plasma;
extern "C" {
    typedef QGraphicsItem* (*loadKaramba)(const KUrl &theme, QGraphicsView *view);
}

namespace Plasma
{

class Corona::Private
{
public:
    Private()
        : formFactor(Planar),
          location(Floating),
          layout(0),
          engineExplorerAction(0)
    {
    }

    Applet::List applets;
    FormFactor formFactor;
    Location location;
    Layout* layout;
    QAction *engineExplorerAction;
};

Corona::Corona(QObject * parent)
    : QGraphicsScene(parent),
      d(new Private)
{
    init();
}

Corona::Corona(const QRectF & sceneRect, QObject * parent )
    : QGraphicsScene(sceneRect, parent),
      d(new Private)
{
    init();
}

Corona::Corona(qreal x, qreal y, qreal width, qreal height, QObject * parent)
    : QGraphicsScene(x, y, width, height, parent),
      d(new Private)
{
    init();
}

void Corona::init()
{
/*    setBackgroundMode(Qt::NoBackground);*/

/*    QPalette pal = palette();
    pal.setBrush(QPalette::Base, Qt::transparent);
    setPalette(pal);*/
    //setViewport(new QGLWidget  ( QGLFormat(QGL::StencilBuffer | QGL::AlphaChannel)   ));

#ifdef ICON_DEMO
    Icon* icon = new Icon();
    icon->setIcon("plasmagik");
    icon->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    addItem(icon);

    icon = new Icon();
    icon->setIcon("user-home");
    icon->setSize(64, 64);
    icon->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    addItem(icon);
#endif
    /*
    KPluginInfo::List applets = Applet::knownApplets();
    kDebug() << "======= Applets =========" << endl;
    foreach (KPluginInfo* info, applets) {
        kDebug() << info->pluginName() << ": " << info->name() << endl;
    }
    kDebug() << "=========================" << endl;
    */

    // Loading SuperKaramba themes example
    /*
    QString karambaLib = QFile::encodeName(KLibLoader::self()->findLibrary(QLatin1String("libsuperkaramba")));
    KLibLoader* loader = KLibLoader::self();
    KLibrary* lib = loader->library(karambaLib);

    if (lib) {
        loadKaramba startKaramba = 0;
        startKaramba = (loadKaramba)lib->resolveFunction("startKaramba");
        if (startKaramba) {
            startKaramba(KUrl("~/themes/aero_aio.skz"), this);
        }

        lib->unload();
    }
    */

//    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(displayContextMenu(QPoint)));
    d->engineExplorerAction = new QAction(i18n("Engine Explorer"), this);
    connect(d->engineExplorerAction, SIGNAL(triggered(bool)), this, SLOT(launchExplorer(bool)));
//    setContextMenuPolicy(Qt::CustomContextMenu);
}

Corona::~Corona()
{
    while (!d->applets.isEmpty()) {
        delete d->applets.takeFirst();
    }
}

Location Corona::location() const
{
    return d->location;
}

void Corona::setLocation(Location location)
{
    d->location = location;
}

FormFactor Corona::formFactor() const
{
    return d->formFactor;
}

void Corona::setFormFactor(FormFactor formFactor)
{
    if (d->formFactor == formFactor) {
        return;
    }

    d->formFactor = formFactor;
    delete d->layout;
    d->layout = 0;

    switch (d->formFactor) {
        case Planar:
            break;
        case Horizontal:
            //d->layout = new HBoxLayout;
            break;
        case Vertical:
            d->layout = new VBoxLayout;
            break;
        case MediaCenter:
            break;
        default:
            kDebug() << "This can't be happening!" << endl;
            break;
    }

    foreach (Applet* applet, d->applets) {
        applet->constraintsUpdated();
    }
}


void Corona::addPlasmoid(const QString& name)
{
    Applet* applet = Applet::loadApplet(name);
    if (applet) {
        addItem(applet);
        d->applets << applet;
    } else {
        kDebug() << "Plasmoid " << name << " could not be loaded." << endl;
    }
}

void Corona::dragEnterEvent( QGraphicsSceneDragDropEvent *event)
{
    kDebug() << "Corona::dragEnterEvent(QGraphicsSceneDragDropEvent* event)" << endl;
    if (event->mimeData()->hasFormat("text/x-plasmoidservicename")) {
        event->acceptProposedAction();
        //TODO Create the applet, move to mouse position then send the 
        //     following event to lock it to the mouse
        //QMouseEvent event(QEvent::MouseButtonPress, event->pos(), Qt::LeftButton, event->mouseButtons(), 0);
        //QApplication::sendEvent(this, &event);
    }
    //TODO Allow dragging an applet from another Corona into this one while
    //     keeping its settings etc.
}

void Corona::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    Q_UNUSED(event);
    kDebug() << "Corona::dragLeaveEvent(QGraphicsSceneDragDropEvent* event)" << endl;
    //TODO If an established Applet is dragged out of the Corona, remove it and
    //     create a QDrag type thing to keep the Applet's settings
}

void Corona::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    Q_UNUSED(event);
    kDebug() << "Corona::dragMoveEvent(QDragMoveEvent* event)" << endl;
}

void Corona::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    kDebug() << "Corona::dropEvent(QDropEvent* event)" << endl;
    if (event->mimeData()->hasFormat("text/x-plasmoidservicename")) {
        //TODO This will pretty much move into dragEnterEvent()
        QString plasmoidName;
        plasmoidName = event->mimeData()->data("text/x-plasmoidservicename");
        addPlasmoid(plasmoidName);
        d->applets.last()->setPos(event->pos());

        event->acceptProposedAction();
    }
}

void Corona::contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent)
{
    QPointF point = contextMenuEvent->scenePos();
    /*
     * example for displaying the SuperKaramba context menu
    QGraphicsItem *item = itemAt(point);
    if(item) {
        QObject *object = dynamic_cast<QObject*>(item->parentItem());
        if(object && object->objectName().startsWith("karamba")) {
            QContextMenuEvent event(QContextMenuEvent::Mouse, point);
            contextMenuEvent(&event);
            return;
        }
    }
    */

    Applet* applet = qgraphicsitem_cast<Applet*>(itemAt(point));
    KMenu desktopMenu;
    if(!applet) {
        desktopMenu.setTitle("Corona");
        desktopMenu.addAction("The");
        desktopMenu.addAction("desktop");
        desktopMenu.addAction("menu");
        desktopMenu.addAction(d->engineExplorerAction);
    } else {
        //desktopMenu.setTitle( applet->name() ); //This isn't implemented in Applet yet...
        desktopMenu.addAction("Widget");
        desktopMenu.addAction("settings");
        desktopMenu.addAction("like");
        desktopMenu.addAction("opacity");
        desktopMenu.addSeparator();
        QAction* configureApplet = new QAction(i18n("Configure Applet..."), this);
        connect(configureApplet, SIGNAL(triggered(bool)),
                applet, SLOT(configureDialog())); //This isn't implemented in Applet yet...
        desktopMenu.addAction(configureApplet);
    }
    desktopMenu.exec(point.toPoint());
    contextMenuEvent->accept();
}

void Corona::launchExplorer(bool /*param*/)
{
    QProcess::execute("plasmaengineexplorer");
}

} // namespace Plasma

#include "corona.moc"

