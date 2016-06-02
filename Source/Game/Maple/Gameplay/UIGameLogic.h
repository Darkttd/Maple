#pragma once

#include <Maple/FWD.h>
#include <Bibim/BitMask.h>
#include <Bibim/IUpdateable.h>
#include <Bibim/UIImage.h>
#include <Bibim/UIPanel.h>
#include <Bibim/Font.h>
#include <Bibim/FontLibrary.h>
#include <Maple/Gameplay/MapleGameLogic.h>

namespace Maple
{
    class UIGameLogic : public Bibim::UIPanel, public Bibim::IUpdateable
    {
        // Define UI of "SquareLogic"

        BBComponentClass(UIGameLogic, Bibim::UIPanel, 'u', 'i', 'g', 'l');
        public:
            UIGameLogic();
            UIGameLogic(Bibim::GameModuleTree* modules);
            virtual ~UIGameLogic();

            void Initialize(Maple::PatternManager* patternManager, int patternNumber);
            void Restart();
            
            virtual void Update(float dt, int timestamp);

            inline Bibim::Timeline* GetTimeline() const;
            void SetTimeline(Bibim::Timeline* value);

            inline Bibim::UIWindow* GetUILayer() const;
            void SetUILayer(Bibim::UIWindow* value);

            inline Bibim::UIWindow* GetPauseUILayer() const;
            void SetPauseUILayer(Bibim::UIWindow* value);

            inline Bibim::UIWindow* GetGameOverUILayer() const;
            void SetGameOverUILayer(Bibim::UIWindow* value);

            inline Bibim::UIWindow* GetCursorLayer() const;

            inline Bibim::UIWindow* GetNumberLayer() const;
            void SetNumberLayer(Bibim::UIWindow* value);

            inline Bibim::Vector2 GetZeroPointOffset() const;
            void SetZeroPointOffset(Bibim::Vector2 value);

            inline Bibim::Vector2 GetContextOffset() const;
            Bibim::Point2 GetInvertedContextCoordinate(float x, float y) const;

            inline float GetBlockPixelSize() const;
            void SetBlockPixelSize(float value);

            inline float GetInterBlockOffset() const;
            void SetInterBlockOffset(float value);

            inline void SetZeroPointOffsetXY(float x, float y);

            inline bool IsPauseMenuShowing() const;
            void SetPauseMenuShowing(bool value);

            inline bool IsClearUIShowing() const;
            void SetClearUIShowing(bool value);

            inline bool IsGameOverUIShowing() const;
            void SetGameOverUIShowing(bool value);

            inline bool IsUnlockUIShowing() const;
            void SetUnlockUIShowing(bool value);

            void CursorMove(int x, int y);
            void TryFillColor();
            void TryFillErased();

            int GetPuzzleSize() const;
            int GetPatternNumber() const;

            Bibim::Point2 GetAxisAlignedMousePosition(Bibim::Point2 startPoint, Bibim::Point2 currentPoint) const;
            Bibim::Point2 GetPickedGridPosition(Bibim::Point2 mousePosition) const;
            Bibim::RectF GetGameAreaRect() const;

            inline void UpdateCursorLayerXY(float x, float y);

        protected:
            virtual void OnDraw(Bibim::UIDrawingContext& context);

            virtual bool OnMouseMove(const Bibim::UIMouseEventArgs& args);
            virtual bool OnMouseButtonDown(const Bibim::UIMouseButtonEventArgs& args);
            virtual bool OnMouseButtonUp(const Bibim::UIMouseButtonEventArgs& args);
            //virtual bool OnMouseButtonPressing(const Bibim::UIMouseButtonEventArgs& args);

            void DrawNumberCount(Bibim::UIDrawingContext& context);
            void DrawPattern(Bibim::UIDrawingContext& context);
            void DrawClickedPattern(Bibim::UIDrawingContext& context);

            void UpdateCursorLayer(const Point2& position);
            void UpdateSelectedRowLayer();

        private:
            Bibim::GameModuleTree* modules;
            Bibim::GraphicsDevice* graphicsDevice;
            Bibim::Timeline* timeline;
            Bibim::CommandQueue* history;
            Bibim::UIWindowPtr uiLayer;
            Bibim::UIWindowPtr pauseUiLayer;
            Bibim::UIWindowPtr gameOverUiLayer;
            Bibim::UIWindowPtr cursorLayer;
            Bibim::UIWindowPtr numberLayer;
            //Bibim::UIImagePtr backgroundLayer;
            Bibim::UIAppEventDispatcher* appEventDispatcher;

            Bibim::FontPtr font;

            MapleGameLogic* mapleGameLogic;

            Bibim::Vector2 zeroPointOffset;
            Bibim::Vector2 contextOffset;
            Bibim::Matrix4 contextMatrixInv;
            bool isGameAreaClicking;
            bool isPauseMenuShowing;
            bool isClearUIShowing;
            bool isUnlockUIShowing;
            bool isGameOverUIShowing;
            int currentPatternNumber;
            Bibim::Point2 clickStartPosition;
            float blockPixelSize;
            float interBlockOffset;
    };

    Bibim::Timeline* UIGameLogic::GetTimeline() const
    {
        return timeline;
    }

    Bibim::UIWindow* UIGameLogic::GetUILayer() const
    {
        return uiLayer;
    }

    Bibim::UIWindow* UIGameLogic::GetPauseUILayer() const
    {
        return pauseUiLayer;
    }

    Bibim::UIWindow* UIGameLogic::GetGameOverUILayer() const
    {
        return gameOverUiLayer;
    }

    Bibim::UIWindow* UIGameLogic::GetNumberLayer() const
    {
        return numberLayer;
    }

    Bibim::UIWindow* UIGameLogic::GetCursorLayer() const
    {
        return cursorLayer;
    }

    Bibim::Vector2 UIGameLogic::GetZeroPointOffset() const
    {
        return zeroPointOffset;
    }
    
    float UIGameLogic::GetBlockPixelSize() const
    {
        return blockPixelSize;
    }

    float UIGameLogic::GetInterBlockOffset() const
    {
        return interBlockOffset;
    }

    void UIGameLogic::SetZeroPointOffsetXY(float x, float y)
    {
        SetZeroPointOffset(Vector2(x, y));
    }

    Bibim::Vector2 UIGameLogic::GetContextOffset() const
    {
        return contextOffset;
    }

    bool UIGameLogic::IsPauseMenuShowing() const
    {
        return isPauseMenuShowing;
    }
    
    bool UIGameLogic::IsClearUIShowing() const
    {
        return isClearUIShowing;
    }

    bool UIGameLogic::IsUnlockUIShowing() const
    {
        return isUnlockUIShowing;
    }

    bool UIGameLogic::IsGameOverUIShowing() const
    {
        return isGameOverUIShowing;
    }

    void UIGameLogic::UpdateCursorLayerXY(float x, float y)
    {
        UpdateCursorLayer(Point2(x, y));
    }
}
