#include "ChunkMesh.h"
#include "PlatformFile.h"
#include "PlatformFilemanager.h"

DEFINE_LOG_CATEGORY(ChunkLog);

AChunkMesh::AChunkMesh()
{
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh"));
	UpdateMesh();
	RootComponent = Mesh;
}

void AChunkMesh::BeginPlay()
{
	LoadFromFile(File);
	UpdateMesh();
}

void AChunkMesh::LoadFromFile(const FString &File)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	IFileHandle* FileHandle = PlatformFile.OpenRead(*File);

	UE_LOG(ChunkLog, Log, TEXT("Loading chunk file %s"), *File);
	
	if (FileHandle)
	{
		FileHandle->Read(Dimensions, 3);
		int32 Count = Dimensions[0] * Dimensions[1] * Dimensions[2];
		Data.Reserve(Count);
		FileHandle->Read(Data.GetData(), Count);
		UE_LOG(ChunkLog, Log, TEXT("Chunk %s loaded"), *File);
		delete FileHandle;
	}
	else
		UE_LOG(ChunkLog, Log, TEXT("Error while loading chunk %s : file not found"), *File);
}

void AChunkMesh::SaveToFile(const FString &File) const
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	IFileHandle* FileHandle = PlatformFile.OpenWrite(*File);
	if (FileHandle)
	{
		FileHandle->Write(Dimensions, 3);
		int32 Count = Dimensions[0] * Dimensions[1] * Dimensions[2];
		FileHandle->Write(Data.GetData(), Count);
		delete FileHandle;
	}
}

uint8 AChunkMesh::GetCellContent(uint8 x, uint8 y, uint8 z) const
{
	if (x < 0 || y < 0 || z < 0 || x >= Dimensions[0] || y >= Dimensions[1] || z >= Dimensions[2])
		return 0;
	else return Data[x + y * Dimensions[0] + z * Dimensions[0] * Dimensions[1]];
}

void AChunkMesh::UpdateMesh()
{
	TArray<FVector> Vertices;
	for (uint8 i = 0; i < Dimensions[0]; i++)
	{
		for (uint8 j = 0; j < Dimensions[1]; j++)
		{
			for (uint8 k = 0; k < Dimensions[2]; k++)
			{
				uint8 Cell = GetCellContent(i, j, k);
				if (Cell > 0)
				{
					// TODO
					for (int8 x = 0; x <= 8; x++)
					{

					}
				}
			}
		}
	}
	Vertices.Add(FVector(0, 0, 0));
	Vertices.Add(FVector(100, 0, 0));
	Vertices.Add(FVector(100, 100, 0));
	Vertices.Add(FVector(0, 100, 0));
	TArray<int32> Triangles;
	Triangles.Add(0);
	Triangles.Add(2);
	Triangles.Add(1);
	Triangles.Add(0);
	Triangles.Add(3);
	Triangles.Add(2);

	TArray<FVector> Normals;
	Normals.Add(FVector(0, 0, 1));
	Normals.Add(FVector(0, 0, 1));
	Normals.Add(FVector(0, 0, 1));
	Normals.Add(FVector(0, 0, 1));
	Normals.Add(FVector(0, 0, 1));
	Normals.Add(FVector(0, 0, 1));

	TArray<FVector2D> UV0;
	UV0.Add(FVector2D(0, 0));
	UV0.Add(FVector2D(0, 0));
	UV0.Add(FVector2D(0, 0));
	UV0.Add(FVector2D(0, 0));
	UV0.Add(FVector2D(0, 0));
	UV0.Add(FVector2D(0, 0));

	TArray<FLinearColor> VertexColors;
	VertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	VertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	VertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	VertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	VertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	VertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));

	TArray<FProcMeshTangent> Tangents;
	Tangents.Add(FProcMeshTangent(0, 1, 0));
	Tangents.Add(FProcMeshTangent(0, 1, 0));
	Tangents.Add(FProcMeshTangent(0, 1, 0));
	Tangents.Add(FProcMeshTangent(0, 1, 0));
	Tangents.Add(FProcMeshTangent(0, 1, 0));
	Tangents.Add(FProcMeshTangent(0, 1, 0));

	Mesh->CreateMeshSection_LinearColor(1, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, false);
}