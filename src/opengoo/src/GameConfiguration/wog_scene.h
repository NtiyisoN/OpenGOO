#pragma once

#include <QSizeF>

#include "wog_circle.h"
#include "wog_vobject.h"

struct WOGLabel
{
    QString id;
    float depth;
    QPointF position;
    QString align;
    float rotation;
    float scale;
    bool overlay;
    bool screenspace;
    QString font;
    QString text;
};

struct WOGSceneLayer : public WOGVObject
{
    QString id;
    QString name;
    QString image;
    QString anim;
    float animspeed;
};

struct WOGRadialForceField
{
    QString id;
    QString type;
    QPointF center;
    float radius;
    float forceatcenter;
    float forceatedge;
    float dampeningfactor;
    bool antigrav;
    bool geomonly;
    bool enabled;
};

struct WOGLinearForceField
{
    QString type;
    QPointF force;
    float dampeningfactor;
    bool antigrav;
    bool geomonly;
};

struct WOGButton : public WOGVObject
{
    QString id;
    QString up;
    QString over;
    QString disabled;
    QString onclick;
    QString onmouseenter;
    QString onmouseexit;
    QString text;
    QString font;
};

struct WOGButtonGroup
{
    QString id;
    QPointF osx;
    std::vector<WOGButton> button;
};

struct WOGLine : public WOGPObject
{
    QPointF anchor;
    QPointF normal;
};

struct WOGRectangle : public WOGPObject
{
    QPointF position;
    QSizeF size;
    float rotation;
};

struct WOGCompositeGeom : public WOGPObject
{
    QPointF position;
    float rotation;
    std::vector<WOGCircle> circle;
    std::vector<WOGRectangle> rectangle;
};

struct WOGScene
{
    struct WOGParticle
    {
        QString effect;
        float depth;
        QPointF position;
        float pretick;
    };

    float minx;
    float miny;
    float maxx;
    float maxy;
    QColor backgroundcolor;
    std::vector<WOGButton> button;
    std::vector<WOGButtonGroup> buttongroup;
    std::vector<WOGSceneLayer> sceneLayer;
    std::vector<WOGLabel> label;
    std::vector<WOGCircle> circle;
    std::vector<WOGLine> line;
    std::vector<WOGRectangle> rectangle;
    std::vector<WOGLinearForceField> linearforcefield;
    std::vector<WOGRadialForceField> radialforcefield;
    std::vector<WOGParticle> particle;
    std::vector<WOGCompositeGeom> compositegeom;

    WOGButtonGroup* GetButtonGroup(const QString&);
    WOGButton* FindButton(const QString &id);
};
