// Quest System by JDNLY. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemGraph.h"
#include "Tools/BaseAssetToolkit.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "GraphEditor.h"

class QUESTSYSTEMEDITOR_API FAssetEditor_QuestSystemGraph : public FAssetEditorToolkit, public FNotifyHook, public FGCObject
{
public:
	FAssetEditor_QuestSystemGraph();
	virtual ~FAssetEditor_QuestSystemGraph() override;

	void InitQuestSystemAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UQuestSystemGraph* GraphToEdit);

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

	// Inherited from FGCObject
	virtual void AddReferencedObjects(class FReferenceCollector& Collector) override;
	
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	// virtual FText GetToolkitName() const override;
	// virtual FText GetToolkitToolTipText() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	//virtual FString GetDocumentationLink() const override;
	//virtual void SaveAsset_Execute() override;
	
	TSharedRef<SDockTab> SpawnViewportTab(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnDetailsTab(const FSpawnTabArgs& Args);
	//TSharedRef<SDockTab> SpawnEditorSettingsTab(const FSpawnTabArgs& Args);

	TSharedRef<SGraphEditor> CreateViewportWidget();

	void CreateEdGraph();

private:
	/** Dockable tab for properties. Might be unnecessary */
	TSharedPtr<class SDockableTab> PropertiesTab;

	/**	The tab ids for all the tabs used */
	static const FName ViewportTabId;
	
	/**	The tab ids for all the tabs used */
	static const FName PropertiesTabId;
	
	/** App Identifier. Technically, all simple editors are the same app, despite editing a variety of assets. */
	static const FName QuestSystemGraphEditorAppIdentifier;

protected:
	void OnGraphChanged(const FEdGraphEditAction& Action);

	FGraphPanelSelectionSet GetSelectedNodes() const;
	
	bool CanSelectAllNodes() const;
	void SelectAllNodes();
	
	bool CanDeleteNodes() const;
	void DeleteSelectedNodes();
	
	bool CanCopyNodes() const;
	void CopySelectedNodes();
	
	bool CanPasteNodes() const;
	void PasteNodes();
	void PasteNodesHere(const FVector2D& Location);

	bool CanCutNodes() const;
	void CutSelectedNodes();

	bool CanDuplicateNodes() const;
	void DuplicateNodes();
	void DeleteSelectedDuplicatableNodes();
	
	/** Called when the selection changes in the GraphEditor */
	void OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection);

	/** Called when a node is double clicked */
	void OnNodeDoubleClicked(class UEdGraphNode* Node);

	void ShowMessage();

protected:
	UQuestSystemGraph* EditorGraph;
	
	TSharedPtr<SGraphEditor> GraphViewportWidget;
	TSharedPtr<IDetailsView> PropertyWidget;

	/** The command list for this editor */
	TSharedPtr<class FUICommandList> GraphEditorCommands;
	
	TSharedPtr<SGraphEditor> GetCurrentGraphEditor() const;
	void CreateCommandList();
};
