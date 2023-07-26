// Quest System by JDNLY. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "EdGraphNode_QuestSystemGraphNode.h"
#include "SGraphNode.h"

class UEdGraphNode_QuestSystemGraphNode;

class QUESTSYSTEMEDITOR_API SGraphNode_QuestSystemGraphNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SGraphNode_QuestSystemGraphNode){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphNode_QuestSystemGraphNode* InNode);
};
