#include <Maple/Gameplay/UIPuzzleSelect.h>
#include <Bibim/UIWindow.h>
#include <Bibim/UIEventMap.h>
using namespace Bibim;

namespace Maple
{
    BBImplementsComponent(UIPuzzleSelect);

    UIPuzzleSelect::UIPuzzleSelect()
    {
        // for cloning and deserialization
    }

    UIPuzzleSelect::UIPuzzleSelect( Bibim::GameModuleTree* modules )
        : modules(modules),
          graphicsDevice(nullptr),
          uiLayer(nullptr),
          unlockedLayer(new UIWindow()),
          lockedLayer(new UIWindow()),
          patternManager(nullptr),
          patternPixel(0.0f),
          currentPage(0)
    {
        unlockedLayer->SetName("UnlockedLayer");
        unlockedLayer->SetPickable(false);
        Add(unlockedLayer);

        lockedLayer->SetName("LockedLayer");
        lockedLayer->SetPickable(false);
        Add(lockedLayer);
    }

    UIPuzzleSelect::~UIPuzzleSelect()
    {
        if (UIEventMap* em = GetEventMap())
        {
            em->ClearHandlers();
            em->ClearDelayedHandlers();
        }

        RemoveAll();
    }

    Bibim::UIWindow* UIPuzzleSelect::GetUILayer() const
    {
        return uiLayer;
    }

    void UIPuzzleSelect::SetUILayer( Bibim::UIWindow* value )
    {
        if(uiLayer != value)
        {
            if(uiLayer)
                Remove(uiLayer);

            uiLayer = value;

            if(uiLayer)
                Add(uiLayer);
        }
    }

    Bibim::UIWindow* UIPuzzleSelect::GetUnlockedLayer() const
    {
        return unlockedLayer;
    }

    Bibim::UIWindow* UIPuzzleSelect::GetLockedLayer() const
    {
        return lockedLayer;
    }

    float UIPuzzleSelect::GetPatternPixel() const
    {
        return patternPixel;
    }

    void UIPuzzleSelect::SetPatternPixel( float value )
    {
        patternPixel = value;
    }

    int UIPuzzleSelect::GetCurrentPage() const
    {
        return currentPage;
    }

    void UIPuzzleSelect::SetCurrentPage(int value)
    {
        currentPage = value;
    }

    Maple::PatternManager* UIPuzzleSelect::GetPatternManager() const
    {
        return patternManager;
    }

    void UIPuzzleSelect::SetPatternManager( Maple::PatternManager* value )
    {
        patternManager = value;

        SetCurrentPage(0);
    }

    void UIPuzzleSelect::OnDraw( Bibim::UIDrawingContext& context )
    {
        if(uiLayer)
            DrawChild(context, uiLayer);

        DrawPuzzlePatterns( context );
    }

    void UIPuzzleSelect::DrawPuzzlePatterns( Bibim::UIDrawingContext& context )
    {
        PatternManager::PuzzleInformationCollection puzzles;

        puzzles = patternManager->GetPuzzleInformationOnPage(0, currentPage);

        UIWindowPtr puzzleSelectWindow;

        puzzleSelectWindow = static_cast<UIWindow*>(uiLayer->FindChild("PuzzleSelect", false))->FindChild("Buttons", false);
        Vector2 defaultOffset = uiLayer->FindChild("PuzzleSelect", false)->GetXY() + puzzleSelectWindow->GetXY();
        defaultOffset += context.GetOffset();

        for( int i = 0; i < puzzles.size(); i++ )
        {
            UIVisual* targetButton = puzzleSelectWindow->FindChild( String::CFormat("Item%d", i + 1), false );
            BBAssert(targetButton->IsWindow());

            UIVisual* targetLock = static_cast<UIWindow*>(targetButton)->FindChild( "Lock", false );
            UIVisual* targetCleared = static_cast<UIWindow*>(targetButton)->FindChild( String::CFormat("Solved%d", puzzles[i].puzzleSize), false );

            if( !targetLock->IsVisible() && targetCleared->IsVisible() )
            {
                DrawPuzzle(context, i, puzzles[i], defaultOffset + targetButton->GetXY() + targetButton->GetSize() / 2.0f);
                DrawCover(context, i, puzzles[i].isUnlocked);
            }
        }
    }
    
    void UIPuzzleSelect::DrawPuzzle( Bibim::UIDrawingContext& context, int index, PatternManager::PuzzleInformation& puzzleInformation, Vector2 centerPos )
    {
        float blockPixelSize = static_cast<int>(patternPixel / static_cast<float>(puzzleInformation.puzzleSize));

        centerPos -= Vector2(0.0f, patternPixel / 8.0f);
        Vector2 leftTopPos = centerPos - Vector2(patternPixel, patternPixel) / 2.0f;

        for(int i = 0; i < puzzleInformation.puzzleSize; i++)
        {
            for(int j = 0; j < puzzleInformation.puzzleSize; j++)
            {
                if(puzzleInformation.puzzlePattern[i][j] == MapleGameLogic::TableState::Colored)
                {
                    const RectF bounds = 
                        RectF(leftTopPos.X + blockPixelSize * i, leftTopPos.Y + blockPixelSize * j,
                               blockPixelSize, blockPixelSize);

                    context.FillRect(bounds, Color::Black);
                }
            }
        }
    }

    void UIPuzzleSelect::DrawCover( Bibim::UIDrawingContext& context, int index, bool isUnlocked )
    {

    }

    void UIPuzzleSelect::OnRead(ComponentStreamReader& /*reader*/)
    {
        BBBreak();
    }

    void UIPuzzleSelect::OnCopy(const GameComponent* /*original*/, CloningContext& /*context*/)
    {
        BBBreak();
    }
}
