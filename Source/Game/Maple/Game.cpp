#include <Bibim/All.h>
#include <Maple/Game.h>
#include <Maple/Lua.h>
#include <Maple/Version.h>
#include <Maple/GamerSettings.h>
#include <Maple/GamerSavedata.h>
#include <Maple/PatternManager/PatternManager.h>

using namespace Bibim;

namespace Maple
{
    const char* ProjectName = "Maple";

    Game::Game()
        : StandardGame(Point2(800, 480), Point2(800, 480))
    {
    }
    
    Game::~Game()
    { 
    }
    
    void Game::Initialize()
    {
        SetFixedTimeStep(false);
        SetFixedElapsedTime(1.0f / 30.0f);

        Maple::Lua* lua = new Maple::Lua(this);
        StandardGame::Start("MoemoeNemologic", Version::ToString(), lua);

        SetClearColor(Color::Black);

        GamerSettings* settings = new GamerSettings();
        settings->AddListener(this);
        GetModules()->GetRoot()->AttachChild(settings);

        GamerSavedata* savedata = new GamerSavedata();
        GetModules()->GetRoot()->AttachChild(savedata);

        // lua->SetArchive(mainMPQ);
        lua->SetGamerSettings(settings);
        lua->SetGamerSavedata(savedata);

        settings->Apply();

        ReloadUI();

    }

    void Game::Update(float dt, int timestamp)
    {
        Mouse* mouse = GetMouse();
        const MouseState mouseState = mouse->GetState();
        const bool anyKeyPressed = GetKeyboard()->GetState().IsPressed();
        const bool mouseMoved = mouseState.Position != lastMouseState.Position;
        lastMouseState = mouseState;

        if (anyKeyPressed && mouseMoved == false)
            mouse->Hide();
        else if (mouseMoved && anyKeyPressed == false)
            mouse->Show();

        StandardGame::Update(dt, timestamp);
    }
    
    void Game::OnApply(GamerSettings* settings)
    {
        if (BGM* bgm = GetBGM())
        {
            bgm->SetMute(settings->GetBGMMute());
            //bgm->SetVolume(static_cast<float>(settings->GetBGMVolume()    - GamerSettings::BGMVolumeMin) / 
            //               static_cast<float>(GamerSettings::BGMVolumeMax - GamerSettings::BGMVolumeMin));
        }

        if (SoundFX* sfx = GetSFX())
        {
            sfx->SetMute(settings->GetSFXMute());
            //sfx->SetVolume(static_cast<float>(settings->GetSFXVolume()    - GamerSettings::SFXVolumeMin) / 
            //               static_cast<float>(GamerSettings::SFXVolumeMax - GamerSettings::SFXVolumeMin));
        }
    }

    void Game::ReloadScripts()
    {
        //Bibim::Log::Warning("ReloadScripts Called");

        StandardGame::LuaBase* lua = GetLua();
        lua->Load("Asset/Script/Localization");
        lua->Load("Asset/Script/Localization-ko");
        lua->Load("Asset/Script/Localization-ja");
        lua->Load("Asset/Script/Localization-en");
        lua->Load("Asset/Script/Animation");
        lua->Load("Asset/Script/Constant");
        lua->Load("Asset/Script/Contents");
        lua->Load("Asset/Script/UI");
        lua->Load("Asset/Script/Standing");
        lua->Load("Asset/Script/Game");
        //lua->Load("Asset/Script/GFX");
        //lua->Load("Asset/Script/SFX");
        lua->Load("Asset/Script/AnimationExtention");
		lua->Load("Asset/Script/PuzzleData");
		lua->Load("Asset/Script/PuzzleSelect");
        lua->Load("Asset/Script/dump");

        // <TODO> ReloadScript() 에서는, 스크립트 관련 코드만 들어가야 합니다.
        // 추후 구조 개선을 시작한다면, 이 코드를 다른 곳으로 빼던지 해야 합니다.
        PatternManager* patternManager = new PatternManager();
        GetModules()->GetRoot()->AttachChild(patternManager);
        patternManager->Init(GetLua());
        static_cast<Maple::Lua*>(lua)->SetPatternManager(patternManager);

        lua_tinker::call<String>(lua->GetState(), "main");

    }
}

BBMain(Maple::Game);
