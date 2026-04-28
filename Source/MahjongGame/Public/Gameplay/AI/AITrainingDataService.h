//==============================================================================
// 麻将游戏 - AI训练数据收集服务
// AITrainingDataService.h
//
// 本服务负责收集玩家游戏数据并发送到后端进行神经网络训练
// 支持批量上报和数据缓存
//==============================================================================

#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AITrainingDataService.generated.h"

//==============================================================================
// 训练数据结构
//==============================================================================
USTRUCT(BlueprintType)
struct FTileFeatures
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TileValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Suit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Number;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDiscarded;
};

//==============================================================================
// 游戏状态特征
//==============================================================================
USTRUCT(BlueprintType)
struct FGameStateFeatures
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RemainingTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentPlayerIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TableID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 Timestamp;
};

//==============================================================================
// 玩家决策记录
//==============================================================================
USTRUCT(BlueprintType)
struct FPlayerDecision
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PlayerNickname;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DecisionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTileFeatures> HandTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTileFeatures> DiscardedTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SelectedTileValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameStateFeatures GameState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DecisionTimeMs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSuccess;
};

//==============================================================================
// AI远程建议结果
//==============================================================================
USTRUCT(BlueprintType)
struct FAIRemoteSuggestion
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SuggestedTileIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Confidence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Strategy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> AlternativeTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Reason;
};

//==============================================================================
// AI训练数据服务
//==============================================================================
UCLASS(Blueprintable)
class MAHJONGGAME_API UAITrainingDataService : public UObject
{
	GENERATED_BODY()

public:
	UAITrainingDataService();

	//============================================================================
	// 记录玩家决策
	//============================================================================
	UFUNCTION(BlueprintCallable, Category = "Mahjong|AI|Training")
	void RecordDecision(
		int32 PlayerID,
		const FString& Nickname,
		int32 DecisionType,
		const TArray<int32>& HandTiles,
		const TArray<int32>& DiscardedTiles,
		int32 SelectedTile,
		int32 RemainingTiles,
		int32 CurrentPlayer,
		int32 PlayerCount,
		int32 DecisionTimeMs
	);

	UFUNCTION(BlueprintCallable, Category = "Mahjong|AI|Training")
	void RecordDecisionBatch(const TArray<FPlayerDecision>& Decisions);

	UFUNCTION(BlueprintCallable, Category = "Mahjong|AI|Training")
	void SendToBackend();

	UFUNCTION(BlueprintCallable, Category = "Mahjong|AI|Training")
	void SendToBackendAsync();

	UFUNCTION(BlueprintPure, Category = "Mahjong|AI|Training")
	int32 GetPendingCount() const { return PendingDecisions.Num(); }

	UFUNCTION(BlueprintCallable, Category = "Mahjong|AI|Training")
	void ClearSentData();

	UFUNCTION(BlueprintCallable, Category = "Mahjong|AI|Training")
	void SetBackendURL(const FString& URL) { BackendURL = URL; }

	UFUNCTION(BlueprintPure, Category = "Mahjong|AI|Training")
	FString GetBackendURL() const { return BackendURL; }

	// 设置远程AI服务URL
	UFUNCTION(BlueprintCallable, Category = "Mahjong|AI|Remote")
	void SetRemoteAIServiceURL(const FString& URL) { RemoteAIServiceURL = URL; }

	// 获取远程AI服务URL
	UFUNCTION(BlueprintPure, Category = "Mahjong|AI|Remote")
	FString GetRemoteAIServiceURL() const { return RemoteAIServiceURL; }

	// 从配置文件或命令行加载URL
	UFUNCTION(BlueprintCallable, Category = "Mahjong|AI|Config")
	void LoadURLsFromConfig();

	UFUNCTION(BlueprintCallable, Category = "Mahjong|AI|Config")
	void SetURLsFromCommandLine();

	UFUNCTION(BlueprintCallable, Category = "Mahjong|AI|Training")
	FString ExportToJSON() const;

	//============================================================================
	// 远程AI服务 - 获取出牌建议
	//============================================================================
	UFUNCTION(BlueprintCallable, Category = "Mahjong|AI|Remote")
	void RequestAISuggestion(
		int32 PlayerID,
		const TArray<int32>& HandTiles,
		const TArray<int32>& DiscardedTiles,
		int32 RemainingTiles,
		int32 CurrentPlayer,
		int32 PlayerCount
	);

	UFUNCTION(BlueprintPure, Category = "Mahjong|AI|Remote")
	FAIRemoteSuggestion GetLastAISuggestion() const { return LastAISuggestion; }

	UFUNCTION(BlueprintPure, Category = "Mahjong|AI|Remote")
	bool HasAISuggestion() const { return bHasAISuggestion; }

protected:
	FTileFeatures ConvertToTileFeatures(int32 TileValue, bool bInHand, bool bDiscarded) const;
	FGameStateFeatures CreateGameStateFeatures(int32 RemainingTiles, int32 CurrentPlayer, int32 PlayerCount, const FString& TableID) const;
	FString BuildRequestBody() const;
	void ParseAISuggestionResponse(const FString& Response);

private:
	UPROPERTY(EditAnywhere, Category = "Mahjong|AI|Training|Config")
	FString BackendURL;

	UPROPERTY(EditAnywhere, Category = "Mahjong|AI|Training|Config")
	FString RemoteAIServiceURL;

	UPROPERTY(EditAnywhere, Category = "Mahjong|AI|Training|Config")
	int32 BatchSize = 100;

	UPROPERTY(EditAnywhere, Category = "Mahjong|AI|Training|Config")
	int32 MaxPendingCount = 10000;

	UPROPERTY()
	TArray<FPlayerDecision> PendingDecisions;

	UPROPERTY()
	TArray<FPlayerDecision> SentDecisions;

	UPROPERTY()
	int32 CurrentBatchID;

	UPROPERTY()
	FAIRemoteSuggestion LastAISuggestion;

	UPROPERTY()
	bool bHasAISuggestion;
};