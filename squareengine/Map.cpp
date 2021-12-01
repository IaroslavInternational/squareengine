#include "Map.h"

Map::Map(std::string heroData, std::string personData, std::string IobjectsData, std::string triggerData,
		 std::shared_ptr<Window> wnd, std::shared_ptr<Camera> camera)
	:
	mdr(heroData),
	hero(mdr, wnd, camera),
	persons(personData),
	Iobjects(IobjectsData),
	triggers(triggerData),
	objQueue(&hero, &persons, &Iobjects)
{
	mdr.~MainPersonDataReader();
}

void Map::Translate(DirectX::XMFLOAT2 delta)
{
	Iobjects.Translate(delta);
	triggers.Translate(delta);
	persons.Translate(delta);
}
