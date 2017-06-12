#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "ChunkMesh.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(ChunkLog, Log, All);

UCLASS()
class AChunkMesh : public AActor
{
	GENERATED_BODY()
public:
	AChunkMesh();

	virtual void BeginPlay() override;

	void LoadFromFile(const FString& File);
	void SaveToFile(const FString& File) const;

	UFUNCTION(BlueprintCallable, Category = "Chunk")
		uint8 GetCellContent(uint8 x, uint8 y, uint8 z) const;

	void UpdateMesh();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "File")
		FString File;

	UPROPERTY(EditAnywhere)
	UProceduralMeshComponent* Mesh;

private:
	TArray<uint8> Data;
	uint8 Dimensions[3];
};
