
#include "PlayerCharacter.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &APlayerCharacter::StartCrouch);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &APlayerCharacter::EndCrouch);
}

void APlayerCharacter::MoveForward(float AxisValue) {
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void APlayerCharacter::MoveRight(float AxisValue) {
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void APlayerCharacter::StartCrouch() {
	Crouch();
}

void APlayerCharacter::EndCrouch() {
	UnCrouch();
}

