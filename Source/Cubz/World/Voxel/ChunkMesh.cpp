#include "ChunkMesh.h"
#include "PlatformFile.h"
#include "PlatformFilemanager.h"

DEFINE_LOG_CATEGORY(ChunkLog);

AChunkMesh::AChunkMesh()
{
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh"));
	UpdateMesh();
	RootComponent = Mesh;
	Width = 10;
	Height = 10;
	Depth = 10;
}

void AChunkMesh::BeginPlay()
{
	LoadFromFile(File);
	Data.Reserve(Width * Height * Depth);
	Data.AddZeroed(Width * Height * Depth);
	UpdateMesh();
}

void AChunkMesh::LoadFromFile(const FString &File)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	IFileHandle* FileHandle = PlatformFile.OpenRead(*File);

	UE_LOG(ChunkLog, Log, TEXT("Loading chunk file %s"), *File);
	
	if (FileHandle)
	{
		FileHandle->Read(&Width, 1);
		FileHandle->Read(&Height, 1);
		FileHandle->Read(&Depth, 1);
		int32 Count = Width * Height * Depth;
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
		FileHandle->Write(&Width, 1);
		FileHandle->Write(&Height, 1);
		FileHandle->Write(&Depth, 1);
		int32 Count = Width * Height * Depth;
		FileHandle->Write(Data.GetData(), Count);
		delete FileHandle;
	}
}

uint8 AChunkMesh::GetCellContent(uint8 x, uint8 y, uint8 z) const
{
	if (x < 0 || y < 0 || z < 0 || x >= Width || y >= Depth || z >= Height)
		return 0;
	else return Data[x + y * Width + z * Width * Depth];
}

/** /!\ Direction must be normalized */
void AddFaceToMesh(FMeshData &MeshData, FVector CellCenter, FVector Direction, float HalfCellSize)
{
	ensure(Direction.IsUnit());
	FVector X = FVector::CrossProduct(Direction, FVector(0, 0, 1));
	if (X.IsZero())
		X = FVector::CrossProduct(Direction, FVector(0, 1, 0));
	FVector Y = FVector::CrossProduct(Direction, X);

	FVector A = CellCenter + (Direction + X + Y) * HalfCellSize;
	FVector B = CellCenter + (Direction - X + Y) * HalfCellSize;
	FVector C = CellCenter + (Direction + X - Y) * HalfCellSize;
	FVector D = CellCenter + (Direction - X - Y) * HalfCellSize;

	int32 AIndex = -1, BIndex = -1, CIndex = -1, DIndex = -1;

	if (MeshData.VerticesIndex.Contains(A))
		AIndex = MeshData.VerticesIndex[A];
	else
	{
		AIndex = MeshData.Vertices.Add(A);
		MeshData.VerticesIndex[A] = AIndex;
	}

	if (MeshData.VerticesIndex.Contains(B))
		BIndex = MeshData.VerticesIndex[B];
	else
	{
		AIndex = MeshData.Vertices.Add(B);
		MeshData.VerticesIndex[B] = BIndex;
	}

	if (MeshData.VerticesIndex.Contains(C))
		CIndex = MeshData.VerticesIndex[C];
	else
	{
		AIndex = MeshData.Vertices.Add(C);
		MeshData.VerticesIndex[C] = CIndex;
	}
	if (MeshData.VerticesIndex.Contains(D))
		DIndex = MeshData.VerticesIndex[D];
	else
	{
		AIndex = MeshData.Vertices.Add(D);
		MeshData.VerticesIndex[D] = DIndex;
	}

	MeshData.Triangles.Add(AIndex);
	MeshData.UV0.Add(FVector2D(1, 1));
	MeshData.Triangles.Add(BIndex);
	MeshData.UV0.Add(FVector2D(0, 1));
	MeshData.Triangles.Add(CIndex);
	MeshData.UV0.Add(FVector2D(1, 0));
	MeshData.Triangles.Add(CIndex);
	MeshData.UV0.Add(FVector2D(1, 0));
	MeshData.Triangles.Add(BIndex);
	MeshData.UV0.Add(FVector2D(0, 1));
	MeshData.Triangles.Add(DIndex);
	MeshData.UV0.Add(FVector2D(0, 0));

	for (int8 i = 0; i < 6; i++)
	{
		MeshData.Normals.Add(Direction);
		MeshData.Tangents.Add(FProcMeshTangent(X, false));
		MeshData.VertexColors.Add(FLinearColor(1, 1, 1, 1));
	}
}

void AChunkMesh::UpdateMesh()
{
	FMeshData MeshData;
	uint8 Directions[] = { 1, 0, 0,  0, 1, 0,  0, 0, 1,  -1, 0, 0,  0, -1, 0,  0, 0, -1 };
	for (uint8 x = 0; x < Width; x++)
	{
		for (uint8 y = 0; y < Depth; y++)
		{
			for (uint8 z = 0; z < Height; z++)
			{
				uint8 Cell = GetCellContent(x, y, z);
				if (Cell > 0)
				{
					for (int8 DirectionId = 0; DirectionId <= 6; DirectionId++)
					{
						uint32 NeighbourCellX = x + Directions[DirectionId * 3];
						uint32 NeighbourCellY = y + Directions[DirectionId * 3 + 1];
						uint32 NeighbourCellZ = z + Directions[DirectionId * 3 + 2];

						uint8 NeighbourCellContent = GetCellContent(NeighbourCellX, NeighbourCellY, NeighbourCellZ);
						if (NeighbourCellContent == 0)
						{
							// Draw face
							AddFaceToMesh(MeshData, FVector(NeighbourCellX, NeighbourCellY, NeighbourCellZ), FVector(Directions[DirectionId * 3], Directions[DirectionId * 3 + 1], Directions[DirectionId * 3 + 2]), 50);
						}
					}
				}
			}
		}
	}
	Mesh->CreateMeshSection_LinearColor(1, MeshData.Vertices, MeshData.Triangles, MeshData.Normals, MeshData.UV0, MeshData.VertexColors, MeshData.Tangents, false);
}