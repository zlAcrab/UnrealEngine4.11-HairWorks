// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
/*=============================================================================
	GeomFitUtils.h: Utilities for fitting collision models to static meshes.
=============================================================================*/

// k-DOP (k-Discrete Oriented Polytopes) Direction Vectors
#define RCP_SQRT2 (0.70710678118654752440084436210485f)
#define RCP_SQRT3 (0.57735026918962576450914878050196f)


const FVector KDopDir10X[10] = 
{
	FVector( 1.f, 0.f, 0.f),
	FVector(-1.f, 0.f, 0.f),
	FVector( 0.f, 1.f, 0.f),
	FVector( 0.f,-1.f, 0.f),
	FVector( 0.f, 0.f, 1.f),
	FVector( 0.f, 0.f,-1.f),
	FVector( 0.f, RCP_SQRT2,  RCP_SQRT2),
	FVector( 0.f,-RCP_SQRT2, -RCP_SQRT2),
	FVector( 0.f, RCP_SQRT2, -RCP_SQRT2),
	FVector( 0.f,-RCP_SQRT2,  RCP_SQRT2)
};

const FVector KDopDir10Y[10] = 
{
	FVector( 1.f, 0.f, 0.f),
	FVector(-1.f, 0.f, 0.f),
	FVector( 0.f, 1.f, 0.f),
	FVector( 0.f,-1.f, 0.f),
	FVector( 0.f, 0.f, 1.f),
	FVector( 0.f, 0.f,-1.f),
	FVector( RCP_SQRT2, 0.f,  RCP_SQRT2),
	FVector(-RCP_SQRT2, 0.f, -RCP_SQRT2),
	FVector( RCP_SQRT2, 0.f, -RCP_SQRT2),
	FVector(-RCP_SQRT2, 0.f,  RCP_SQRT2)
};

const FVector KDopDir10Z[10] = 
{
	FVector( 1.f, 0.f, 0.f),
	FVector(-1.f, 0.f, 0.f),
	FVector( 0.f, 1.f, 0.f),
	FVector( 0.f,-1.f, 0.f),
	FVector( 0.f, 0.f, 1.f),
	FVector( 0.f, 0.f,-1.f),
	FVector( RCP_SQRT2,  RCP_SQRT2, 0.f),
	FVector(-RCP_SQRT2, -RCP_SQRT2, 0.f),
	FVector( RCP_SQRT2, -RCP_SQRT2, 0.f),
	FVector(-RCP_SQRT2,  RCP_SQRT2, 0.f)
};

const FVector KDopDir18[18] = 
{
	FVector( 1.f, 0.f, 0.f),
	FVector(-1.f, 0.f, 0.f),
	FVector( 0.f, 1.f, 0.f),
	FVector( 0.f,-1.f, 0.f),
	FVector( 0.f, 0.f, 1.f),
	FVector( 0.f, 0.f,-1.f),
	FVector( 0.f, RCP_SQRT2,  RCP_SQRT2),
	FVector( 0.f,-RCP_SQRT2, -RCP_SQRT2),
	FVector( 0.f, RCP_SQRT2, -RCP_SQRT2),
	FVector( 0.f,-RCP_SQRT2,  RCP_SQRT2),
	FVector( RCP_SQRT2, 0.f,  RCP_SQRT2),
	FVector(-RCP_SQRT2, 0.f, -RCP_SQRT2),
	FVector( RCP_SQRT2, 0.f, -RCP_SQRT2),
	FVector(-RCP_SQRT2, 0.f,  RCP_SQRT2),
	FVector( RCP_SQRT2,  RCP_SQRT2, 0.f),
	FVector(-RCP_SQRT2, -RCP_SQRT2, 0.f),
	FVector( RCP_SQRT2, -RCP_SQRT2, 0.f),
	FVector(-RCP_SQRT2,  RCP_SQRT2, 0.f)
};

const FVector KDopDir26[26] = 
{
	FVector( 1.f, 0.f, 0.f),
	FVector(-1.f, 0.f, 0.f),
	FVector( 0.f, 1.f, 0.f),
	FVector( 0.f,-1.f, 0.f),
	FVector( 0.f, 0.f, 1.f),
	FVector( 0.f, 0.f,-1.f),
	FVector( 0.f, RCP_SQRT2,  RCP_SQRT2),
	FVector( 0.f,-RCP_SQRT2, -RCP_SQRT2),
	FVector( 0.f, RCP_SQRT2, -RCP_SQRT2),
	FVector( 0.f,-RCP_SQRT2,  RCP_SQRT2),
	FVector( RCP_SQRT2, 0.f,  RCP_SQRT2),
	FVector(-RCP_SQRT2, 0.f, -RCP_SQRT2),
	FVector( RCP_SQRT2, 0.f, -RCP_SQRT2),
	FVector(-RCP_SQRT2, 0.f,  RCP_SQRT2),
	FVector( RCP_SQRT2,  RCP_SQRT2, 0.f),
	FVector(-RCP_SQRT2, -RCP_SQRT2, 0.f),
	FVector( RCP_SQRT2, -RCP_SQRT2, 0.f),
	FVector(-RCP_SQRT2,  RCP_SQRT2, 0.f),
	FVector( RCP_SQRT3,  RCP_SQRT3,  RCP_SQRT3),
	FVector( RCP_SQRT3,  RCP_SQRT3, -RCP_SQRT3),
	FVector( RCP_SQRT3, -RCP_SQRT3,  RCP_SQRT3),
	FVector( RCP_SQRT3, -RCP_SQRT3, -RCP_SQRT3),
	FVector(-RCP_SQRT3,  RCP_SQRT3,  RCP_SQRT3),
	FVector(-RCP_SQRT3,  RCP_SQRT3, -RCP_SQRT3),
	FVector(-RCP_SQRT3, -RCP_SQRT3,  RCP_SQRT3),
	FVector(-RCP_SQRT3, -RCP_SQRT3, -RCP_SQRT3),
};

// Utilities
UNREALED_API int32 GenerateKDopAsSimpleCollision(UStaticMesh* StaticMesh, TArray<FVector> &dirs);
UNREALED_API int32 GenerateBoxAsSimpleCollision(UStaticMesh* StaticMesh);
UNREALED_API int32 GenerateSphereAsSimpleCollision(UStaticMesh* StaticMesh);
UNREALED_API int32 GenerateSphylAsSimpleCollision(UStaticMesh* StaticMesh);

/**
 * Refresh Collision Change
 * 
 * Collision has been changed, so it will need to recreate physics state to reflect it
 * Utilities functions to propagate BodySetup change for StaticMesh
 *
 * @param	StaticMesh	StaticMesh that collision has been changed for
 */
UNREALED_API void RefreshCollisionChange(const UStaticMesh * StaticMesh);
	