// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBlueprintFunctionLibrary.h"

FProjectilePredictionResult UMyBlueprintFunctionLibrary::ComputeProjectilePrediction(FVector P0, FVector V0, FVector P1, float s1, float m) {
	FProjectilePredictionResult result;

	
	FVector D = P0 - P1;
	float a = FMath::Square(V0.X) + FMath::Square(V0.Y) + FMath::Square(V0.Z) - FMath::Square(s1);
	float b = 2 * (D.X*V0.X + D.Y*V0.Y + D.Z*V0.Z - s1 * m);
	float c = FMath::Square(D.X) + FMath::Square(D.Y) + FMath::Square(D.Z) - FMath::Square(m);
	float d = b * b - 4 * a * c;

	if (d < 0) {
		return result;
	}

	float t1 = (-b + FMath::Sqrt(d)) / (2 * a);
	float t2 = (-b - FMath::Sqrt(d)) / (2 * a);

	float t = MinNonNegative(t1, t2);
	if (t < 0) {
		return result;
	}

	result.bSuccess = true;
	result.predictedImpact = V0 * t + P0;
	//result.projectileVelocity = (P0 - P1).GetSafeNormal() * s0;
	result.time = t;

	return result;
}


float UMyBlueprintFunctionLibrary::MinNonNegative(float a, float b) {
	if (a > 0 && b > 0) {
		return FMath::Min(a, b);
	} else if (a > 0) {
		return a;
	} else if (b > 0) {
		return b;
	}
	return -1.f;
}