#include "Gameplay/Lobby/MahjongLobbyGameMode.h"

AMahjongLobbyGameMode::AMahjongLobbyGameMode()
{
    Rooms.Empty();
}

FString AMahjongLobbyGameMode::CreateRoom(const FString& RoomName, int32 MaxPlayers, AMahjongLobbyPlayerController* Creator)
{
    if (!Creator || RoomName.IsEmpty() || MaxPlayers <= 0)
    {
        return TEXT("");
    }

    FString RoomID = FString::Printf(TEXT("Room_%d"), Rooms.Num() + 1);

    FRoomInfo NewRoom;
    NewRoom.RoomID = RoomID;
    NewRoom.RoomName = RoomName;
    NewRoom.MaxPlayers = MaxPlayers;
    NewRoom.Players.Add(Creator);

    Rooms.Add(RoomID, NewRoom);
    return RoomID;
}

bool AMahjongLobbyGameMode::JoinRoom(const FString& RoomID, AMahjongLobbyPlayerController* Player)
{
    if (!Player || RoomID.IsEmpty())
    {
        return false;
    }

    FRoomInfo* Room = Rooms.Find(RoomID);
    if (Room && Room->Players.Num() < Room->MaxPlayers)
    {
        Room->Players.Add(Player);
        return true;
    }
    return false;
}

void AMahjongLobbyGameMode::GetRoomList(TArray<FString>& OutRoomIDs, TArray<FString>& OutRoomNames, TArray<int32>& OutPlayerCounts)
{
    OutRoomIDs.Empty();
    OutRoomNames.Empty();
    OutPlayerCounts.Empty();

    for (const auto& RoomPair : Rooms)
    {
        OutRoomIDs.Add(RoomPair.Value.RoomID);
        OutRoomNames.Add(RoomPair.Value.RoomName);
        OutPlayerCounts.Add(RoomPair.Value.Players.Num());
    }
}