#include "Environment/SFTerrainManager.h"
#include "Landscape.h"

ASFTerrainManager::ASFTerrainManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

}
//
//void ASFTerrainManager::GenerateTerrain(FTerrainSettings Settings)
//{
//	////ULandscapeEditorObject* UISettings = LandscapeEdMode->UISettings;
//	const int32 ComponentCountX = Settings.ComponentCount.X;
//	const int32 ComponentCountY = Settings.ComponentCount.Y;
//	const int32 QuadsPerComponent = Settings.SectionsPerComponent * Settings.QuadsPerSection;
//	const int32 SizeX = ComponentCountX * QuadsPerComponent + 1;
//	const int32 SizeY = ComponentCountY * QuadsPerComponent + 1;
//
//	TOptional< TArray< FLandscapeImportLayerInfo > > MaterialImportLayers = FNewLandscapeUtils::CreateImportLayersInfo(UISettings, LandscapeEdMode->NewLandscapePreviewMode);
//
//	////if (!MaterialImportLayers)
//	////{
//	////	return FReply::Handled();
//	////}
//
//	TMap<FGuid, TArray<uint16>> HeightDataPerLayers;
//	TMap<FGuid, TArray<FLandscapeImportLayerInfo>> MaterialLayerDataPerLayers;
//
//	////HeightDataPerLayers.Add(FGuid(), FNewLandscapeUtils::ComputeHeightData(UISettings, MaterialImportLayers.GetValue(), LandscapeEdMode->NewLandscapePreviewMode));
//	////// ComputeHeightData will also modify/expand material layers data, which is why we create MaterialLayerDataPerLayers after calling ComputeHeightData
//	////MaterialLayerDataPerLayers.Add(FGuid(), MaterialImportLayers.GetValue());
//
//	////FScopedTransaction Transaction(LOCTEXT("Undo", "Creating New Landscape"));
//
//	//const FVector Offset = FTransform(UISettings->NewLandscape_Rotation, FVector::ZeroVector, UISettings->NewLandscape_Scale).TransformVector(FVector(-ComponentCountX * QuadsPerComponent / 2, -ComponentCountY * QuadsPerComponent / 2, 0));
//
//	ALandscape* Landscape = GetWorld()->SpawnActor<ALandscape>(Settings.Location /*+ Offset*/, Settings.Rotation);
//	Landscape->bCanHaveLayersContent = Settings.bCanHaveLayersContent;
//	//Landscape->LandscapeMaterial = UISettings->NewLandscape_Material.Get();
//	Landscape->SetActorRelativeScale3D(Settings.Scale);
//
//	//// automatically calculate a lighting LOD that won't crash lightmass (hopefully)
//	//// < 2048x2048 -> LOD0
//	//// >=2048x2048 -> LOD1
//	//// >= 4096x4096 -> LOD2
//	//// >= 8192x8192 -> LOD3
//	//Landscape->StaticLightingLOD = FMath::DivideAndRoundUp(FMath::CeilLogTwo((SizeX * SizeY) / (2048 * 2048) + 1), (uint32)2);
//
//	//if (LandscapeEdMode->NewLandscapePreviewMode == ENewLandscapePreviewMode::ImportLandscape)
//	//{
//	if (!Settings.HeightmapFilename.IsEmpty())
//	{
//		Landscape->ReimportHeightmapFilePath = Settings.HeightmapFilename; //UISettings->ImportLandscape_HeightmapFilename;
//	}
//
//	Landscape->Import(FGuid::NewGuid(), 0, 0, SizeX - 1, SizeY - 1, Settings.SectionsPerComponent, Settings.QuadsPerSection, HeightDataPerLayers, nullptr, MaterialLayerDataPerLayers, UISettings->ImportLandscape_AlphamapType);
//
//	//ULandscapeInfo* LandscapeInfo = Landscape->GetLandscapeInfo();
//	//check(LandscapeInfo);
//
//	//LandscapeInfo->UpdateLayerInfoMap(Landscape);
//
//	//// Import doesn't fill in the LayerInfo for layers with no data, do that now
//	//const TArray<FLandscapeImportLayer>& ImportLandscapeLayersList = UISettings->ImportLandscape_Layers;
//	//for (int32 i = 0; i < ImportLandscapeLayersList.Num(); i++)
//	//{
//	//	if (ImportLandscapeLayersList[i].LayerInfo != nullptr)
//	//	{
//	//		if (LandscapeEdMode->NewLandscapePreviewMode == ENewLandscapePreviewMode::ImportLandscape)
//	//		{
//	//			Landscape->EditorLayerSettings.Add(FLandscapeEditorLayerSettings(ImportLandscapeLayersList[i].LayerInfo, ImportLandscapeLayersList[i].SourceFilePath));
//	//		}
//	//		else
//	//		{
//	//			Landscape->EditorLayerSettings.Add(FLandscapeEditorLayerSettings(ImportLandscapeLayersList[i].LayerInfo));
//	//		}
//
//	//		int32 LayerInfoIndex = LandscapeInfo->GetLayerInfoIndex(ImportLandscapeLayersList[i].LayerName);
//	//		if (ensure(LayerInfoIndex != INDEX_NONE))
//	//		{
//	//			FLandscapeInfoLayerSettings& LayerSettings = LandscapeInfo->Layers[LayerInfoIndex];
//	//			LayerSettings.LayerInfoObj = ImportLandscapeLayersList[i].LayerInfo;
//	//		}
//	//	}
//	//}
//
//	//LandscapeEdMode->UpdateLandscapeList();
//	//LandscapeEdMode->SetLandscapeInfo(LandscapeInfo);
//	//LandscapeEdMode->CurrentToolTarget.TargetType = ELandscapeToolTargetType::Heightmap;
//	//LandscapeEdMode->SetCurrentTargetLayer(NAME_None, nullptr);
//	//LandscapeEdMode->SetCurrentTool("Select"); // change tool so switching back to the manage mode doesn't give "New Landscape" again
//	//LandscapeEdMode->SetCurrentTool("Sculpt"); // change to sculpting mode and tool
//	//LandscapeEdMode->SetCurrentLayer(0);
//
//	//if (LandscapeEdMode->CurrentToolTarget.LandscapeInfo.IsValid())
//	//{
//	//	ALandscapeProxy* LandscapeProxy = LandscapeEdMode->CurrentToolTarget.LandscapeInfo->GetLandscapeProxy();
//	//	LandscapeProxy->OnMaterialChangedDelegate().AddRaw(LandscapeEdMode, &FEdModeLandscape::OnLandscapeMaterialChangedDelegate);
//	//}
//}
//
