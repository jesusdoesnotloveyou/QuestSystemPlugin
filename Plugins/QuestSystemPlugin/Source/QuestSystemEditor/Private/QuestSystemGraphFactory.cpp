// Quest System by JDNLY. All Rights Reserved


#include "QuestSystemGraphFactory.h"

UQuestSystemGraphFactory::UQuestSystemGraphFactory()
{
	// To create asset in the Content Browser
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UQuestSystemGraph::StaticClass();
}

UQuestSystemGraphFactory::~UQuestSystemGraphFactory()
{
	
}


UObject* UQuestSystemGraphFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UQuestSystemGraph>(InParent, Class, Name, Flags, Context);
}
