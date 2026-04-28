//==============================================================================
// 麻将游戏 - AI决策模块
// MahjongAIController.h
//
// 本文件定义AI决策控制器
// 实现策略层、动作层、出牌层的三层分离
//==============================================================================

#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "MahjongAIController.generated.h"

//==============================================================================
// AI决策层级
//==============================================================================
UENUM(BlueprintType)
enum class EMActionLevel : uint8
{
	Strategy,   // 策略层：整体决策
	Action,     // 动作层：碰/杠/胡/过
	Discard     // 出牌层：选择打哪张
};

//==============================================================================
// 出牌决策结果
//==============================================================================
USTRUCT(BlueprintType)
struct FMahjongAIDiscardResult
{
	GENERATED_BODY()

	// 选择打出的牌索引
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SelectedTileIndex;

	// 危险性评分 (0-100)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DangerScore;

	// 决策理由
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Reason;
};

//==============================================================================
// 动作决策结果
//==============================================================================
USTRUCT(BlueprintType)
struct FMahjongAIActionResult
{
	GENERATED_BODY()

	// 选择的动作 (0=过, 1=碰, 2=杠, 3=胡)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SelectedAction;

	// 是否可以使用该动作
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanExecute;

	// 动作评分
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ActionScore;

	// 决策理由
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Reason;
};

//==============================================================================
// 听牌信息
//==============================================================================
USTRUCT(BlueprintType)
struct FMahjongTingInfo
{
	GENERATED_BODY()

	// 可以胡的牌值列表
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> TingTiles;

	// 是否已经听牌
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsTing;

	// 听牌数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TingCount;
};

//==============================================================================
// AI决策控制器
// 分离策略/动作/出牌三层
//==============================================================================
UCLASS()
class MAHJONGGAME_API AMahjongAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMahjongAIController();

	//============================================================================
	// 策略层决策
	// 基于当前游戏状态做出整体决策
	//============================================================================
	UFUNCTION(BlueprintCallable, Category = "Mahjong|AI|Strategy")
	void MakeStrategyDecision();

	//============================================================================
	// 动作层决策
	// 决定是否碰/杠/胡/过
	//============================================================================
	UFUNCTION(BlueprintCallable, Category = "Mahjong|AI|Action")
	FMahjongAIActionResult MakeActionDecision(const TArray<int32>& HandTiles, int32 LastDiscardTile, bool bCanPeng, bool bCanGang, bool bCanHu);

	//============================================================================
	// 出牌层决策
	// 选择打哪张牌
	//============================================================================
	UFUNCTION(BlueprintCallable, Category = "Mahjong|AI|Discard")
	FMahjongAIDiscardResult MakeDiscardDecision(const TArray<int32>& HandTiles, bool bIsLastTurn);

	//============================================================================
	// 获取听牌信息
	//============================================================================
	UFUNCTION(BlueprintPure, Category = "Mahjong|AI|Ting")
	FMahjongTingInfo GetTingInfo(const TArray<int32>& HandTiles) const;

	//============================================================================
	// 神经网络训练数据收集
	//============================================================================
	// 记录玩家决策用于训练
	UFUNCTION(BlueprintCallable, Category = "Mahjong|AI|Training")
	void RecordPlayerDecision(int32 PlayerID, const TArray<int32>& HandTiles, int32 Action, int32 SelectedTile);

	// 获取训练数据
	UFUNCTION(BlueprintCallable, Category = "Mahjong|AI|Training")
	TArray<FString> GetTrainingData() const;

	// 导出训练数据到JSON
	UFUNCTION(BlueprintCallable, Category = "Mahjong|AI|Training")
	FString ExportTrainingDataToJSON() const;

	//============================================================================
	// 公共成员 - 可被测试访问
	//============================================================================
	UPROPERTY()
	int32 CurrentPlayerIndex;

	UPROPERTY()
	int32 RemainingTiles;

	UPROPERTY()
	int32 OpponentCount;

	// AI难度设置
	UPROPERTY(EditAnywhere, Category = "Mahjong|AI|Settings")
	float AIDifficulty = 0.7f;

	UPROPERTY(EditAnywhere, Category = "Mahjong|AI|Settings")
	bool bUseAggressiveStrategy = false;

protected:
	//============================================================================
	// 策略层私有方法
	//============================================================================
	void UpdateGameState();
	float EvaluateGameSituation() const;
	bool ShouldPlayAggressive() const;
	bool ShouldPlayDefensive() const;

	//============================================================================
	// 动作层私有方法
	//============================================================================
	float ScorePengAction(const TArray<int32>& HandTiles, int32 TileValue) const;
	float ScoreGangAction(const TArray<int32>& HandTiles, int32 TileValue) const;
	float ScoreHuAction(const TArray<int32>& HandTiles, int32 TileValue) const;

	//============================================================================
	// 出牌层私有方法
	//============================================================================
	float CalculateTileDanger(int32 TileValue, const TArray<int32>& HandTiles) const;
	float CalculateTileValue(int32 TileValue, const TArray<int32>& HandTiles) const;
	int32 FindSafeTileIndex(const TArray<int32>& HandTiles) const;
	int32 FindAggressiveTileIndex(const TArray<int32>& HandTiles) const;
	FString GetActionReason(int32 Action, float Score) const;

	//============================================================================
	// 神经网络训练数据收集
	//============================================================================
	// 训练数据记录
	UPROPERTY()
	TArray<FString> TrainingDataRecords;

	// 历史手牌记录（用于训练）
	TArray<TArray<int32>> HandHistory;

	// 历史出牌记录
	TArray<int32> DiscardHistory;
};