#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "utils.h"
#include "jumper.h"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;


// Separate thread
    // Spawn objects after specific amount of time -- should this be done in a separate thread? 

    // Increase object velocity 
        // Increase current velocity value

int main(int argc, char** argv)
{
    // Initialize SDL components
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();

	SDL_Window* window = SDL_CreateWindow("Pong", 0, 0, kWindowWidth, kWindowHeight, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0); 

    // Initialize the font
	TTF_Font* scoreFont = TTF_OpenFont("fonts/DejaVuSansMono.ttf", 64);

	// Initialize sound
	// Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	// Mix_Chunk* wallHitSound = Mix_LoadWAV("audio/pongWallHit.wav");
	// Mix_Chunk* paddleHitSound = Mix_LoadWAV("audio/pongPaddleHit.wav");

    // Create Jumper (starts at configured defaults)
    Jumper jumper;

    // Game logic
    {
        bool running = true;
		float dt = 0.0f;

        // Continue looping and processing events until user exits
		while (running)
		{
			auto startTime = std::chrono::high_resolution_clock::now();
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
				{
					running = false;
				}
				
				// Check which buttons are down/up
				else if (event.type == SDL_KEYDOWN)
				{
					switch(event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							running = false;
							break;
						case SDLK_SPACE:
                            std::cout << "space down\n";
							if (jumper.state() == JumperState::RUNNING)
                            {
                                jumper.jump();
                            }
							break;
						case SDLK_s:
							if (jumper.state() == JumperState::RUNNING)
                            {
                                jumper.duck();
                            }
							break;
						// case SDLK_UP:
						// 	buttonsDown[ControlButtons::PaddleTwoUp] = true;
						// 	break;
						// case SDLK_DOWN:
						// 	buttonsDown[ControlButtons::PaddleTwoDown] = true;
						// 	break;
						// default:
						// 	break;
					}
				}
				else if (event.type == SDL_KEYUP)
				{
					switch(event.key.keysym.sym)
					{
						case SDLK_s:
                            if (jumper.state() == JumperState::DUCKING)
							    jumper.reset();
                    }
                }
				// 		case SDLK_UP:
				// 			buttonsDown[ControlButtons::PaddleTwoUp] = false;
				// 			break;
				// 		case SDLK_DOWN:
				// 			buttonsDown[ControlButtons::PaddleTwoDown] = false;
				// 			break;
				// 		default:
				// 			break;
				// 	}
				// }
			}

            jumper.updateMotion(dt);

            // Clear the window to black
			SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
			SDL_RenderClear(renderer);

            // 
			// Draw stuff to render
            //

            // Set the draw color to be white
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

            // Draw the ball
            jumper.draw(renderer);

			// Present the backbuffer
			SDL_RenderPresent(renderer);

            // Calculate frame time
			auto stopTime = std::chrono::high_resolution_clock::now();
			dt = std::chrono::duration<float, std::chrono::milliseconds::period>(stopTime - startTime).count();
        }


    // Update to latest velocity if necessary (velocity thread has changed it)

    // Draw player

    // Draw obstacles

    }

    return 0;
}
// To spawn:
    // Add object to shared obsctacle list at whatever the 
