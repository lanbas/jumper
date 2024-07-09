#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "utils.h"
#include "jumper.h"

using namespace std::chrono_literals;

bool running = false;
std::mutex obstacleLock;
std::condition_variable cv;

void printObstacle(Obstacle& o)
{
	std::cout << "============\n";
	std::cout << "Position: (" << o.position().x << ", " <<  o.position().y << ")\n";
	std::cout << "============\n";
}

void printObstacleList(std::deque<Obstacle>& obstacleList)
{
	for (auto& o : obstacleList)
	{
		printObstacle(o);
	}
}

float randomFloatInRange(float min, float max)
{
	return min + static_cast<float>(rand()) / ( static_cast<float>(RAND_MAX / (max - min)) );
}

void spawnObstacles(std::deque<Obstacle>& obstacleList, std::chrono::_V2::system_clock::time_point startTime)
{
	float obstacleVelocityX = kInitialObstacleVelocity;
	float obstacleWidthFactorMin = kInitialObstacleWidthFactorMin;
	float obstacleWidthFactorMax = kInitialObstacleWidthFactorMax;
	uint32_t obstacleSpawnInterval_ms = kInitialObstacleSpawnInterval_ms;

	while (running)
	{
		// Determine total time elapsed
		auto now = std::chrono::high_resolution_clock::now();
		auto totalTimeElapsed_s = std::chrono::duration<float, std::chrono::seconds::period>(now - startTime).count();

		// Velocity and object width distribution factor increase until steady state is reached
		if (totalTimeElapsed_s < kSteadyStateTimeElapsed_s)
		{
			obstacleVelocityX += kObstacleVelocityIncrement;
			obstacleWidthFactorMin += kWidthFactorIncrement;
			obstacleWidthFactorMax += kWidthFactorIncrement;
			obstacleSpawnInterval_ms += kObstacleSpawnIntervalIncrement_ms;
		}

		// Determine obstacle width
		float obstacleWidthFactor = randomFloatInRange(obstacleWidthFactorMin, obstacleWidthFactorMax);
		float obstacleWidth = obstacleWidthFactor * kObstacleUnitWidth_px;
		
		// Initialize as jump obstacle, kDuckObstacleProportion chance to change to duck obstacle
		float obstaclePositionY;
		float obstacleHeight;
		if (randomFloatInRange(0.0f, 1.0f) < kDuckObstacleProportion) // Duck obstacle
		{
			// TODO: Change how duck obstacle height is determined
			obstaclePositionY = randomFloatInRange(kJumperHomeY - kDuckObstacleHeight, kFloorHeight - kDuckHeight - kDuckObstacleHeight); 
			obstacleHeight = kDuckObstacleHeight;
		}
		else // Jump obstacle
		{
			// TODO: Change how jump obstacle height is determined
			// Obstacles can peak <duck obstacle height> px above standing jumper, and be as low as <duck obstacle height> px above ducking jumper
			obstaclePositionY = randomFloatInRange(kJumperHomeY - kDuckObstacleHeight, kFloorHeight - kDuckHeight - kDuckObstacleHeight);
			obstacleHeight = kFloorHeight - obstaclePositionY;
		}

		Obstacle obstacle(obstaclePositionY, {obstacleVelocityX, 0}, obstacleHeight, obstacleWidth);

		std::unique_lock<std::mutex> g(obstacleLock);
		obstacleList.push_back(obstacle);
		// std::cout << "Adding obstacle\n";
		float spawnDeviation_ms = randomFloatInRange(-1 * kObstacleSpawnIntervalDeviation_ms, kObstacleSpawnIntervalDeviation_ms);
		cv.wait_for(g, std::chrono::milliseconds(obstacleSpawnInterval_ms + (int)spawnDeviation_ms));

	}
}

int main(int argc, char** argv)
{
    // Initialize SDL components
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();

	SDL_Window* window = SDL_CreateWindow("Jumper", 0, 0, kWindowWidth, kWindowHeight, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0); 

    // Initialize the font
	TTF_Font* scoreFont = TTF_OpenFont("fonts/DejaVuSansMono.ttf", 64);

	// Initialize sound
	// Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	// Mix_Chunk* wallHitSound = Mix_LoadWAV("audio/pongWallHit.wav");
	// Mix_Chunk* paddleHitSound = Mix_LoadWAV("audio/pongPaddleHit.wav");

    // Create Jumper (starts at configured defaults)
    Jumper jumper;
	auto globalStartTime = std::chrono::high_resolution_clock::now();

    // Game logic
    {
        running = true;
		
		float dt = 0.0f;
		std::deque<Obstacle> obstacleList;
		std::thread t(&spawnObstacles, std::ref(obstacleList), globalStartTime);

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
						case SDLK_DOWN:
							if (jumper.state() == JumperState::RUNNING)
                            {
                                jumper.duck();
                            }
							break;
					}
				}
				else if (event.type == SDL_KEYUP)
				{
					switch(event.key.keysym.sym)
					{
						case SDLK_s:
                            if (jumper.state() == JumperState::DUCKING)
							    jumper.reset();
							break;
						case SDLK_DOWN:
							if (jumper.state() == JumperState::DUCKING)
							    jumper.reset();
							break;
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

            // Clear the window to black
			SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
			SDL_RenderClear(renderer);

            // Set the draw color to be white
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

			jumper.updateMotion(dt);

			{
				// Remove front if necessary, then update + draw remaining
				std::unique_lock<std::mutex> g(obstacleLock);
				if (!obstacleList.empty() && obstacleList.front().right() <= 0)
				{
					// Timing is such that only one obstacle (the front one) will need to be deleted on any given run of this loop
					obstacleList.pop_front();
				}

				for (auto& obstacle : obstacleList)
				{
					// Nice to only iterate once, but idk if there is a benefit to drawing all at once instead of sneaking in an updateMotion
					obstacle.updateMotion(dt);
					obstacle.draw(renderer);
				}
			}

			jumper.draw(renderer);

			// Present the backbuffer
			SDL_RenderPresent(renderer);

            // Calculate frame time
			auto stopTime = std::chrono::high_resolution_clock::now();
			dt = std::chrono::duration<float, std::chrono::milliseconds::period>(stopTime - startTime).count();
        }

		cv.notify_one();
		t.join();

    }

    return 0;
}
// To spawn:
    // Add object to shared obsctacle list at whatever the 
