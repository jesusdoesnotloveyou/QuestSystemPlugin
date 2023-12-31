// Quest System by JDNLY. All Rights Reserved

#include "QuestSystemGraphAssetEditor/AssetQuestSystemGraphSchema.h"

#include "GraphEditorActions.h"
#include "QuestSystemGraph.h"
#include "QuestSystemGraphNode.h"
#include "ToolMenus.h"
#include "QuestSystemGraphAssetEditor/EdGraphNode_QuestSystemGraphNode.h"
#include "QuestSystemGraphAssetEditor/EdGraph_QuestSystemGraph.h"
#include "Framework/Commands/GenericCommands.h"

#define LOCTEXT_NAMESPACE "AssetSchema_QuestSystemEditor"

class FNodeVisitorCycleChecker
{
public:
	/** Check whether a loop in the graph would be caused by linking the passed-in nodes */
	bool CheckForLoop(UEdGraphNode* StartNode, UEdGraphNode* EndNode)
	{
		VisitedNodes.Add(StartNode);
		return TraverseNodes(EndNode);
	}

private:
	bool TraverseNodes(UEdGraphNode* Node)
	{
		VisitedNodes.Add(Node);

		for (auto Pin : Node->Pins)
		{
			if (Pin->Direction == EGPD_Output)
			{
				for (auto OtherPin : Pin->LinkedTo)
				{
					UEdGraphNode* OtherNode = OtherPin->GetOwningNode();
					if (VisitedNodes.Contains(OtherNode))
					{
						// Only  an issue if this is a back-edge
						return false;
					}
					else if (!FinishedNodes.Contains(OtherNode))
					{
						// Only should traverse if this node hasn't been traversed
						if (!TraverseNodes(OtherNode)) return false;
					}
				}
			}
		}

		VisitedNodes.Remove(Node);
		FinishedNodes.Add(Node);
		return true;
	};
	
	TSet<UEdGraphNode*> VisitedNodes;
	TSet<UEdGraphNode*> FinishedNodes;
};

void FQuestSystemEdGraphSchemaAction_NewNode::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdGraphSchemaAction::AddReferencedObjects(Collector);
	Collector.AddReferencedObject(NodeTemplate);
}

UEdGraphNode* FQuestSystemEdGraphSchemaAction_NewNode::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	UEdGraphNode* ResultNode = nullptr;
	if (NodeTemplate != nullptr)
    {
    	const FScopedTransaction Transaction(LOCTEXT("QuestSystemEditorNewNode", "Quest System Editor: New Node"));
    	ParentGraph->Modify();
    	if (FromPin != nullptr) FromPin->Modify();

    	NodeTemplate->Rename(nullptr, ParentGraph);
    	ParentGraph->AddNode(NodeTemplate, true, bSelectNewNode);

    	NodeTemplate->CreateNewGuid();
    	NodeTemplate->PostPlacedNewNode();
    	NodeTemplate->AllocateDefaultPins();
    	NodeTemplate->AutowireNewNode(FromPin);

    	NodeTemplate->NodePosX = Location.X;
    	NodeTemplate->NodePosY = Location.Y;

    	NodeTemplate->QuestSystemGraphNode->SetFlags(RF_Transactional);
    	NodeTemplate->SetFlags(RF_Transactional);

    	ResultNode = NodeTemplate;
    }
    	return ResultNode;
}

void UAssetQuestSystemGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	UQuestSystemGraph* Graph = CastChecked<UQuestSystemGraph>(ContextMenuBuilder.CurrentGraph->GetOuter());
	if (Graph->NodeType == nullptr)
	{
		return;
	}

	const bool bNoParent = (ContextMenuBuilder.FromPin == NULL);

	const FText AddToolTip = LOCTEXT("NewQuestSystemGraphNodeTooltip", "Add node here");

	TSet<TSubclassOf<UQuestSystemGraphNode>> Visited;

	FText Desc = Graph->NodeType.GetDefaultObject()->ContextMenuName;

	if (Desc.IsEmpty())
	{
		FString Title = Graph->NodeType->GetName();
		Title.RemoveFromEnd("_C");
		Desc = FText::FromString(Title);
	}

	if (!Graph->NodeType->HasAnyClassFlags(CLASS_Abstract))
	{
		TSharedPtr<FQuestSystemEdGraphSchemaAction_NewNode> NewNodeAction(new FQuestSystemEdGraphSchemaAction_NewNode(LOCTEXT("QuestSystemGraphNodeAction", "Quest System Graph Node"), Desc, AddToolTip, 0));
		NewNodeAction->NodeTemplate = NewObject<UEdGraphNode_QuestSystemGraphNode>(ContextMenuBuilder.OwnerOfTemporaries);
		NewNodeAction->NodeTemplate->QuestSystemGraphNode = NewObject<UQuestSystemGraphNode>(NewNodeAction->NodeTemplate, Graph->NodeType);
		NewNodeAction->NodeTemplate->QuestSystemGraphNode->Graph = Graph;
		ContextMenuBuilder.AddAction(NewNodeAction);

		Visited.Add(Graph->NodeType);
	}

	for (TObjectIterator<UClass> It; It; ++It)
	{
		if (It->IsChildOf(Graph->NodeType) && !It->HasAnyClassFlags(CLASS_Abstract) && !Visited.Contains(*It))
		{
			TSubclassOf<UQuestSystemGraphNode> NodeType = *It;

			if (It->GetName().StartsWith("REINST") || It->GetName().StartsWith("SKEL"))
				continue;

			if (!Graph->GetClass()->IsChildOf(NodeType.GetDefaultObject()->CompatibleGraphType))
				continue;

			Desc = NodeType.GetDefaultObject()->ContextMenuName;

			if (Desc.IsEmpty())
			{
				FString Title = NodeType->GetName();
				Title.RemoveFromEnd("_C");
				Desc = FText::FromString(Title);
			}

			TSharedPtr<FQuestSystemEdGraphSchemaAction_NewNode> Action(new FQuestSystemEdGraphSchemaAction_NewNode(LOCTEXT("QuestSystemGraphNodeAction", "Quest System Graph Node"), Desc, AddToolTip, 0));
			Action->NodeTemplate = NewObject<UEdGraphNode_QuestSystemGraphNode>(ContextMenuBuilder.OwnerOfTemporaries);
			Action->NodeTemplate->QuestSystemGraphNode = NewObject<UQuestSystemGraphNode>(Action->NodeTemplate, NodeType);
			Action->NodeTemplate->QuestSystemGraphNode->Graph = Graph;
			ContextMenuBuilder.AddAction(Action);

			Visited.Add(NodeType);
		}
	}
}

void UAssetQuestSystemGraphSchema::GetContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const
{
	// if (Context->Pin)
	// {
	// 	{
	// 		FToolMenuSection& Section = Menu->AddSection("GenericGraphAssetGraphSchemaNodeActions", LOCTEXT("PinActionsMenuHeader", "Pin Actions"));
	// 		// Only display the 'Break Links' option if there is a link to break!
	// 		if (Context->Pin->LinkedTo.Num() > 0)
	// 		{
	// 			Section.AddMenuEntry(FGraphEditorCommands::Get().BreakPinLinks);
	//
	// 			// add sub menu for break link to
	// 			if (Context->Pin->LinkedTo.Num() > 1)
	// 			{
	// 				Section.AddSubMenu(
	// 					"BreakLinkTo",
	// 					LOCTEXT("BreakLinkTo", "Break Link To..."),
	// 					LOCTEXT("BreakSpecificLinks", "Break a specific link..."),
	// 					FNewToolMenuDelegate::CreateUObject((UAssetQuestSystemGraphSchema* const)this, &UAssetQuestSystemGraphSchema::GetBreakLinkToSubMenuActions, const_cast<UEdGraphPin*>(Context->Pin)));
	// 			}
	// 			else
	// 			{
	// 				((UAssetQuestSystemGraphSchema* const)this)->GetBreakLinkToSubMenuActions(Menu, const_cast<UEdGraphPin*>(Context->Pin));
	// 			}
	// 		}
	// 	}
	// }
	//else if (Context->Node)
	if (Context->Node /*&& !Context->Pin*/)
	{
		{
			FToolMenuSection& Section = Menu->AddSection("QuestSystemGraphSchemaNodeActions", LOCTEXT("ClassActionsMenuHeader", "Node Actions"));
			Section.AddMenuEntry(FGenericCommands::Get().Delete);
			Section.AddMenuEntry(FGenericCommands::Get().Cut);
			Section.AddMenuEntry(FGenericCommands::Get().Copy);
			Section.AddMenuEntry(FGenericCommands::Get().Duplicate);
			
			Section.AddMenuEntry(FGraphEditorCommands::Get().BreakNodeLinks);
		}
	}
	Super::GetContextMenuActions(Menu, Context);
}


const FPinConnectionResponse UAssetQuestSystemGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
    UEdGraphNode_QuestSystemGraphNode* OutEdNode = Cast<UEdGraphNode_QuestSystemGraphNode>(A->GetOwningNode());
    UEdGraphNode_QuestSystemGraphNode* InEdNode = Cast<UEdGraphNode_QuestSystemGraphNode>(B->GetOwningNode());

	if (OutEdNode == InEdNode)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorSameNode", "Cannot connect node to itself"));
	}
	
    if (!OutEdNode || !InEdNode)
    {
    	return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinError", "Not valid UEdGraphNode"));
    }
    	
    //Determine if we can have cycles or not
    bool bAllowCycles = false;
    const auto EdGraph = Cast<UEdGraph_QuestSystemGraph>(OutEdNode->GetGraph());
    if (EdGraph)
    {
    	bAllowCycles = EdGraph->GetGenericGraph()->bCanBeCyclical;
    }

    // Check for cycles
    FNodeVisitorCycleChecker CycleChecker;
    if (!bAllowCycles && !CycleChecker.CheckForLoop(OutEdNode, InEdNode))
    {
    	return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorCycle", "Can't create a graph cycle"));
    }

    FText ErrorMessage;
    if (!OutEdNode->QuestSystemGraphNode->CanCreateConnectionTo(InEdNode->QuestSystemGraphNode, OutEdNode->GetOutputPin()->LinkedTo.Num(), ErrorMessage))
    {
    	return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, ErrorMessage);
    }
    if (!InEdNode->QuestSystemGraphNode->CanCreateConnectionFrom(OutEdNode->QuestSystemGraphNode, InEdNode->GetInputPin()->LinkedTo.Num(), ErrorMessage))
    {
    	return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, ErrorMessage);
    }

    if (OutEdNode->QuestSystemGraphNode->GetGraph()->bEdgeEnabled)
    {
    	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE_WITH_CONVERSION_NODE, LOCTEXT("PinConnect", "Connect nodes with edge"));
    }
    else
    {
    	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, LOCTEXT("PinConnect", "Connect nodes"));
    }
}

bool UAssetQuestSystemGraphSchema::TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const
{
	return true;
}

FLinearColor UAssetQuestSystemGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	return FLinearColor::White;
}

#undef LOCTEXT_NAMESPACE