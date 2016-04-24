#include <Maple/Lua.h>
#include <Maple/GamerSavedata.h>
#include <Bibim/All.h>
using namespace Bibim;

namespace Maple
{
    GamerSavedata::GamerSavedata()
    {
        Load();
    }

    GamerSavedata::~GamerSavedata()
    {
        Save();
    }

    void GamerSavedata::Load()
    {
        const String path = Environment::GetAppDataPath(ProjectName, "maple_savedata");
        FileStreamPtr stream = new FileStream(path, FileStream::ReadOnly);

        if (stream->CanRead())
        {
            BinaryReader reader(stream);
            const int version = reader.ReadInt();

            BBAssert(version == CurrentSaveDataVersion);

            const int count = reader.ReadInt();
            for (int i = 0; i < count; i++)
                items.insert(reader.ReadString());
            stream->Close();
        }

        InitialUnlockedItem();
    }

    void GamerSavedata::Save()
    {
        const String path = Environment::GetAppDataPath(ProjectName, "maple_savedata");
        FileStreamPtr stream = new FileStream(path, FileStream::WriteOnly);
        BinaryWriter writer(stream);
        writer.Write(CurrentSaveDataVersion);
        writer.Write(static_cast<int>(items.size()));
        for (std::set<String>::const_iterator it = items.begin(); it != items.end(); it++)
            writer.Write(*it);
        stream->Close();
    }

    void GamerSavedata::InitialUnlockedItem()
    {
        for(int i = 1; i <= 10; i++)
        {
            items.insert( String::CFormat( "MOA0%02d", i) );
            items.insert( String::CFormat( "MPL0%02d", i) );
        }

        items.insert( "KradmoaSchoolUniform" );
        items.insert( "MapleSchoolUniform" );
    }

    void GamerSavedata::Unlock(const String& name)
    {
        items.insert(name);
    }

    bool GamerSavedata::IsUnlocked(const String& name) const
    {
        return items.find(name) != items.end();
    }

    void GamerSavedata::Reset()
    {
        items.clear();
        InitialUnlockedItem();
    }

    int GamerSavedata::GetMoaSolvedCount() const
    {
        int count = 0;

        for(std::set<String>::iterator it = items.begin(); it != items.end(); it++)
        {
            if(it->Contains("MOA") && it->Contains("_Clear"))
                count++;
        }

        return count;
    }

    int GamerSavedata::GetMapleSolvedCount() const
    {
        int count = 0;

        for(std::set<String>::iterator it = items.begin(); it != items.end(); it++)
        {
            if(it->Contains("MPL") && it->Contains("_Clear"))
                count++;
        }

        return count;
    }
}