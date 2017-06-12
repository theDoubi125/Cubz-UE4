#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "ChunkMesh.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(ChunkLog, Log, All);

struct FMeshData
{
	TArray<FVector> Vertices;
	TMap<FVector, uint32> VerticesIndex;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FLinearColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;
};

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dimension")
		uint8 Width;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dimension")
		uint8 Height;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dimension")
		uint8 Depth;

private:
	UPROPERTY()
	TArray<uint8> Data;
};
