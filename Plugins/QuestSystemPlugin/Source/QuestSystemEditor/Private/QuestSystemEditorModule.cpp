// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuestSystemEditorModule.h"
#include "QuestSystemGraphNodeFactory.h"

#define LOCTEXT_NAMESPACE "FQuestSystemEditorModule"

void FQuestSystemEditorModule::StartupModule()
{
	QuestSystemGraphActions = MakeShared<FQuestSystemGraphActions>();
	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(QuestSystemGraphActions.ToSharedRef());
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	QuestSystemGraphNodeFactory = MakeShareable(new FQuestSystemGraphNodeFactory());
	FEdGraphUtilities::RegisterVisualNodeFactory(QuestSystemGraphNodeFactory);

	QuestSystemGraphNodePinFactory = MakeShareable(new FQuestSystemGraphNodePinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(QuestSystemGraphNodePinFactory);
}

void FQuestSystemEditorModule::ShutdownModule()
{
	if (!FModuleManager::Get().IsModuleLoaded("AssetTools")) return;
		FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(QuestSystemGraphActions.ToSharedRef());
	
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FEdGraphUtilities::UnregisterVisualNodeFactory(QuestSystemGraphNodeFactory);
	FEdGraphUtilities::RegisterVisualPinFactory(QuestSystemGraphNodePinFactory);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FQuestSystemEditorModule, QuestSystemPlugin)