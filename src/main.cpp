#include "App.hpp"

#include "Core/Context.hpp"

//#include "Util/Logger.hpp"

int main(int, char**) {
    //Util::Logger::SetLevel(Util::Logger::Level::INFO);
    
    auto context = Core::Context::GetInstance();
    App app;

    while (!context->GetExit()) {
        switch (app.GetCurrentState()) {
            case App::State::START:
                app.Start();
                break;

            case App::State::UPDATE:
                app.Update();
                break;

            case App::State::RESET:
                app.Reset();
                break;

            case App::State::EATING_GHOST:
                app.EatingGhost();
                break;

            case App::State::DYING:
                app.Dying();
                break;
            
            case App::State::DEAD:
                app.Dead();
                break;
            
            case App::State::GAMEOVER:
                app.Gameover();
                break;

            case App::State::END:
                app.End();
                context->SetExit(true);
                break;
        }
        context->Update();
    }
    return 0;
}
