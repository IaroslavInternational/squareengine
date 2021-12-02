#include "Map.h"

Map::Map(std::string mapData, std::string heroData, std::string personData, std::string IobjectsData, std::string triggerData,
		 std::shared_ptr<Window> wnd, std::shared_ptr<Camera> camera)
	:
	mdr(heroData),
	hero(mdr, wnd, camera),
	persons(personData),
	Iobjects(IobjectsData),
	triggers(triggerData),
	objQueue(&hero, &persons, &Iobjects),
	sprite(mapData)
{
	mdr.~MainPersonDataReader();
}

void Map::Draw(Graphics& gfx)
{
	gfx.DrawSpriteNonChroma(position.x, position.y, sprite);
}

void Map::Translate(DirectX::XMFLOAT2 delta)
{
	position.x += delta.x;
	position.y += delta.y;

	Iobjects.Translate(delta);
	triggers.Translate(delta);
	persons.Translate(delta);
}
