// Quest System by JDNLY. All Rights Reserved

#include "QuestSystemGraphAssetEditor/AssetEditor_QuestSystemGraph.h"
#include "QuestSystemGraphAssetEditor/AssetQuestSystemGraphSchema.h"
#include "QuestSystemGraphAssetEditor/EdGraph_QuestSystemGraph.h"
#include "QuestSystemGraphAssetEditor/EdNodeEdge_QuestSystemGraphEdge.h"
#include "EdGraphUtilities.h"
#include "Framework/Commands/GenericCommands.h"
#include "GraphEditorActions.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Kismet2/BlueprintEditorUtils.h"

class FEditorStyle;

#define LOCTEXT_NAMESPACE "QuestSystemEditor"

const FName FAssetEditor_QuestSystemGraph::PropertiesTabId( TEXT ("QuestSystemGraph_Properties" ) );
const FName FAssetEditor_QuestSystemGraph::ViewportTabId( TEXT ("QuestSystemGraph_Viewport" ) );

FAssetEditor_QuestSystemGraph::FAssetEditor_QuestSystemGraph()
{
	EditorGraph = nullptr;
}

FAssetEditor_QuestSystemGraph::~FAssetEditor_QuestSystemGraph()
{
	
}

const FName FAssetEditor_QuestSystemGraph::QuestSystemGraphEditorAppIdentifier( TEXT("QuestSystemGraphEditorApp") );

void FAssetEditor_QuestSystemGraph::InitQuestSystemAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UQuestSystemGraph* GraphToEdit)
{
	EditorGraph = GraphToEdit;
	CreateEdGraph();

	FGenericCommands::Register(); // Usually done in StartupModule()
	FGraphEditorCommands::Register();
	
	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("QuestSystemEditor_Layout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
				->SetHideTabWell(true)
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Horizontal)
				->SetSizeCoefficient(0.2f)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.8f)
					->SetHideTabWell(false)
					->AddTab(ViewportTabId, ETabState::OpenedTab)
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.2f)
					->SetHideTabWell(false)
					->AddTab(PropertiesTabId, ETabState::OpenedTab)
				)
			)
		);

	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	
	//
	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, QuestSystemGraphEditorAppIdentifier, StandaloneDefaultLayout,
	                                     bCreateDefaultStandaloneMenu,
	                                     bCreateDefaultToolbar, EditorGraph);
}

void FAssetEditor_QuestSystemGraph::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_QuestSystemEditor", "Quest System Editor"));
	
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
	
	InTabManager->RegisterTabSpawner(ViewportTabId, FOnSpawnTab::CreateSP(this, &FAssetEditor_QuestSystemGraph::SpawnViewportTab))
		.SetDisplayName( LOCTEXT("ViewportTab", "Viewport") )
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Viewport"));
	
	InTabManager->RegisterTabSpawner(PropertiesTabId, FOnSpawnTab::CreateSP(this, &FAssetEditor_QuestSystemGraph::SpawnDetailsTab))
		.SetDisplayName( LOCTEXT("PropertiesTab", "Details") )
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FAssetEditor_QuestSystemGraph::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
	
	InTabManager->UnregisterTabSpawner(ViewportTabId);
	InTabManager->UnregisterTabSpawner(PropertiesTabId);
}

void FAssetEditor_QuestSystemGraph::AddReferencedObjects(class FReferenceCollector& Collector)
{
	
}

TSharedRef<SDockTab> FAssetEditor_QuestSystemGraph::SpawnDetailsTab(const FSpawnTabArgs& Args)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	const FDetailsViewArgs DetailsViewArgs(false, false, true, FDetailsViewArgs::HideNameArea, true, this);
	TSharedRef<IDetailsView> PropertyEditorRef = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	PropertyWidget = PropertyEditorRef;

	check(Args.GetTabId() == PropertiesTabId);
	return SNew(SDockTab)
		//.Label(FText::FromString("Details"))
		[
			PropertyWidget.ToSharedRef()
		];
}

TSharedRef<SDockTab> FAssetEditor_QuestSystemGraph::SpawnViewportTab(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == ViewportTabId);
	GraphViewportWidget = CreateViewportWidget();
	
	return SNew(SDockTab)
		.TabColorScale(GetTabColorScale())
		.TabRole(ETabRole::PanelTab)
		[
			GraphViewportWidget.ToSharedRef()
		];
}

TSharedRef<SGraphEditor> FAssetEditor_QuestSystemGraph::CreateViewportWidget()
{
	check(EditorGraph->EdGraph); // What's this for?
	FGraphAppearanceInfo GraphAppearanceInfo;
	// Allow to use LOCTEXT below
	GraphAppearanceInfo.CornerText = FText::FromString("Quest System Editor");

	CreateCommandList();

	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FAssetEditor_QuestSystemGraph::OnSelectedNodesChanged);
	InEvents.OnNodeDoubleClicked = FSingleNodeEvent::CreateSP(this, &FAssetEditor_QuestSystemGraph::OnNodeDoubleClicked);

	return SNew(SGraphEditor)
		.AdditionalCommands(GraphEditorCommands)
		.IsEditable(true)
		.Appearance(GraphAppearanceInfo)
		.GraphToEdit(EditorGraph->EdGraph)
		.GraphEvents(InEvents)
		.AutoExpandActionMenu(true)
		.ShowGraphStateOverlay(false);
}

void FAssetEditor_QuestSystemGraph::CreateEdGraph()
{
	if (!EditorGraph->EdGraph)
	{
		EditorGraph->EdGraph = CastChecked<UEdGraph_QuestSystemGraph>(FBlueprintEditorUtils::CreateNewGraph(EditorGraph, NAME_None, UEdGraph_QuestSystemGraph::StaticClass(), UAssetQuestSystemGraphSchema::StaticClass()));
		EditorGraph->EdGraph->bAllowDeletion = false;
	}
}

TSharedPtr<SGraphEditor> FAssetEditor_QuestSystemGraph::GetCurrentGraphEditor() const
{
	return GraphViewportWidget;
}

void FAssetEditor_QuestSystemGraph::CreateCommandList()
{
	if (GraphEditorCommands.IsValid()) // if list doesn't exist
	{
		return;
	}
	GraphEditorCommands = MakeShareable(new FUICommandList);
	{
		GraphEditorCommands->MapAction(FGenericCommands::Get().SelectAll,
			FExecuteAction::CreateSP(this, &FAssetEditor_QuestSystemGraph::SelectAllNodes),
			FCanExecuteAction::CreateSP(this, &FAssetEditor_QuestSystemGraph::CanSelectAllNodes)
			);

		GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
			FExecuteAction::CreateSP(this, &FAssetEditor_QuestSystemGraph::DeleteSelectedNodes),
			FCanExecuteAction::CreateSP(this, &FAssetEditor_QuestSystemGraph::CanDeleteNodes)
			);

		GraphEditorCommands->MapAction(FGenericCommands::Get().Copy,
			FExecuteAction::CreateSP(this, &FAssetEditor_QuestSystemGraph::CopySelectedNodes),
			FCanExecuteAction::CreateSP(this, &FAssetEditor_QuestSystemGraph::CanCopyNodes)
			);

		GraphEditorCommands->MapAction(FGenericCommands::Get().Paste,
			FExecuteAction::CreateSP(this, &FAssetEditor_QuestSystemGraph::PasteNodes),
			FCanExecuteAction::CreateSP(this, &FAssetEditor_QuestSystemGraph::CanPasteNodes)
			);

		GraphEditorCommands->MapAction(FGenericCommands::Get().Cut,
			FExecuteAction::CreateSP(this, &FAssetEditor_QuestSystemGraph::CutSelectedNodes),
			FCanExecuteAction::CreateSP(this, &FAssetEditor_QuestSystemGraph::CanCutNodes)
			);

		GraphEditorCommands->MapAction(FGenericCommands::Get().Duplicate,
			FExecuteAction::CreateSP(this, &FAssetEditor_QuestSystemGraph::DuplicateNodes),
			FCanExecuteAction::CreateSP(this, &FAssetEditor_QuestSystemGraph::CanDuplicateNodes)
			);
	}
}

void FAssetEditor_QuestSystemGraph::OnGraphChanged(const FEdGraphEditAction& Action)
{
}

FGraphPanelSelectionSet FAssetEditor_QuestSystemGraph::GetSelectedNodes() const
{
	FGraphPanelSelectionSet CurrentSelectionSet;
	const TSharedPtr<SGraphEditor> FocusedGraphEditor = GetCurrentGraphEditor();

	// TO DO: check the difference between IsValid and if()
	// Check the content in GraphEditor.h
	if (FocusedGraphEditor.IsValid())
	{
		CurrentSelectionSet = FocusedGraphEditor->GetSelectedNodes();
	}
	return CurrentSelectionSet;
}

bool FAssetEditor_QuestSystemGraph::CanSelectAllNodes() const
{
	return true;
}

void FAssetEditor_QuestSystemGraph::SelectAllNodes()
{
	const TSharedPtr<SGraphEditor> FocusedGraphEditor = GetCurrentGraphEditor();
	if (FocusedGraphEditor.IsValid())
	{
		FocusedGraphEditor->SelectAllNodes();
	}
}

bool FAssetEditor_QuestSystemGraph::CanDeleteNodes() const
{
	// If any of the nodes can be deleted then we should allow deleting
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		const UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanUserDeleteNode())
		{
			return true;
		}
	}
	return false;
}

void FAssetEditor_QuestSystemGraph::DeleteSelectedNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrentGraphEditor();
	if (!CurrentGraphEditor.IsValid()) return;

	const FScopedTransaction Transaction(FGenericCommands::Get().Delete->GetDescription());

	CurrentGraphEditor->GetCurrentGraph()->Modify();

	const FGraphPanelSelectionSet SelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		UEdGraphNode* EdNode = Cast<UEdGraphNode>(*NodeIt);
		if (!EdNode || !EdNode->CanUserDeleteNode()) continue;

		if (UEdGraphNode_QuestSystemGraphNode* QuestSystemEdNode = Cast<UEdGraphNode_QuestSystemGraphNode>(EdNode))
		{
			QuestSystemEdNode->Modify();

			const UEdGraphSchema* Schema = QuestSystemEdNode->GetSchema();
			if (Schema)
			{
				Schema->BreakNodeLinks(*QuestSystemEdNode);
			}
			QuestSystemEdNode->DestroyNode();
		}
		else
		{
			EdNode->Modify();
			EdNode->DestroyNode();
		}
	}
}

bool FAssetEditor_QuestSystemGraph::CanCopyNodes() const
{
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanDuplicateNode())
		{
			return true;
		}
	}
	return false;
}

void FAssetEditor_QuestSystemGraph::CopySelectedNodes()
{
	// Export the selected nodes and place the text on the clipboard
	FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();

	FString ExportedText;

	for (FGraphPanelSelectionSet::TIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (!Node)
		{
			SelectedIter.RemoveCurrent();
			continue;
		}

		if (UEdNodeEdge_QuestSystemGraphEdge* NodeEdge = Cast<UEdNodeEdge_QuestSystemGraphEdge>(*SelectedIter))
		{
			UEdGraphNode_QuestSystemGraphNode* StartNode = NodeEdge->GetStartNode();
			UEdGraphNode_QuestSystemGraphNode* EndNode = NodeEdge->GetEndNode();

			if (!SelectedNodes.Contains(StartNode) || !SelectedNodes.Contains(EndNode))
			{
				SelectedIter.RemoveCurrent();
				continue;
			}
		}
		Node->PrepareForCopying();
	}
	FEdGraphUtilities::ExportNodesToText(SelectedNodes, ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);
}

void FAssetEditor_QuestSystemGraph::PasteNodes()
{
	if (TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrentGraphEditor())
	{
		PasteNodesHere(CurrentGraphEditor->GetPasteLocation());
	}
}

void FAssetEditor_QuestSystemGraph::PasteNodesHere(const FVector2D& Location)
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrentGraphEditor();
	if (!CurrentGraphEditor.IsValid()) return;

	if (UEdGraph* EdGraph = CurrentGraphEditor->GetCurrentGraph())
	{
		const FScopedTransaction Transaction(FGenericCommands::Get().Paste->GetDescription());
		EdGraph->Modify();

		CurrentGraphEditor->ClearSelectionSet();

		FString TextToImport;
		FPlatformApplicationMisc::ClipboardPaste(TextToImport);

		TSet<UEdGraphNode*> PastedNodes;

		FVector2D AverageNodePosition(0.0f, 0.0f);

		for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
		{
			const UEdGraphNode* Node = *It;
			AverageNodePosition.X += Node->NodePosX;
			AverageNodePosition.Y += Node->NodePosY;
		}

		if (PastedNodes.Num() > 0)
		{
			const float InvNumNodes = 1.0f / static_cast<float>(PastedNodes.Num());
			AverageNodePosition.X *= InvNumNodes;
			AverageNodePosition.Y *= InvNumNodes;
		}

		for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
		{
			UEdGraphNode* Node = *It;
			CurrentGraphEditor->SetNodeSelection(Node, true);

			Node->NodePosX = (Node->NodePosX - AverageNodePosition.X) + Location.X;
			Node->NodePosY = (Node->NodePosY - AverageNodePosition.Y) + Location.Y;

			Node->SnapToGrid(16);

			// Give new node a different Guid from the old one
			Node->CreateNewGuid();
		}
		// UpdateUI
		CurrentGraphEditor->NotifyGraphChanged();

		if (UObject* GraphOwner = EdGraph->GetOuter())
		{
			GraphOwner->PostEditChange();
			GraphOwner->MarkPackageDirty();
		}
	}
}

bool FAssetEditor_QuestSystemGraph::CanPasteNodes() const
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrentGraphEditor();
	if (!CurrentGraphEditor.IsValid()) return false;

	FString ClipboardContent;
	FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);

	return FEdGraphUtilities::CanImportNodesFromText(CurrentGraphEditor->GetCurrentGraph(), ClipboardContent);
}

void FAssetEditor_QuestSystemGraph::CutSelectedNodes()
{
	CopySelectedNodes();
	DeleteSelectedDuplicatableNodes();
}

bool FAssetEditor_QuestSystemGraph::CanCutNodes() const
{
	return CanCopyNodes() && CanDeleteNodes();
}

bool FAssetEditor_QuestSystemGraph::CanDuplicateNodes() const
{
	return CanCopyNodes();;
}

void FAssetEditor_QuestSystemGraph::DuplicateNodes()
{
	CopySelectedNodes();
	PasteNodes();
}

void FAssetEditor_QuestSystemGraph::DeleteSelectedDuplicatableNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrentGraphEditor();
	if (!CurrentGraphEditor.IsValid()) return;

	const FGraphPanelSelectionSet OldSelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanDuplicateNode())
		{
			CurrentGraphEditor->SetNodeSelection(Node, true);
		}
	}
	// Delete the duplicatable nodes
	DeleteSelectedNodes();
	
	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter))
		{
			CurrentGraphEditor->SetNodeSelection(Node, true);
		}
	}
}

void FAssetEditor_QuestSystemGraph::OnSelectedNodesChanged(const TSet<UObject*>& NewSelection)
{
	
}

void FAssetEditor_QuestSystemGraph::OnNodeDoubleClicked(UEdGraphNode* Node)
{
}

void FAssetEditor_QuestSystemGraph::ShowMessage()
{
}

FName FAssetEditor_QuestSystemGraph::GetToolkitFName() const
{
	return FName("FQuestSystemGraphEditor");
}

FText FAssetEditor_QuestSystemGraph::GetBaseToolkitName() const
{
	return FText::FromString("QuestSystemGraph EditorToolkitName");
}

FString FAssetEditor_QuestSystemGraph::GetWorldCentricTabPrefix() const
{
	return FString("QuestSystemGraph Editor");
}

FLinearColor FAssetEditor_QuestSystemGraph::GetWorldCentricTabColorScale() const
{
	return FLinearColor::White;
}

#undef LOCTEXT_NAMESPACE