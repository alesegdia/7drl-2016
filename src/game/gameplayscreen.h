#pragma once

#include <alligator/game/iscreen.h>
#include <alligator/util/matrix.h>
#include <alligator/camera/camera.h>
#include <secs/secs.h>

#include "../core/gameworld.h"

#include <allegro5/allegro_ttf.h>

class PhackmanGame;

class GameplayScreen : public IScreen {
public:
	GameplayScreen(PhackmanGame* g);
    ~GameplayScreen() override = default;

	// IScreen interface
	void show() override;
	void update(double delta) override;
	void render() override;
	void hide() override;

private:
	PhackmanGame* m_game;

    Camera::SharedPtr m_cam;
    Camera::SharedPtr m_guiCam;

    std::shared_ptr<GameWorld> gw;

    int m_scale = 2;

    bool m_pause = true;

};
