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
		uint8 GetCellContent(int32 x, int32 y, int32 z) const;
	
	UFUNCTION(BlueprintCallable, Category = "Chunk")
		void SetCellContent(int32 x, int32 y, int32 z, uint8 value);

	void UpdateMesh();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "File")
		FString File;

	UPROPERTY(EditAnywhere)
	UProceduralMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dimension")
		int32 Width;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dimension")
		int32 Height;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dimension")
		int32 Depth;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dimension")
		int32 MaxFaces;

private:
	UPROPERTY()
	TArray<uint8> Data;
};
