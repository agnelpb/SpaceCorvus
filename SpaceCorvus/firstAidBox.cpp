#include "firstAidBox.h"


void firstAidBox::load(XMLElement* objectNode, Object* pickableObj)
{
	XMLElement* healthNode = objectNode->FirstChildElement("health"); // Getting description
	if (healthNode != NULL)
	{
		int health = std::stoi(healthNode->GetText());                  // String to integer conversion
		static_cast<firstAidBox*>(pickableObj)->health = health ;       // Ha ! Dodged the minefield ! Again !
	}
}

firstAidBox::firstAidBox()
{
}


firstAidBox::~firstAidBox()
{
}
