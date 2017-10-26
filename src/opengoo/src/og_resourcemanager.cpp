#include <QFile>
#include <QImage>
#include <QString>

#include "GameConfiguration/binltlfile.h"
#include "GameConfiguration/wog_text.h"
#include "GameConfiguration/wog_resources.h"

#include <logger.h>

#include "og_resourcemanager.h"
#include "og_resourceconfig.h"
#include "og_data.h"

bool OGResourceManager::ParseResourceFile(const QString& a_filename)
{
    if (m_resources.contains(a_filename))
        return true;

    QString filename = a_filename;
    if (!QFile::exists(filename))
    {
        filename += ".bin";
        if (!QFile::exists(filename))
        {
            logWarn("File:" + a_filename + " not exists");
            return false;
        }
    }

    WOGResourcesSPtr res;
    if (!Load(res, filename))
    {
        logWarn("Could not load file:" + filename);
        return false;
    }

    m_resources[filename] = res;

    return true;
}

bool OGResourceManager::ParseTextFile(const QString& a_filename, const QString& a_language)
{
    m_text.reset(OGData::GetText(a_filename, a_language));
    return m_text.get();
}

bool OGResourceManager::ParseFxFile(const QString& a_filename)
{
    m_effects.reset(OGData::GetEffects(a_filename));
    return m_effects.get();
}

og::ImageSourceSPtr OGResourceManager::CreateImageSource(const QString& a_filename)
{
#ifdef Q_OS_MAC
    auto fn = a_filename + ".png.binltl";
    if (!QFile::exists(fn))
    {
        return std::make_shared<og::ImageSource>();
    }

    QFile file(fn);
    if (!file.open(QIODevice::ReadOnly))
    {
        return std::make_shared<og::ImageSource>();
    }

    auto data = BinLtlFile::Decompress(file);
    return std::make_shared<og::ImageSource>(data);
#else
    return std::make_shared<og::ImageSource>(a_filename + ".png");
#endif
}

og::ImageSourceSPtr OGResourceManager::GetImageSourceById(const QString& a_id)
{
    if (auto is = m_imageSources.value(a_id))
        return is;

    QString filename;
    foreach (auto& res, m_resources)
    {
        filename = res->GetImage(a_id);
        if (!filename.isEmpty())
            break;
    }

    if (filename.isEmpty())
    {
        logWarn("Could not find id:" + a_id);
        return nullptr;
    }

    auto is = CreateImageSource(filename);
    m_imageSources[a_id] = is;
    return is;
}

template<typename T>
bool OGResourceManager::Load(T& a_data, const QString& a_filename)
{
    typename T::element_type::Conf conf(a_filename);
    if (!conf.Open())
    {
        logWarn("Could not open file:" + a_filename);
        return false;
    }

    if (!conf.Read())
    {
        logWarn("Could not read file:" + a_filename);
        return false;
    }

    a_data.reset(conf.Parser());

    return true;
}

const WOGBall* OGResourceManager::GetBallByType(const QString& a_type)
{
    if (auto ball = m_balls.value(a_type))
        return ball.get();
    
    WOGBallPtr ball;
    QString path = "res/balls/" + a_type + "/balls.xml";
    Load(ball, path);
    if (!ball)
        return nullptr;

    ParseResourceFile("res/balls/" + a_type + "/resources.xml");

    auto sp = WOGBallSPtr(ball.release());
    m_balls[a_type] = sp;

    return sp.get();
}

og::audio::SoundSource* OGResourceManager::AddSoundSource(const QString a_id)
{
    foreach (auto& res, m_resources)
    {
        auto filename = res->GetSound(a_id);
        if (!filename.isEmpty())
        {
            filename.append(".ogg");
            auto it = m_soundSources.insert(a_id, og::audio::SoundSource(filename.toStdString()));;
            return &(it.value());
        }
    }

    return nullptr;
}

const og::audio::SoundSource* OGResourceManager::GetSoundSource(const QString& a_id)
{
    auto it = m_soundSources.find(a_id);
    if (it != m_soundSources.end())
    {
        return &it.value();
    }

    return AddSoundSource(a_id);
}

SoundSPtr OGResourceManager::GetSound(const QString& a_id)
{
    auto it = m_soundSources.find(a_id);
    if (it != m_soundSources.end())
    {
        return std::make_shared<og::audio::Sound>(it.value());
    }

    if (auto src = AddSoundSource(a_id))
    {
        return std::make_shared<og::audio::Sound>(*src);
    }

    return nullptr;
}

MusicSPtr OGResourceManager::GetMusic(const QString& a_id)
{
    if (m_Music.id == a_id)
    {
        return m_Music.audio;
    }

    foreach (auto& res, m_resources)
    {
        auto filename = res->GetSound(a_id);
        if (!filename.isEmpty())
        {
            m_Music.id = filename;
            filename.append(".ogg");
            m_Music.audio = std::make_shared<og::audio::Music>(filename.toStdString());
            return m_Music.audio;
        }
    }

    return nullptr;
}

QString OGResourceManager::GetText(const QString& aId)
{
    auto& string = m_text->string;
    auto it = string.find(aId);
    if (it == string.end())
    {
        return "";
    }

    return it.value();
}
