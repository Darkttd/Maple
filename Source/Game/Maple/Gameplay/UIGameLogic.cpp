#include <Maple/Gameplay/UIGameLogic.h>
#include <Bibim/Texture2D.h>
#include <Bibim/UIWindow.h>
#include <Bibim/Timeline.h>
#include <Bibim/CommandQueue.h>
#include <Bibim/GameModuleTree.h>
#include <Bibim/UIEventMap.h>
#include <Bibim/UIMouseEventArgs.h>
#include <Bibim/UIMouseButtonEventArgs.h>
#include <Bibim/UIDrawingContext.h>
#include <Bibim/Log.h>
#include <Bibim/Math.h>
#include <Bibim/StandardGame.EmbeddedFont.h>
#include <Bibim/UIAppEventDispatcher.h>
#include <Bibim/UIEventMap.h>
#include <Bibim/UIAsyncEventQueue.h>
#include <Maple/PatternManager/PatternManager.h>
#include <Maple/Gameplay/GenericEventArgs.h>
#include <vector>
using namespace Bibim;
using namespace std;

namespace Maple
{
    BBImplementsComponent(UIGameLogic);

    UIGameLogic::UIGameLogic()
    {
        // for cloning and deserialization
    }

    UIGameLogic::UIGameLogic(Bibim::GameModuleTree* modules)
        : modules(modules),
          graphicsDevice(nullptr),
          timeline(nullptr),
          history(new CommandQueue()),
          uiLayer(nullptr),
          cursorLayer(new UIWindow()),
          numberLayer(nullptr),
          mapleGameLogic(nullptr),
          zeroPointOffset(Vector2()),
          isGameAreaClicking(false),
          isPauseMenuShowing(false),
          isClearUIShowing(false),
          isUnlockUIShowing(false),
          isGameOverUIShowing(false),
          currentPatternNumber(-1),
          blockPixelSize(0.0f),
          interBlockOffset(0.0f),
          font(nullptr)
    {
        SetFocusable(true);

        cursorLayer->SetName("Cursor");
        cursorLayer->SetPickable(false);

        Add(cursorLayer);

        graphicsDevice = modules->FindModule<GraphicsDevice>();
        appEventDispatcher = modules->FindModule<UIAppEventDispatcher>();

        UIEventMap* eventMap = new UIEventMap();
        eventMap->SetAsyncEventQueue(modules->FindModule<UIAsyncEventQueue>());
        eventMap->SetEventDispatcher(appEventDispatcher);
        SetEventMap(eventMap);
    }

    UIGameLogic::~UIGameLogic()
    {
        //SetTimeline(nullptr);
        if (UIEventMap* em = GetEventMap())
        {
            em->ClearHandlers();
            em->ClearDelayedHandlers();
        }

        delete mapleGameLogic;

        RemoveAll();

        delete history;
    }

    void UIGameLogic::Initialize(PatternManager* patternManager, int patternNumber)
    {
        {
            currentPatternNumber = patternNumber;
            const PatternManager::PuzzleInformation& puzzleInfo = patternManager->GetPuzzleInformationOnNumber(0, patternNumber);

            int puzzleSize = puzzleInfo.puzzleSize;
            MapleGameLogic::GamePanel ansPattern;

            for(int i = 0; i < puzzleSize; i++)
            {
                for(int j = 0; j < puzzleSize; j++)
                {
                    ansPattern.arr[i][j] = puzzleInfo.puzzlePattern[i][j];
                }
            }

            mapleGameLogic = new MapleGameLogic(puzzleSize, ansPattern);
        }

        if (FontLibrary* fontLibrary = modules->FindModule<FontLibrary>())
        {
            int tempPuzzleSize = mapleGameLogic->GetPuzzleSize();

            font = new Font(fontLibrary);
            font->SetFace(EMBEDDED_FONT_DATA,
                          sizeof(EMBEDDED_FONT_DATA) / sizeof(EMBEDDED_FONT_DATA[0]));
            font->SetColor(Color::Black);

            switch(tempPuzzleSize)
            {
            case 5:
                font->SetSize(20.0f);
                break;

            case 10:
                font->SetSize(18.0f);
                break;

            case 15:
                font->SetSize(11.0f);
                break;

            case 20:
                font->SetSize(8.0f);
                break;
            }

            //font->SetScale(30.0f);
            font->SetFaceURI("Asset/NanumGothic");
            //font->SetShadowOffsetXY(500.0f,400.0f);
            //font->set
        }
    }

    void UIGameLogic::Restart()
    {
        mapleGameLogic->Restart();
    }

    void UIGameLogic::Update(float /*dt*/, int /*timestamp*/)
    {
    }

    void UIGameLogic::SetTimeline(Timeline* value)
    {
        if (timeline != value)
        {
            if (timeline)
                timeline->Remove(this);

            timeline = value;

            if (timeline)
                timeline->Add(this);
        }
    }

    void UIGameLogic::SetUILayer(UIWindow* value)
    {
        if(uiLayer != value)
        {
            if(uiLayer)
                Remove(uiLayer);

            uiLayer = value;
            UpdateSelectedRowLayer();
            
            if(uiLayer)
                Add(uiLayer);
        }
    }

    void UIGameLogic::SetPauseUILayer(UIWindow* value)
    {
        if(pauseUiLayer != value)
        {
            if(pauseUiLayer)
                Remove(pauseUiLayer);

            pauseUiLayer = value;

            if(pauseUiLayer)
                Add(pauseUiLayer);
        }
    }

    void UIGameLogic::SetGameOverUILayer(UIWindow* value)
    {
        if(gameOverUiLayer != value)
        {
            if(gameOverUiLayer)
                Remove(gameOverUiLayer);

            gameOverUiLayer = value;

            if(gameOverUiLayer)
                Add(gameOverUiLayer);
        }
    }

    void UIGameLogic::SetNumberLayer( Bibim::UIWindow* value )
    {
        if (numberLayer != value)
        {
            if(numberLayer)
                Remove(numberLayer);

            numberLayer = value;
            
            if(numberLayer)
                Add(numberLayer);
        }
    }

    void UIGameLogic::UpdateCursorLayer(const Point2& position)
    {
        UIVisual* child = cursorLayer->FindChild("AnimatedCursor", false);
        
        if(child)
        {
            child->SetXY(position.X * (blockPixelSize + interBlockOffset) + zeroPointOffset.X + (blockPixelSize + interBlockOffset) / 2,
                         position.Y * (blockPixelSize + interBlockOffset) + zeroPointOffset.Y + (blockPixelSize + interBlockOffset) / 2);
        }
    }

    void UIGameLogic::UpdateSelectedRowLayer()
    {
        // <TODO> This function will be rewrite on Script

        UIVisual* gamePanel = uiLayer->FindChild("GamePanel", false);

        if(gamePanel != nullptr && gamePanel->IsPanel())
        {
            int ps = mapleGameLogic->GetPuzzleSize();

            String panelName = String::CFormat("Panel%d%d", ps, ps);

            UIVisual* targetPanel = StaticCast<UIPanel>(gamePanel)->FindChild(panelName, false);

            if(targetPanel != nullptr && targetPanel->IsPanel())
            {
                Point2 cursor = mapleGameLogic->GetCursor();
                Point2 cursorTail = mapleGameLogic->GetCursorTail();

                // Off all

                UIVisual* verticalVisual = StaticCast<UIPanel>(targetPanel)->FindChild("Vertical", false);
                UIVisual* horizontalVisual = StaticCast<UIPanel>(targetPanel)->FindChild("Horizontal", false);

                UIPanel* verticalPanel;
                UIPanel* horizontalPanel;

                if(verticalVisual != nullptr && verticalVisual->IsPanel())
                    verticalPanel = static_cast<UIPanel*>(verticalVisual);
                else
                    return;

                if(horizontalVisual != nullptr && horizontalVisual->IsPanel())
                    horizontalPanel = static_cast<UIPanel*>(horizontalVisual);
                else
                    return;

                for(int i = 0; i < ps; i++)
                {
                    verticalPanel->FindChild(String::CFormat("%d", i), false)->SetVisibility(UIVisual::Invisible);
                    horizontalPanel->FindChild(String::CFormat("%d", i), false)->SetVisibility(UIVisual::Invisible);
                }

                if(cursor == cursorTail)
                {
                    verticalPanel->FindChild(String::CFormat("%d", cursor.Y), false)->SetVisibility(UIVisual::Visible);
                    horizontalPanel->FindChild(String::CFormat("%d", cursor.X), false)->SetVisibility(UIVisual::Visible);
                }
                else
                {
                    BBAssert(cursor.X == cursorTail.X || cursor.Y == cursorTail.Y);

                    // <TODO> Duplicate with DrawClickedPattern.

                    // dic is direction which from cursor to cursorTail
                    Point2 dic = cursorTail - cursor;

                    if(dic.X != 0)
                        dic.X /= Math::Abs(dic.X);
                    else
                        dic.Y /= Math::Abs(dic.Y);

                    for(; cursor != (cursorTail + dic); cursor += dic)
                    {
                        verticalPanel->FindChild(String::CFormat("%d", cursor.Y), false)->SetVisibility(UIVisual::Visible);
                        horizontalPanel->FindChild(String::CFormat("%d", cursor.X), false)->SetVisibility(UIVisual::Visible);
                    }
                }
            }
        }
    }

    void UIGameLogic::SetZeroPointOffset(Bibim::Vector2 value)
    {
        zeroPointOffset = value;
    }

    void UIGameLogic::SetBlockPixelSize(float value)
    {
        blockPixelSize = value;
    }

    void UIGameLogic::SetInterBlockOffset(float value)
    {
        interBlockOffset = value;
    }

    void UIGameLogic::SetPauseMenuShowing(bool value)
    {
        isPauseMenuShowing = value;
    }

    void UIGameLogic::SetClearUIShowing(bool value)
    {
        isClearUIShowing = value;
    }

    void UIGameLogic::SetUnlockUIShowing(bool value)
    {
        isUnlockUIShowing = value;
    }

    void UIGameLogic::SetGameOverUIShowing(bool value)
    {
        isGameOverUIShowing = value;
    }

    void UIGameLogic::OnDraw(Bibim::UIDrawingContext& context)
    {
        if(uiLayer)
            DrawChild(context, uiLayer);

        if(!IsClearUIShowing())
        {
            //if (numberLayer != nullptr)
            //{
            //    numberLayer->SetXY(30.0f, 30.0f);
            //    DrawChild(context, numberLayer);
            //}
            
            DrawNumberCount(context);
            DrawClickedPattern(context);
        }

        if(!IsUnlockUIShowing())
            DrawPattern(context);

        if(!IsClearUIShowing())
        {
            DrawChild(context, cursorLayer);
        }

        if(pauseUiLayer)
            DrawChild(context, pauseUiLayer);

        if(isGameOverUIShowing)
            DrawChild(context, gameOverUiLayer);

        contextOffset = context.GetOffset();
        //Matrix4 contextTransform = context.GetCurrentTransform();
        //contextScale = Vector2(contextTransform.M00, contextTransform.M11);

        contextMatrixInv = context.GetCurrentTransformInv();
    }

    Point2 UIGameLogic::GetAxisAlignedMousePosition(Point2 startPoint, Point2 currentPoint) const
    {
        if(Math::Abs(startPoint.X - currentPoint.X) > Math::Abs(startPoint.Y - currentPoint.Y))
        {
            // if diff of x is larger, use currentPoint's x

            return Point2(currentPoint.X, startPoint.Y);
        }
        else
        {
            // if not, use currentPoint's y

            return Point2(startPoint.X, currentPoint.Y);
        }
    }

    Point2 UIGameLogic::GetPickedGridPosition(Point2 mousePosition) const
    {
        Point2 ret;

        if(mapleGameLogic)
        {
            const float x = zeroPointOffset.X + contextOffset.X;
            const float y = zeroPointOffset.Y + contextOffset.Y;
            //const float x = zeroPointOffset.X;
            //const float y = zeroPointOffset.Y;

            ret = Point2((mousePosition.X - x) / (blockPixelSize + interBlockOffset),
                            (mousePosition.Y - y) / (blockPixelSize + interBlockOffset));

        
            ret.X = Math::Clamp(ret.X, 0, mapleGameLogic->GetPuzzleSize() - 1);
            ret.Y = Math::Clamp(ret.Y, 0, mapleGameLogic->GetPuzzleSize() - 1);
        }

        return ret;
    }

    RectF UIGameLogic::GetGameAreaRect() const
    {
        if(mapleGameLogic)
        {
            int puzzleSize = mapleGameLogic->GetPuzzleSize();
            return RectF(
                GetZeroPointOffset().X + contextOffset.X,
                GetZeroPointOffset().Y + contextOffset.Y,
                //GetZeroPointOffset().X,
                //GetZeroPointOffset().Y,
                (blockPixelSize + interBlockOffset) * puzzleSize,
                (blockPixelSize + interBlockOffset) * puzzleSize);
        }
        else
            return RectF::Empty;
    }

    void UIGameLogic::CursorMove(int x, int y)
    {
        //Bibim::Log::Warning(Bibim::String::CFormat("%d, %d", x, y));

        if(Math::Abs(x) + Math::Abs(y) != 1)
            return;

        if(mapleGameLogic)
        {
            mapleGameLogic->CursorMove(Point2(x, y));

            UpdateCursorLayer(mapleGameLogic->GetCursor());
            UpdateSelectedRowLayer();
        }
    }

    void UIGameLogic::TryFillColor()
    {
        if(mapleGameLogic)
        {
            MapleGameLogic::TryFillColorResult::T result = mapleGameLogic->TryFillColor();

            switch(result)
            {
                // send event to Lua

                case MapleGameLogic::TryFillColorResult::GameOver:
                    appEventDispatcher->RaiseEvent(UIEventID("GameOver"), Args(this));
                    break;

                case MapleGameLogic::TryFillColorResult::Clear:
                    appEventDispatcher->RaiseEvent(UIEventID("GameClear"), Args(this));
                    break;

                case MapleGameLogic::TryFillColorResult::Incorrect:
                    appEventDispatcher->RaiseEvent(UIEventID("OnIncorrect"), Args(this, mapleGameLogic->GetRestLife()));
                    break;

                case MapleGameLogic::TryFillColorResult::Correct:
                    appEventDispatcher->RaiseEvent(UIEventID("OnCorrect"), Args(this));
                    break;
            }
        }
    }

    void UIGameLogic::TryFillErased()
    {
        if(mapleGameLogic)
        {
            mapleGameLogic->TryFillErased();
        }
    }

    int UIGameLogic::GetPuzzleSize() const
    {
        if(mapleGameLogic)
        {
            return mapleGameLogic->GetPuzzleSize();
        }
        
        return -1;
    }

    int UIGameLogic::GetPatternNumber() const
    {
        return currentPatternNumber;
    }
    
    Bibim::Point2 UIGameLogic::GetInvertedContextCoordinate(float x, float y) const
    {
        const Vector3 translatedMousePos = contextMatrixInv.Transform(Vector3(x, y, 0.0f));
        const Point2 targetMousePos = Point2(translatedMousePos.X, translatedMousePos.Y);

        return targetMousePos;
    }

    bool UIGameLogic::OnMouseMove(const Bibim::UIMouseEventArgs& args)
    {
        //const Point2 targetMousePos = Point2(args.GetPositionX() / contextScale.X - contextOffset.X,
            //args.GetPositionY() / contextScale.Y - contextOffset.Y);
        const Vector3 translatedMousePos = contextMatrixInv.Transform(Vector3(args.GetPositionX(), args.GetPositionY(), 0));
        const Point2 targetMousePos = Point2(translatedMousePos.X, translatedMousePos.Y);

        if(isGameAreaClicking && mapleGameLogic)
        {
            // If mouse click start from This Game, This event will be called until unclick even if mouse pointer is out of window.
            const Point2 gridPos = GetPickedGridPosition(GetAxisAlignedMousePosition(clickStartPosition, targetMousePos));

            mapleGameLogic->SetCursor(gridPos);
            UpdateCursorLayer(gridPos);
            UpdateSelectedRowLayer();
        }

        return Base::OnMouseMove(args);
    }

    bool UIGameLogic::OnMouseButtonDown(const Bibim::UIMouseButtonEventArgs& args)
    {
        //const Point2 targetMousePos = Point2(args.GetPositionX() / contextScale.X - contextOffset.X,
        //args.GetPositionY() / contextScale.Y - contextOffset.Y);
        const Vector3 translatedMousePos = contextMatrixInv.Transform(Vector3(args.GetPositionX(), args.GetPositionY(), 0));
        const Point2 targetMousePos = Point2(translatedMousePos.X, translatedMousePos.Y);

        if(!IsPauseMenuShowing() && !IsGameOverUIShowing() && mapleGameLogic &&
            GetGameAreaRect().Contains(Vector2(targetMousePos.X, targetMousePos.Y)) && args.GetButtonCode() == Key::MouseLeftButton)
        {
            Point2 gridPos = GetPickedGridPosition(targetMousePos);

            UpdateCursorLayer(gridPos);
            mapleGameLogic->SetCursorTail(gridPos);
            UpdateSelectedRowLayer();

            clickStartPosition = targetMousePos;
            isGameAreaClicking = true;

            return true;
        }

        return Base::OnMouseButtonDown(args);
    }

    bool UIGameLogic::OnMouseButtonUp(const Bibim::UIMouseButtonEventArgs& args)
    {
        //const Point2 targetMousePos = Point2(args.GetPositionX() / contextScale.X - contextOffset.X,
        //args.GetPositionY() / contextScale.Y - contextOffset.Y);
        const Vector3 translatedMousePos = contextMatrixInv.Transform(Vector3(args.GetPositionX(), args.GetPositionY(), 0));
        const Point2 targetMousePos = Point2(translatedMousePos.X, translatedMousePos.Y);

        if(!IsPauseMenuShowing() && !IsGameOverUIShowing() && mapleGameLogic)
        {
            if(isGameAreaClicking && args.GetButtonCode() == Key::MouseLeftButton)
            {
                // The case which Left Button is Clicking

                const Point2 position = GetPickedGridPosition(GetAxisAlignedMousePosition(clickStartPosition, targetMousePos));
                UpdateCursorLayer(position);
                UpdateSelectedRowLayer();

                isGameAreaClicking = false;
            }
            else
            {
                isGameAreaClicking = false;
            }

            return true;
        }
        
        return Base::OnMouseButtonUp(args);
    }

    void UIGameLogic::DrawNumberCount(Bibim::UIDrawingContext& context)
    {
        vector<vector<int> > horizontal = mapleGameLogic->GetHorizontalNumberCount();
        vector<vector<int> > vertical = mapleGameLogic->GetVerticalNumberCount();

        int ps = mapleGameLogic->GetPuzzleSize();

        {
            Vector2 defaultOffset;
            Vector2 xSideOffset;
            Vector2 ySideOffset;
            Vector2 centerOffset;

            switch(ps)
            {
            case 5:
                defaultOffset = Vector2(-35.0f, 24.0f);
                xSideOffset = Vector2(-20.0f, 0.0f); // Distance between characters
                ySideOffset = Vector2(0.0f, 74.0f);
                centerOffset = Vector2(0.0f, 0.0f);
                break;

            case 10:
                defaultOffset = Vector2(-27.0f, 9.0f);
                xSideOffset = Vector2(-16.0f, 0.0f); // Distance between characters
                ySideOffset = Vector2(0.0f, 37.0f);
                centerOffset = Vector2(-10.0f, 0.0f); // offset for above 10
                break;

            case 15:
                defaultOffset = Vector2(-17.0f, 11.0f);
                xSideOffset = Vector2(-12.0f, 0.0f); // Distance between characters
                ySideOffset = Vector2(0.0f, 24.58f);
                centerOffset = Vector2(-7.0f, 0.0f); // offset for above 10
                break;

            case 20:
                defaultOffset = Vector2(-18.0f, 8.0f);
                xSideOffset = Vector2(-10.0f, 0.0f); // Distance between characters
                ySideOffset = Vector2(0.0f, 18.5f);
                centerOffset = Vector2(-8.0f, 0.0f); // offset for above 10
                break;
            }

            defaultOffset += context.GetOffset();

            for(int i = 0; i < static_cast<int>(vertical.size()); i++)
            {
                Vector2 centerOffsetAcc = Vector2::Zero;

                for(int j = 0; j < static_cast<int>(vertical[i].size()); j++)
                {
                    String targetString = String::CFormat("%d", vertical[i][vertical[i].size() - j - 1]);

                    if (targetString.GetLength() == 2)
                        centerOffsetAcc += centerOffset / 2.0f;

                    Vector2 targetPos = zeroPointOffset + defaultOffset + ySideOffset * static_cast<float>(i) + xSideOffset * static_cast<float>(j)
                            + centerOffsetAcc;

                    if (targetString.GetLength() == 2)
                        centerOffsetAcc += centerOffset / 2.0f;

                    targetPos = Vector2((int)targetPos.X, (int)targetPos.Y);

                    if (numberLayer != nullptr)
                    {
                        context.DrawUnclipped(targetPos, 0.0f,
                            (static_cast<UIImage*>(numberLayer->FindChild(targetString, true)))->GetSource());
                    }
                    else
                    {
                        context.DrawString(targetPos, font, targetString);
                    }
                }
            }
        }

        {
            Vector2 defaultOffset;
            Vector2 xSideOffset;
            Vector2 ySideOffset;
            Vector2 centerOffset;

            switch(ps)
            {
            case 5:
                defaultOffset = Vector2(29.0f, -35.0f);
                xSideOffset = Vector2(74.0f, 0.0f); 
                ySideOffset = Vector2(0.0f, -30.0f); // Distance between characters
                centerOffset = Vector2(0.0f, 0.0f);
                break;

            case 10:
                defaultOffset = Vector2(12.0f, -30.0f);
                xSideOffset = Vector2(37.0f, 0.0f); 
                ySideOffset = Vector2(0.0f, -20.0f); // Distance between characters
                centerOffset = Vector2(-6.0f, 0.0f); // offset for above 10
                break;

            case 15:
                defaultOffset = Vector2(10.0f, -14.0f);
                xSideOffset = Vector2(24.6f, 0.0f); 
                ySideOffset = Vector2(0.0f, -14.0f); // Distance between characters
                centerOffset = Vector2(0.0f, 0.0f); // offset for above 10
                break;

            case 20:
                defaultOffset = Vector2(7.0f, -14.0f);
                xSideOffset = Vector2(18.48f, 0.0f); 
                ySideOffset = Vector2(0.0f, -10.0f); // Distance between characters
                centerOffset = Vector2(0.0f, 0.0f); // offset for above 10
                break;
            }

            defaultOffset += context.GetOffset();

            for(int i = 0; i < static_cast<int>(horizontal.size()); i++)
            {
                for(int j = 0; j < static_cast<int>(horizontal[i].size()); j++)
                {
                    String targetString = String::CFormat("%d", horizontal[i][j]);
                    Vector2 targetPos = zeroPointOffset + defaultOffset + xSideOffset * static_cast<float>(i) + ySideOffset * static_cast<float>(j)
                            + (targetString.GetLength() == 2 ? centerOffset : Vector2::Zero);

                    targetPos = Vector2((int)targetPos.X, (int)targetPos.Y);

                    if (numberLayer != nullptr)
                    {
                        context.DrawUnclipped(targetPos, 0.0f,
                            (static_cast<UIImage*>(numberLayer->FindChild(targetString, true)))->GetSource());
                    }
                    else
                    {
                        context.DrawString(
                            targetPos,
                            font,
                            targetString);
                    }
                }
            }
        }

    }

    void UIGameLogic::DrawPattern(Bibim::UIDrawingContext& context)
    {
        if(mapleGameLogic)
        {
            int puzzleSize = mapleGameLogic->GetPuzzleSize();
            const MapleGameLogic::GamePanel& panel = mapleGameLogic->GetSolvePanel();
            const float x = zeroPointOffset.X + context.GetOffsetX();
            const float y = zeroPointOffset.Y + context.GetOffsetY();

            for(int i = 0; i < puzzleSize; i++)
            {
                for(int j = 0; j < puzzleSize; j++)
                {
                    const RectF bounds = RectF(i * (blockPixelSize + interBlockOffset) + x,
                                               j * (blockPixelSize + interBlockOffset) + y,
                                               blockPixelSize,
                                               blockPixelSize);

                    switch(panel.arr[i][j])
                    {
                        case MapleGameLogic::TableState::Uncolored:
                            if(IsClearUIShowing())
                            {
                                context.FillRect(bounds, Color::White);
                            }
                            break;

                        case MapleGameLogic::TableState::Erased:
                            if(IsClearUIShowing())
                            {
                                context.FillRect(bounds, Color::White);
                            }
                            else
                            {
                                context.DrawLine(bounds.GetLeftTop(), bounds.GetRightBottom(), Color::Black);
                                context.DrawLine(bounds.GetLeftTop() + Vector2(1, 0), bounds.GetRightBottom() + Vector2(0, -1), Color::Black);
                                context.DrawLine(bounds.GetLeftTop() + Vector2(0, 1), bounds.GetRightBottom() + Vector2(-1, 0), Color::Black);

                                context.DrawLine(bounds.GetRightTop(), bounds.GetLeftBottom(), Color::Black);
                                context.DrawLine(bounds.GetRightTop() + Vector2(-1, 0), bounds.GetLeftBottom() + Vector2(0, -1), Color::Black);
                                context.DrawLine(bounds.GetRightTop() + Vector2(0, 1), bounds.GetLeftBottom() + Vector2(1, 0), Color::Black);
                            }
                            break;

                        case MapleGameLogic::TableState::ForceErased:
                            if(IsClearUIShowing())
                            {
                                context.FillRect(bounds, Color::White);
                            }
                            else
                            {
                                context.DrawLine(bounds.GetLeftTop(), bounds.GetRightBottom(), Color::Red);
                                context.DrawLine(bounds.GetLeftTop() + Vector2(1, 0), bounds.GetRightBottom() + Vector2(0, -1), Color::Red);
                                context.DrawLine(bounds.GetLeftTop() + Vector2(0, 1), bounds.GetRightBottom() + Vector2(-1, 0), Color::Red);

                                context.DrawLine(bounds.GetRightTop(), bounds.GetLeftBottom(), Color::Red);
                                context.DrawLine(bounds.GetRightTop() + Vector2(-1, 0), bounds.GetLeftBottom() + Vector2(0, -1), Color::Red);
                                context.DrawLine(bounds.GetRightTop() + Vector2(0, 1), bounds.GetLeftBottom() + Vector2(1, 0), Color::Red);
                            }
                            break;

                        case MapleGameLogic::TableState::Colored:
                            context.FillRect(bounds, Color::Black);
                            break;
                    }
                }
            }
        }
    }

    void UIGameLogic::DrawClickedPattern(Bibim::UIDrawingContext& context)
    {
        // Draw filled blue square where selected area

        // Cursor is printed cursorLayer.

        if(mapleGameLogic)
        {
            Point2 cursor = mapleGameLogic->GetCursor();
            Point2 cursorTail = mapleGameLogic->GetCursorTail();

            if(cursor != cursorTail)
            {
                // Only draw blue when cursor and cursorTail are not same position //
                const float x = zeroPointOffset.X + context.GetOffsetX();
                const float y = zeroPointOffset.Y + context.GetOffsetY();

                BBAssert(cursor.X == cursorTail.X || cursor.Y == cursorTail.Y);

                // dic is direction where from cursor to cursorTail
                Point2 dic = cursorTail - cursor;

                if(dic.X != 0)
                    dic.X /= Math::Abs(dic.X);
                else
                    dic.Y /= Math::Abs(dic.Y);

                for(; cursor != (cursorTail + dic); cursor += dic)
                {
                    RectF bounds = RectF(cursor.X * (blockPixelSize + interBlockOffset) + x,
                                         cursor.Y * (blockPixelSize + interBlockOffset) + y,
                                         blockPixelSize,
                                         blockPixelSize);

                    context.FillRect(bounds, Color::SkyBlue);
                }
            }
        }
    }

    void UIGameLogic::OnRead(ComponentStreamReader& /*reader*/)
    {
        BBBreak();
    }

    void UIGameLogic::OnCopy(const GameComponent* /*original*/, CloningContext& /*context*/)
    {
        BBBreak();
    }

}
