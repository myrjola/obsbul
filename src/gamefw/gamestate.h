
#ifndef GAMESTATE_H
#define GAMESTATE_H

namespace gamefw {

/**
 * @brief Encapsulates an independent state ie. main menu, pause or ingame.
 */
class IGameState
{
public:
	/**
	 * @brief Processes input and sends the current state to the rendering pipeline.
	 */
	virtual void update();

	/**
	 * @brief Changes the GameState's controller.
	 * 
	 * The controller is used for input processing through the update() method.
	 * 
	 * @param controller ditto.
	 */
    virtual void changeController(shared_ptr<IController> controller);
};

}

#endif // GAMESTATE_H
