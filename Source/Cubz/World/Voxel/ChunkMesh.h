#pragma once

#include "CoreMinimal.h"
#include "ChunkMesh.generated.h"

UCLASS()
class AChunkMesh : public AActor
{
	GENERATED_BODY()
public:
	AChunkMesh();
private:
	TArray<uint8> Data;
	uint8 Width, Height;
};
