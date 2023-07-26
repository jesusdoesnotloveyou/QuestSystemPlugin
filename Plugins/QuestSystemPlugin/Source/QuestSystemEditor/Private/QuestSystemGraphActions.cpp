// Quest System by JDNLY. All Rights Reserved

#include "QuestSystemGraphActions.h"
#include "QuestSystemGraph.h"
#include "QuestSystemGraphAssetEditor/AssetEditor_QuestSystemGraph.h"

class UQuestSystemGraph;

// FQuestSystemGraphActions::FQuestSystemGraphActions(EAssetTypeCategories::Type InAssetCategory)
// 	: QuestSystemAssetTypeCategory(InAssetCategory)
// {
// }

UClass* FQuestSystemGraphActions::GetSupportedClass() const
{
	return UQuestSystemGraph::StaticClass(); 
}

FText FQuestSystemGraphActions::GetName() const
{
	return INVTEXT("Quest System Graph");
}

FColor FQuestSystemGraphActions::GetTypeColor() const
{
	return FColor::Cyan;
}

uint32 FQuestSystemGraphActions::GetCategories()
{
	return EAssetTypeCategories::Gameplay;
}

void FQuestSystemGraphActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UQuestSystemGraph* Graph = Cast<UQuestSystemGraph>(*ObjIt))
		{
			TSharedRef<FAssetEditor_QuestSystemGraph> NewGraphEditor(new FAssetEditor_QuestSystemGraph());
			NewGraphEditor->InitQuestSystemAssetEditor(Mode, EditWithinLevelEditor, Graph);
		}
	}
	FAssetTypeActions_Base::OpenAssetEditor(InObjects, EditWithinLevelEditor);
}