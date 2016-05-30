#pragma once

#include <Maple/FWD.h>
//#include <Bibim/BitMask.h>
//#include <Bibim/IUpdateable.h>
#include <Bibim/UIImage.h>
#include <Bibim/UIPanel.h>
//#include <Bibim/Font.h>
//#include <Bibim/FontLibrary.h>
#include <Maple/PatternManager/PatternManager.h>

namespace Maple
{
    class UIPuzzleSelect : public Bibim::UIPanel
    {
        // Define UI of "Puzzle Select Scene"

        BBComponentClass(UIPuzzleSelect, Bibim::UIPanel, 'u', 'i', 'p', 'z');
        public:
            UIPuzzleSelect();
            UIPuzzleSelect(Bibim::GameModuleTree* modules);
            virtual ~UIPuzzleSelect();

            Bibim::UIWindow* GetUILayer() const;
            void SetUILayer(Bibim::UIWindow* value);

            Bibim::UIWindow* GetUnlockedLayer() const;
            Bibim::UIWindow* GetLockedLayer() const;

            float GetPatternPixel() const;
            void SetPatternPixel(float value);

            int GetCurrentPage() const;
            void SetCurrentPage(int value);

            Maple::PatternManager* GetPatternManager() const;
            void SetPatternManager(Maple::PatternManager* value);

        protected:
            virtual void OnDraw(Bibim::UIDrawingContext& context);

            void DrawPuzzlePatterns(Bibim::UIDrawingContext& context);
            void DrawPuzzle(Bibim::UIDrawingContext& context, int index, PatternManager::PuzzleInformation& puzzleInformation, Vector2 centerPos);
            void DrawCover(Bibim::UIDrawingContext& context, int index, bool isUnlocked);

        private:
            Bibim::GameModuleTree* modules;
            Bibim::GraphicsDevice* graphicsDevice;
            Bibim::UIWindowPtr uiLayer;
            Bibim::UIWindowPtr unlockedLayer;
            Bibim::UIWindowPtr lockedLayer;

            Maple::PatternManager* patternManager;

            float patternPixel;
            int currentPage;
    };
}
