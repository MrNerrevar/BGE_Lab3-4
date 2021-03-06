#include "Lab3.h"
#include "Content.h"
#include "VectorDrawer.h"
#include "LazerBeam.h"

using namespace BGE;

Lab3::Lab3(void)
{
	elapsed = 10000;
}


bool Lab3::Initialise()
{
	std::shared_ptr<GameComponent> ground = make_shared<Ground>();
	Attach(ground);	

	ship1 = make_shared<GameComponent>(true);
	ship1->Attach(Content::LoadModel("cobramk3", glm::rotate(glm::mat4(1), 180.0f, glm::vec3(0,1,0))));
	ship1->Attach(make_shared<VectorDrawer>(glm::vec3(5,5,5)));
	ship1->transform->position = glm::vec3(-10, 2, -10);
	Attach(ship1);

	ship2 = make_shared<GameComponent>(true);
	ship2->Attach(Content::LoadModel("python", glm::rotate(glm::mat4(1), 180.0f, glm::vec3(0,1,0))));
	ship2->Attach(make_shared<VectorDrawer>(glm::vec3(5,5,5)));
	ship2->transform->position = glm::vec3(10, 2, -10);
	Attach(ship2);
	Game::Initialise();

	camera->transform->position = glm::vec3(0, 4, 20);
	return true;
}

void Lab3::Update(float timeDelta)
{
	static float timeToFire = 1.0f / 2.0f;

	// Movement of ship2
	if (keyState[SDL_SCANCODE_UP])
	{
		ship2->transform->position += ship2->transform->look * speed * timeDelta;
	}
	if (keyState[SDL_SCANCODE_DOWN])
	{
		ship2->transform->position -= ship2->transform->look * speed * timeDelta;
	}
	if (keyState[SDL_SCANCODE_RIGHT])
	{
		ship2->transform->Yaw(-speed / 6);
	}
	if (keyState[SDL_SCANCODE_LEFT])
	{
		ship2->transform->Yaw(speed / 6);
	}
	

	// Movement of ship1
	if (keyState[SDL_SCANCODE_U])
	{
		ship1->transform->position += ship1->transform->look * speed * timeDelta;
	}
	if (keyState[SDL_SCANCODE_J])
	{
		ship1->transform->position -= ship1->transform->look * speed * timeDelta;
	}
	if (keyState[SDL_SCANCODE_K])
	{
		//ship1->transform->Yaw(-speed / 6);
		ship1->transform->Yaw(-timeDelta * speed * speed);
	}
	if (keyState[SDL_SCANCODE_H])
	{
		//ship1->transform->Yaw(speed / 6);
		ship1->transform->Yaw(timeDelta * speed * speed);
	}

	bool inRange;
	float distance = glm::length(ship2->transform->position - ship1->transform->position);
	if (distance <= 10)
	{
		PrintText("Within Range");
		inRange = true;
		//cout << "Within Range" << endl;
	}
	else
	{
		inRange = false;
		PrintText("Not In Range");
		//cout << "Not in Range" << endl;
	}


	glm::vec3 diff = ship2->transform->position - ship1->transform->position;

	float dot = glm::dot(ship1->transform->look, diff);

	if (dot > 0)
	{
		PrintText("In Front");
		//cout << "In Front" << endl;
	}
	else
	{
		PrintText("Behind");
		//cout << "Behind" << endl;
	}

	

	float lenDiff = glm::length(diff);
	float theta = glm::acos(dot / lenDiff);
	float FoV = 45.0;
	bool inView;

	if (theta > glm::radians(FoV / 2))
	{
		PrintText("Outside FoV");
		inView = false;
	}
	else
	{
		PrintText("Inside FoV");
		inView = true;
	}

	float lazerRate = 0.5f;
	
	if (inView == true && inRange == true && elapsed > lazerRate)
	{
		shared_ptr<LazerBeam> lazer = make_shared<LazerBeam>();
		lazer->transform->position = ship1->transform->position;
		lazer->transform->look = ship1->transform->look;
		Attach(lazer);
		elapsed = 0;
	}



	elapsed += timeDelta;
	Game::Update(timeDelta);
}
