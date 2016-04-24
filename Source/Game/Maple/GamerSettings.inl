namespace Maple
{
    void GamerSettings::AddListener(IListener* item)
    {
        listeners.Add(item);
    }

    void GamerSettings::AddListener(IListener* item, Bibim::SharedObject* object)
    {
        listeners.Add(item, object);
    }

    bool GamerSettings::RemoveListener(IListener* item)
    {
        return listeners.Remove(item);
    }

    GamerSettings::StandingCharacter GamerSettings::GetStandingCharacter() const
    {
        return standingCharacter;
    }

    GamerSettings::StandingCharacterCloth GamerSettings::GetMoaStandingCharacterCloth() const
    {
        return moaStandingCharacterCloth;
    }

    GamerSettings::StandingCharacterCloth GamerSettings::GetMapleStandingCharacterCloth() const
    {
        return mapleStandingCharacterCloth;
    }

    bool GamerSettings::GetBGMMute() const
    {
        return bgmMute;
    }

    bool GamerSettings::GetSFXMute() const
    {
        return sfxMute;
    }

    int GamerSettings::GetLastViewPuzzlePage() const
    {
        return lastViewPuzzlePage;
    }
}