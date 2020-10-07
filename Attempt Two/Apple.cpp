#include <Apple.h>
#include <Snake.h>
Apple::Apple()
{
	effect = &Apple::effectGrow;
	position = glm::vec3((float)(rand() % PLAY_WIDTH) - PLAY_WIDTH / 2, (float)(rand() % PLAY_HEIGHT) - PLAY_HEIGHT / 2, (float)(rand() % PLAY_DEPTH) - PLAY_DEPTH / 2);
}
void Apple::effectGrow()
{
	new Node;
	new Node;
	new Node;
	new Node;
	new Node;
	position = glm::vec3((float)(rand() % PLAY_WIDTH) - PLAY_WIDTH / 2, (float)(rand() % PLAY_HEIGHT) - PLAY_HEIGHT / 2, (float)(rand() % PLAY_DEPTH) - PLAY_DEPTH / 2);
}