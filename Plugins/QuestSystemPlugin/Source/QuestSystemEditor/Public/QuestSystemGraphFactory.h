// Quest System by JDNLY. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemGraph.h"
#include "Factories/Factory.h"
#include "QuestSystemGraphFactory.generated.h"

UCLASS()
class QUESTSYSTEMEDITOR_API UQuestSystemGraphFactory : public UFactory
{
	GENERATED_BODY()

public:
	UQuestSystemGraphFactory();
	virtual ~UQuestSystemGraphFactory() override;

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	
};
