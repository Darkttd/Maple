#include <Maple/Lua.h>
#include <Maple/GamerSettings.h>
#include <Bibim/All.h>
using namespace Bibim;

namespace Maple
{
    GamerSettings::GamerSettings()
    {
        SetDefaults();
        Load();
    }

    GamerSettings::~GamerSettings()
    {
        Save();
    }

    void GamerSettings::Load()
    {
        const String path = Environment::GetAppDataPath(ProjectName, "settings");
        FileStreamPtr stream = new FileStream(path, FileStream::ReadOnly);
        if (stream->CanRead() == false)
            return;

        stream->Seek(sizeof(String::UTF8BOM), Stream::FromBegin);
        const int length = stream->GetLength() - sizeof(String::UTF8BOM);
        if (length <= 0)
            return;

        std::vector<char> bodyBytes;
        bodyBytes.resize(length);
        stream->Read(&bodyBytes[0], length);
        stream->Close();

        const String body = String(&bodyBytes[0], 0, length);
        int startIndex = 0;
        while (startIndex < body.GetLength())
        {
            int endIndex = body.Find(String::NewLine, startIndex);
            if (endIndex == -1)
                endIndex = body.GetLength();
                
            String line = body.Substring(startIndex, endIndex - startIndex);
            line.Trim();

            String key;
            String value;
            const int keyValueIndex = line.Find('=');
            if (keyValueIndex != -1)
            {
                key = line.Substring(0, keyValueIndex);
                value = line.Substring(keyValueIndex + 1);
            }
            else
                key = line;

            if(key.EqualsIgnoreCase("MoaStandingCharacterCloth"))
                SetMoaStandingCharacterCloth(ConvertFromCharsToStandingCharacterCloth(value.CStr()));
            else if(key.EqualsIgnoreCase("MapleStandingCharacterCloth"))
                SetMapleStandingCharacterCloth(ConvertFromCharsToStandingCharacterCloth(value.CStr()));
            else if (key.EqualsIgnoreCase("BGMMute"))
                SetBGMMute(Bool::Parse(value));
            else if (key.EqualsIgnoreCase("SFXMute"))
                SetSFXMute(Bool::Parse(value));
            else if (key.EqualsIgnoreCase("LastViewPuzzlePage"))
                SetLastViewPuzzlePage(Int::Parse(value));
            else if (key.StartsWith("@"))
                Set(&key.CStr()[1], value.CStr());

            startIndex = endIndex + String::NewLine.GetLength();
        }
    }

    void GamerSettings::Save()
    {
        String body;

        body.Append("MoaStandingCharacterCloth=");
        body.Append(ConvertFromStandingCharacterClothToChars(GetMoaStandingCharacterCloth()));
        body.Append(String::NewLine);

        body.Append("MapleStandingCharacterCloth=");
        body.Append(ConvertFromStandingCharacterClothToChars(GetMapleStandingCharacterCloth()));
        body.Append(String::NewLine);

        body.Append("BGMMute=");
        body.Append(Bool::ToString(GetBGMMute()));
        body.Append(String::NewLine);

        body.Append("SFXMute=");
        body.Append(Bool::ToString(GetSFXMute()));
        body.Append(String::NewLine);
        
        body.Append("LastViewPuzzlePage=");
        body.Append(Int::ToString(GetLastViewPuzzlePage()));
        body.Append(String::NewLine);

        for (std::map<String, String>::const_iterator it = values.begin(); it != values.end(); it++)
        {
            body.Append("@");
            body.Append((*it).first);
            body.Append("=");
            body.Append((*it).second);
            body.Append(String::NewLine);
        }

        const String path = Environment::GetAppDataPath(ProjectName, "settings");
        FileStreamPtr stream = new FileStream(path, FileStream::WriteOnly);
        stream->Write(String::UTF8BOM, sizeof(String::UTF8BOM));
        stream->Write(body.CStr(), body.GetLength());
        stream->Close();
    }

    void GamerSettings::SetDefaults()
    {
        standingCharacter = Unselected;
        moaStandingCharacterCloth = SchoolUniform;
        mapleStandingCharacterCloth = SchoolUniform;
        bgmMute = false;
        sfxMute = false;
        lastViewPuzzlePage = 0;
    }

    void GamerSettings::Reset()
    {
        SetDefaults();
        Apply();
    }

    void GamerSettings::Apply()
    {
        Save();

        for (Listeners<IListener>::Iterator it = listeners.Begin(); it != listeners.End(); it++)
            (*it)->OnApply(this);
    }

    void GamerSettings::SetStandingCharacter(StandingCharacter value)
    {
        standingCharacter = value;
    }

    void GamerSettings::SetMoaStandingCharacterCloth(StandingCharacterCloth value)
    {
        moaStandingCharacterCloth = value;
    }

    void GamerSettings::SetMapleStandingCharacterCloth(StandingCharacterCloth value)
    {
        mapleStandingCharacterCloth = value;
    }

    void GamerSettings::SetBGMMute(bool value)
    {
        bgmMute = value;
    }

    void GamerSettings::SetSFXMute(bool value)
    {
        sfxMute = value;
    }

    void GamerSettings::SetLastViewPuzzlePage(int value)
    {
        lastViewPuzzlePage = value;
    }

    const char* GamerSettings::Get(const char* key) const
    {
        if (key == nullptr)
            return nullptr;

        std::map<String, String>::const_iterator it = values.find(key);
        if (it != values.end())
            return (*it).second.CStr();
        else
            return nullptr;
    }

    void GamerSettings::Set(const char* key, const char* value)
    {
        if (key == nullptr)
            return;

        if (value)
            values[key] = value;
        else
        {
            std::map<String, String>::iterator it = values.find(key);
            if (it != values.end())
                values.erase(it);
        }
    }

    const char* GamerSettings::ConvertFromStandingCharacterToChars(StandingCharacter value)
    {
        switch (value)
        {
            case Unselected:      return "Unselected";
            case Kradmoa:         return "Kradmoa";
            case Maple:           return "Maple";
            default:              return "Unselected";
        }
    }

    GamerSettings::StandingCharacter GamerSettings::ConvertFromCharsToStandingCharacter(const char* value)
    {
             if (String::EqualsCharsIgnoreCase(value, "Unselected")) return Unselected;
        else if (String::EqualsCharsIgnoreCase(value, "Kradmoa"))    return Kradmoa;
        else if (String::EqualsCharsIgnoreCase(value, "Maple"))      return Maple;
        else                                                         return Unselected;
    }

    const char* GamerSettings::ConvertFromStandingCharacterClothToChars(StandingCharacterCloth value)
    {
        switch (value)
        {
            case SchoolUniform:  return "SchoolUniform";
            case Hanbok:         return "Hanbok";
            case Gimono:         return "Gimono";
            case ChinaDress:     return "ChinaDress";
            case Nurse:          return "Nurse";
            case SwimSuits:      return "SwimSuits";
            case Maid:           return "Maid";
            default:             return "SchoolUniform";
        }
    }

    GamerSettings::StandingCharacterCloth GamerSettings::ConvertFromCharsToStandingCharacterCloth(const char* value)
    {
             if (String::EqualsCharsIgnoreCase(value, "SchoolUniform")) return SchoolUniform;
        else if (String::EqualsCharsIgnoreCase(value, "Hanbok"))        return Hanbok;
        else if (String::EqualsCharsIgnoreCase(value, "Gimono"))        return Gimono;
        else if (String::EqualsCharsIgnoreCase(value, "ChinaDress"))    return ChinaDress;
        else if (String::EqualsCharsIgnoreCase(value, "Nurse"))         return Nurse;
        else if (String::EqualsCharsIgnoreCase(value, "SwimSuits"))     return SwimSuits;
        else if (String::EqualsCharsIgnoreCase(value, "Maid"))          return Maid;
        else                                                            return SchoolUniform;
    }
}