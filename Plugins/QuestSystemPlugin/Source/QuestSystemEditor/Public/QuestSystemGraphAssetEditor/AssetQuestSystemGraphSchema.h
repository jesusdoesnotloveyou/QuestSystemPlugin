// Quest System by JDNLY. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "EdGraphNode_QuestSystemGraphNode.h"
#include "EdGraph/EdGraphSchema.h"
#include "AssetQuestSystemGraphSchema.generated.h"

// Action to add a node to the graph
USTRUCT()
struct QUESTSYSTEMEDITOR_API FQuestSystemEdGraphSchemaAction_NewNode : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY()
public:
	UEdGraphNode_QuestSystemGraphNode* NodeTemplate;

	FQuestSystemEdGraphSchemaAction_NewNode() : NodeTemplate(nullptr) {}
	FQuestSystemEdGraphSchemaAction_NewNode(const FText& InNodeCategory, const FText& InMenuDesc, const FText& InToolTip, const int32 InGrouping)
	: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping), NodeTemplate(nullptr) {}

	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
};

UCLASS()
class QUESTSYSTEMEDITOR_API UAssetQuestSystemGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()
public:
	// Begin EdGraphSchema Interface

	// virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
	
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	
	virtual void GetContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
	
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
	virtual bool TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const override;
	
	// virtual const FPinConnectionResponse CanMergeNodes(const UEdGraphNode* A, const UEdGraphNode* B) const override;
	virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;
	// virtual int32 GetNodeSelectionCount(const UEdGraph* Graph) const override;
	// virtual bool IsCacheVisualizationOutOfDate(int32 InVisualizationCacheID) const override;
	// virtual int32 GetCurrentVisualizationCacheID() const override;
	// virtual void ForceVisualizationCacheClear() const override;
	// virtual TSharedPtr<FEdGraphSchemaAction> GetCreateCommentAction() const override;
};