// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    ValidWords = GetValidWords(HiddenWords);
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = ValidWords[FMath::RandRange(0, ValidWords.Num() - 1)];
    Lives = HiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("Welcome to the Bull Cow Game!"));
    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("You have %i attemps."), Lives);
    PrintLine(TEXT("Type in your guess and press ENTER\nto continue.."));
    PrintLine(TEXT("%s"), *HiddenWord);
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (HiddenWord == Guess)
    {
        PrintLine(TEXT("You win!\n"));
        EndGame();
        return;
    }

    if (HiddenWord.Len() != Guess.Len())
    {
        PrintLine(TEXT("The hidden word is %i letters long,\ntry again!"), HiddenWord.Len());
        return;
    }

    if (!IsIsogram(Guess))
    {
        PrintLine("No repeating letters allowed!\nTry again!");
        return;
    }

    Lives--;
    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You're out of lives."));
        PrintLine(TEXT("The hidden word was: %s\n"), *HiddenWord);
        EndGame();
        return;
    }

    PrintLine(TEXT("You have %i attemps."), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 i = 0; i < Word.Len() - 1; i++)
    {
        for (int32 j = i + 1; j < Word.Len(); j++)
        {
            if (Word[i] == Word[j])
            {
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& InitialList) const
{
    TArray<FString> Result;

    for (FString Word : InitialList) {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            Result.Emplace(Word);
        }
    }

    return Result;
}
