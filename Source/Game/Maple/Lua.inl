namespace Maple
{
    Game* Lua::GetGame() const
    {
        return game;
    }

    GamerSettings* Lua::GetGamerSettings() const
    {
        return gamerSettings;
    }

    GamerSavedata* Lua::GetGamerSavedata() const
    {
        return gamerSavedata;
    }

    PatternManager* Lua::GetPatternManager() const
    {
        return patternManager;
    }
}