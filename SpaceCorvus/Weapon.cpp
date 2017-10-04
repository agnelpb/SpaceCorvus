#include "Weapon.h"


void Weapon::load(XMLElement* objectNode, Object* pickableObj)
{
	XMLElement* damageNode = objectNode->FirstChildElement("damage"); // Getting description
	if (damageNode != NULL)
	{
		int damage = std::stoi( damageNode->GetText());           // String to integer conversion
		static_cast<Weapon*>(pickableObj)->damage = damage;       // Ha ! Dodged the minefield ! 
	}
}


Weapon::Weapon()
{
}


Weapon::~Weapon()
{
}
