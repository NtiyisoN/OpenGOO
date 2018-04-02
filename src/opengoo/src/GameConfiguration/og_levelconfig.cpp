#include "og_levelconfig.h"
#include "wog_level.h"

#include "../OGLib/util.h"

#include "Parsers/valuewriter.h"

struct OGLevelConfigHelper
{
    static WOGCamera CreateCamera(const QDomElement& element);
    static WOGPoi CreatePoi(const QDomElement& element);
    static WOGBallInstance CreateBallInstance(const QDomElement& element);
    static WOGStrand CreateStrand(const QDomElement& element);
    static QPointF CreateVertex(const QDomElement& element);

    static void Load(WOGLevelExit& obj, const QDomElement& element);
    static void Load(WOGPipe& pipe, const QDomElement& element);
};

template<typename TOptional>
inline void OptionalSetValue(TOptional& aOptional, const QDomElement& aValue)
{
    aOptional.first = true;
    OGLevelConfigHelper::Load(aOptional.second, aValue);
}

WOGPoi OGLevelConfigHelper::CreatePoi(const QDomElement &element)
{
    WOGPoi obj;
    obj.position = ValueWriter::StringToPointF(element.attribute("pos"));
    obj.traveltime = element.attribute("traveltime").toDouble();
    obj.pause = element.attribute("pause").toDouble();
    obj.zoom = element.attribute("zoom").toDouble();

    return obj;
}

WOGCamera OGLevelConfigHelper::CreateCamera(const QDomElement& element)
{
    WOGCamera obj;
    auto aspect = element.attribute("aspect", "normal");
    if (aspect == "normal")
    {
        obj.aspect = WOGCamera::Normal;
    }
    else if (aspect == "widescreen")
    {
        obj.aspect = WOGCamera::WideScreen;
    }
    else
    {
        obj.aspect = WOGCamera::Unknown;
    }

    obj.endpos = ValueWriter::StringToPointF(element.attribute("endpos"));
    obj.endzoom = element.attribute("endzoom", "1").toDouble();

    for (auto node = element.firstChild(); !node.isNull(); node = node.nextSibling())
    {
        obj.poi.push_back(CreatePoi(node.toElement()));
    }

    return obj;
}

WOGBallInstance OGLevelConfigHelper::CreateBallInstance(const QDomElement& element)
{
    WOGBallInstance obj;
    obj.type = element.attribute("type");
    obj.x = element.attribute("x").toFloat();
    obj.y = element.attribute("y").toFloat();

    obj.id = element.attribute("id");
    obj.angle = element.attribute("angle").toFloat();
    obj.discovered = ValueWriter::StringToBool(element.attribute("discovered", "true"));

    return obj;
}

QPointF OGLevelConfigHelper::CreateVertex(const QDomElement &element)
{
    float x = element.attribute("x").toFloat();
    float y = element.attribute("y").toFloat();

    return QPointF(x, y);
}

WOGStrand OGLevelConfigHelper::CreateStrand(const QDomElement& element)
{
    WOGStrand obj;
    obj.gb1 = element.attribute("gb1");
    obj.gb2 = element.attribute("gb2");

    return obj;
}

void OGLevelConfigHelper::Load(WOGLevelExit& obj, const QDomElement &element)
{
    obj.id = element.attribute("id");
    obj.pos = ValueWriter::StringToPointF(element.attribute("pos"));
    obj.radius = element.attribute("radius").toFloat();
    obj.filter = element.attribute("filter");
}

void OGLevelConfigHelper::Load(WOGPipe& pipe, const QDomElement &element)
{
    pipe.id = element.attribute("id", "0");
    pipe.depth = element.attribute("depth", "0").toFloat();
    pipe.type = element.attribute("type");
    for (auto node = element.firstChild(); !node.isNull(); node = node.nextSibling())
    {
        pipe.vertex.push_back(CreateVertex(node.toElement()));
    }
}

OGLevelConfig::Type OGLevelConfig::Parser()
{
    auto level = Type(new WOGLevel);
    OptionalReset(level->levelexit);
    OptionalReset(level->pipe);

    level->ballsrequired = rootElement.attribute("ballsrequired").toInt();
    level->letterboxed = ValueWriter::StringToBool(rootElement.attribute("letterboxed"));
    level->visualdebug = ValueWriter::StringToBool(rootElement.attribute("visualdebug"));
    level->autobounds = ValueWriter::StringToBool(rootElement.attribute("autobounds"));
    level->textcolor = ValueWriter::StringToColor(rootElement.attribute("textcolor"));
    level->texteffects = ValueWriter::StringToBool(rootElement.attribute("texteffects"));
    level->timebugprobability = rootElement.attribute("timebugprobability").toDouble();

    level->strandgeom = ValueWriter::StringToBool(rootElement.attribute("strandgeom"));
    level->allowskip = ValueWriter::StringToBool(rootElement.attribute("allowskip"));

    for (auto node = rootElement.firstChild(); !node.isNull(); node = node.nextSibling())
    {
        auto domElement = node.toElement();

        if (domElement.tagName() == "camera")
        {
            level->camera.push_back(OGLevelConfigHelper::CreateCamera(domElement));
        }
        else if (domElement.tagName() == "music")
        {
            level->music.id = domElement.attribute("id");
        }
        else if (domElement.tagName() == "BallInstance")
        {
            level->ball.push_back(OGLevelConfigHelper::CreateBallInstance(domElement));
        }
        else if (domElement.tagName() == "levelexit")
        {
            if (!OptionalHasValue(level->levelexit))
            {
                OptionalSetValue(level->levelexit, domElement);
            }
        }
        else if (domElement.tagName() == "pipe")
        {
            if (!OptionalHasValue(level->pipe))
            {
                OptionalSetValue(level->pipe, domElement);
            }
        }
        else if (domElement.tagName() == "Strand")
        {
            level->strand.push_back(OGLevelConfigHelper::CreateStrand(domElement));
        }
    }

    return level;
}
