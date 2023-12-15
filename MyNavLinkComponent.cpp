// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNavLinkComponent.h"

void UMyNavLinkComponent::SetupLink(FVector Left, FVector Right, ENavLinkDirection::Type Direction)
{
    this->Links[0].Left = Left;
    this->Links[0].Right = Right;
    this->Links[0].Direction = Direction;
}