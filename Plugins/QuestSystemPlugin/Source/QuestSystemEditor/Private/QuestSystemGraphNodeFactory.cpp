// Quest System by JDNLY. All Rights Reserved

#include "QuestSystemGraphNodeFactory.h"
#include "EdGraph/EdGraphNode.h"
#include "QuestSystemGraphAssetEditor/EdGraphNode_QuestSystemGraphNode.h"
#include "QuestSystemGraphAssetEditor/SGraphNode_QuestSystemGraphNode.h"
#include "QuestSystemGraphAssetEditor/EdNodeEdge_QuestSystemGraphEdge.h"
#include "QuestSystemGraphAssetEditor/SNodeEdge_QuestSystemGraphEdge.h"

TSharedPtr<SGraphNode> FQuestSystemGraphNodeFactory::CreateNode(UEdGraphNode* Node) const
{
	if (UEdGraphNode_QuestSystemGraphNode* EdGraphNode = Cast<UEdGraphNode_QuestSystemGraphNode>(Node))
	{
		return SNew(SGraphNode_QuestSystemGraphNode, EdGraphNode);
	}
	if (UEdNodeEdge_QuestSystemGraphEdge* EdNodeEdge = Cast<UEdNodeEdge_QuestSystemGraphEdge>(Node))
	{
		return SNew(SNodeEdge_QuestSystemGraphEdge, EdNodeEdge);
	}
	return nullptr;
}

TSharedPtr<class SGraphPin> FQuestSystemGraphNodePinFactory::CreatePin(class UEdGraphPin* Pin) const
{
	return nullptr;
}
