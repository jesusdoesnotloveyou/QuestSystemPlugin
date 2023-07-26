// Quest System by JDNLY. All Rights Reserved

#include "QuestSystemGraphAssetEditor/SGraphNode_QuestSystemGraphNode.h"

void SGraphNode_QuestSystemGraphNode::Construct(const FArguments& InArgs, UEdGraphNode_QuestSystemGraphNode* InNode)
{
	GraphNode = InNode;
	UpdateGraphNode();
}
