#include<stdio.h>

struct item
{
	int power;
	struct player* owner;
};
struct item new_item(int power, struct player* owner)
{
	struct item an_item;

	an_item.power = power;
	an_item.owner = owner;

	return an_item;
}

struct player
{
	int age;
	struct item item_1;
	struct item item_2;
};
struct player new_player(int age)
{
	struct player a_player;
	a_player.age = age;
	a_player.item_1 = new_item(1, &a_player);
	a_player.item_2 = new_item(2, &a_player);

	return a_player;
}

int main()
{
	struct player player_1 = new_player(77);

	printf("Age of the player: %d.\n", player_1.age);
	printf("Age of the player (from item_1): %d.\n", player_1.item_1.owner->age);
	printf("Age of the player (from item_2): %d.\n", player_1.item_2.owner->age);

	printf("Age of the player (from item_1 by owner): %d.\n",
		   	player_1.item_1.owner->item_1.owner->age);
	printf("Age of the player (from item_2 by owner): %d.\n",
		   	player_1.item_2.owner->item_2.owner->age);

	printf("Age of the player (from item_1 by owner to item_2): %d.\n",
		   	player_1.item_1.owner->item_2.owner->age);
	printf("Age of the player (from item_2 by owner to item_1): %d.\n",
		   	player_1.item_2.owner->item_1.owner->age);
	
	printf("Power of the first item: %d, power of the second item: %d.\n",
		   	player_1.item_1.power, player_1.item_2.power);
	printf("Power of the first item (from item_1): %d, power of the first item (from item_2): %d.\n",
		   	player_1.item_1.owner->item_1.power, player_1.item_2.owner->item_1.power);
	printf("Power of the second item (from item_1): %d, power of the second item (from item_2): %d.\n",
		   	player_1.item_1.owner->item_2.power, player_1.item_2.owner->item_2.power);

	return 0;
}
