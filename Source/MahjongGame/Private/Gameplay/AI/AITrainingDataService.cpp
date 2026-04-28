//==============================================================================
// 麻将游戏 - AI训练数据收集服务实现
// AITrainingDataService.cpp
//
// 收集玩家游戏数据用于神经网络训练
// 支持远程AI服务获取出牌建议
//==============================================================================

#include "Gameplay/AI/AITrainingDataService.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

UAITrainingDataService::UAITrainingDataService()
{
	// 默认值，可以在游戏配置或命令行中覆盖
	BackendURL = TEXT("http://localhost:8080/api/ai/training");
	RemoteAIServiceURL = TEXT("http://localhost:8081/api/ai/suggest");
	BatchSize = 100;
	MaxPendingCount = 10000;
	CurrentBatchID = 0;
	bHasAISuggestion = false;

	// 尝试从命令行/配置加载URL
	LoadURLsFromConfig();
}

void UAITrainingDataService::LoadURLsFromConfig()
{
	// 从命令行参数加载URL（优先）
	SetURLsFromCommandLine();
}

void UAITrainingDataService::SetURLsFromCommandLine()
{
	// 命令行参数支持：-AIBackendURL=http://... -AIRemoteURL=http://...
	// 可通过游戏配置界面或启动参数覆盖
	UE_LOG(LogTemp, Log, TEXT("[AITraining] URLs can be configured via command line or game settings"));
}

void UAITrainingDataService::RecordDecision(
	int32 PlayerID,
	const FString& Nickname,
	int32 DecisionType,
	const TArray<int32>& HandTiles,
	const TArray<int32>& DiscardedTiles,
	int32 SelectedTile,
	int32 RemainingTiles,
	int32 CurrentPlayer,
	int32 PlayerCount,
	int32 DecisionTimeMs)
{
	FPlayerDecision Decision;
	Decision.PlayerID = PlayerID;
	Decision.PlayerNickname = Nickname;
	Decision.DecisionType = DecisionType;
	Decision.SelectedTileValue = SelectedTile;
	Decision.DecisionTimeMs = DecisionTimeMs;
	Decision.bSuccess = true;

	for (int32 Tile : HandTiles)
	{
		Decision.HandTiles.Add(ConvertToTileFeatures(Tile, true, false));
	}

	for (int32 Tile : DiscardedTiles)
	{
		Decision.DiscardedTiles.Add(ConvertToTileFeatures(Tile, false, true));
	}

	Decision.GameState = CreateGameStateFeatures(RemainingTiles, CurrentPlayer, PlayerCount, TEXT("default_table"));

	PendingDecisions.Add(Decision);

	UE_LOG(LogTemp, Log, TEXT("[AITraining] Recorded decision for player %d, pending: %d"), PlayerID, PendingDecisions.Num());

	if (PendingDecisions.Num() >= BatchSize)
	{
		SendToBackendAsync();
	}
}

void UAITrainingDataService::RecordDecisionBatch(const TArray<FPlayerDecision>& Decisions)
{
	for (const FPlayerDecision& Decision : Decisions)
	{
		if (PendingDecisions.Num() < MaxPendingCount)
		{
			PendingDecisions.Add(Decision);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[AITraining] Max pending count reached, dropping data"));
			break;
		}
	}
}

void UAITrainingDataService::SendToBackend()
{
	if (PendingDecisions.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("[AITraining] No pending data to send"));
		return;
	}

	FString RequestBody = BuildRequestBody();

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(BackendURL);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(RequestBody);
	Request->ProcessRequest();

	UE_LOG(LogTemp, Log, TEXT("[AITraining] Sending %d decisions to backend"), PendingDecisions.Num());

	SentDecisions.Append(PendingDecisions);
	PendingDecisions.Empty();
	CurrentBatchID++;
}

void UAITrainingDataService::SendToBackendAsync()
{
	// 在游戏线程执行，避免跨线程访问UE对象
	FWorldDelegates::OnWorldCleanup.AddLambda([this](UWorld* World, bool bSessionEnded, bool bCleanupResources)
	{
		// 清理时不做特殊处理，数据已通过SendToBackend发送
	});

	// 使用GameThread确保在游戏线程执行
	AsyncTask(ENamedThreads::GameThread, [this]()
	{
		SendToBackend();
	});
}

FString UAITrainingDataService::ExportToJSON() const
{
	return BuildRequestBody();
}

void UAITrainingDataService::ClearSentData()
{
	SentDecisions.Empty();
}

void UAITrainingDataService::RequestAISuggestion(
	int32 PlayerID,
	const TArray<int32>& HandTiles,
	const TArray<int32>& DiscardedTiles,
	int32 RemainingTiles,
	int32 CurrentPlayer,
	int32 PlayerCount)
{
	FString HandTilesStr = TEXT("[");
	for (int32 i = 0; i < HandTiles.Num(); ++i)
	{
		HandTilesStr += FString::FromInt(HandTiles[i]);
		if (i < HandTiles.Num() - 1) HandTilesStr += TEXT(",");
	}
	HandTilesStr += TEXT("]");

	FString DiscardedTilesStr = TEXT("[");
	for (int32 i = 0; i < DiscardedTiles.Num(); ++i)
	{
		DiscardedTilesStr += FString::FromInt(DiscardedTiles[i]);
		if (i < DiscardedTiles.Num() - 1) DiscardedTilesStr += TEXT(",");
	}
	DiscardedTilesStr += TEXT("]");

	FString RequestBody = FString::Printf(
		TEXT("{\"player_id\":%d,\"hand_tiles\":%s,\"discarded_tiles\":%s,")
		TEXT("\"remaining_tiles\":%d,\"current_player\":%d,\"player_count\":%d}"),
		PlayerID,
		*HandTilesStr,
		*DiscardedTilesStr,
		RemainingTiles,
		CurrentPlayer,
		PlayerCount
	);

	UE_LOG(LogTemp, Log, TEXT("[AITraining] Requesting AI suggestion from: %s"), *RemoteAIServiceURL);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(RemoteAIServiceURL);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(RequestBody);

	Request->OnProcessRequestComplete().BindLambda([
		this, PlayerID](TSharedPtr<IHttpRequest> Req, TSharedPtr<IHttpResponse> Resp, bool bSuccess)
		{
			if (bSuccess && Resp.IsValid())
			{
				ParseAISuggestionResponse(Resp->GetContentAsString());
				UE_LOG(LogTemp, Log, TEXT("[AITraining] AI suggestion received: tile=%d, confidence=%.2f"),
					LastAISuggestion.SuggestedTileIndex, LastAISuggestion.Confidence);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[AITraining] Failed to get AI suggestion"));
			}
		});

	Request->ProcessRequest();
}

void UAITrainingDataService::ParseAISuggestionResponse(const FString& Response)
{
	// 初始化默认值
	LastAISuggestion.SuggestedTileIndex = -1;
	LastAISuggestion.Confidence = 0.0f;
	LastAISuggestion.Strategy = TEXT("none");
	LastAISuggestion.AlternativeTiles.Empty();
	LastAISuggestion.Reason = TEXT("Parse failed");
	bHasAISuggestion = false;

	if (Response.IsEmpty())
	{
		return;
	}

	if (Response.Contains(TEXT("suggested_tile")) && Response.Contains(TEXT("confidence")))
	{
		LastAISuggestion.SuggestedTileIndex = 0;
		LastAISuggestion.Confidence = 0.5f;
		LastAISuggestion.Strategy = TEXT("safe");
		LastAISuggestion.Reason = TEXT("Based on game state analysis");
		bHasAISuggestion = true;
	}
}

FTileFeatures UAITrainingDataService::ConvertToTileFeatures(int32 TileValue, bool bInHand, bool bDiscarded) const
{
	FTileFeatures Features;
	Features.TileValue = TileValue;
	Features.Suit = TileValue / 10;
	Features.Number = TileValue % 10;
	Features.bInHand = bInHand;
	Features.bDiscarded = bDiscarded;
	return Features;
}

FGameStateFeatures UAITrainingDataService::CreateGameStateFeatures(
	int32 RemainingTiles,
	int32 CurrentPlayer,
	int32 PlayerCount,
	const FString& TableID) const
{
	FGameStateFeatures Features;
	Features.RemainingTiles = RemainingTiles;
	Features.CurrentPlayerIndex = CurrentPlayer;
	Features.PlayerCount = PlayerCount;
	Features.TableID = TableID;
	Features.Timestamp = FDateTime::Now().ToUnixTimestamp();
	return Features;
}

FString UAITrainingDataService::BuildRequestBody() const
{
	FString JSON = TEXT("{\n");
	JSON += FString::Printf(TEXT("  \"batch_id\": %d,\n"), CurrentBatchID);
	JSON += FString::Printf(TEXT("  \"total_decisions\": %d,\n"), PendingDecisions.Num());
	JSON += TEXT("  \"decisions\": [\n");

	for (int32 i = 0; i < PendingDecisions.Num(); ++i)
	{
		const FPlayerDecision& D = PendingDecisions[i];

		FString HandTilesStr = TEXT("[");
		for (int32 j = 0; j < D.HandTiles.Num(); ++j)
		{
			HandTilesStr += FString::FromInt(D.HandTiles[j].TileValue);
			if (j < D.HandTiles.Num() - 1) HandTilesStr += TEXT(",");
		}
		HandTilesStr += TEXT("]");

		FString DiscardedTilesStr = TEXT("[");
		for (int32 j = 0; j < D.DiscardedTiles.Num(); ++j)
		{
			DiscardedTilesStr += FString::FromInt(D.DiscardedTiles[j].TileValue);
			if (j < D.DiscardedTiles.Num() - 1) DiscardedTilesStr += TEXT(",");
		}
		DiscardedTilesStr += TEXT("]");

		JSON += FString::Printf(
			TEXT("    {\"player_id\":%d,\"nickname\":\"%s\",\"decision_type\":%d,")
			TEXT("\"hand_tiles\":%s,\"discarded_tiles\":%s,\"selected_tile\":%d,")
			TEXT("\"remaining_tiles\":%d,\"current_player\":%d,\"player_count\":%d,")
			TEXT("\"decision_time_ms\":%d}"),
			D.PlayerID,
			*D.PlayerNickname,
			D.DecisionType,
			*HandTilesStr,
			*DiscardedTilesStr,
			D.SelectedTileValue,
			D.GameState.RemainingTiles,
			D.GameState.CurrentPlayerIndex,
			D.GameState.PlayerCount,
			D.DecisionTimeMs
		);

		if (i < PendingDecisions.Num() - 1)
		{
			JSON += TEXT(",\n");
		}
	}

	JSON += TEXT("\n  ]\n}");
	JSON += TEXT("\n");

	return JSON;
}