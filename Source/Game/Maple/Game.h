#pragma once

#include <Bibim/StandardGame.h>
#include <Bibim/MouseState.h>
#include <Bibim/String.h>
#include <Maple/GamerSettings.h>

namespace Maple
{
    class Game : public Bibim::StandardGame, public GamerSettings::IListener
    {
        BBThisIsNoncopyableClass(Game);
        public:
            Game();
            ~Game();

        protected:
            virtual void Initialize();
            virtual void Update(float dt, int timestamp);
            virtual void ReloadScripts();

        private:
            // Local Functions
            virtual void OnApply(GamerSettings* settings);

        private:
            Bibim::MouseState lastMouseState;
    };
}
