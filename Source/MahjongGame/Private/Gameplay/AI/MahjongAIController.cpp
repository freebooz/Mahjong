//==============================================================================
// 麻将游戏 - AI决策模块实现
// MahjongAIController.cpp
//
// 实现策略层、动作层、出牌层的三层分离
// 支持神经网络训练数据收集
//==============================================================================

#include "Gameplay/AI/MahjongAIController.h"
#include "Engine/World.h"
#include "TimerManager.h"

// AI模块常量
static constexpr int32 TOTAL_TILES = 144;           // 总牌数
static constexpr int32 HALF_TILES = 72;             // 半牌数
static constexpr int32 QUARTER_TILES = 36;         // 1/4牌数
static constexpr float SCORE_GUO = 50.0f;           // 过牌默认评分
static constexpr float SCORE_HU = 100.0f;            // 胡牌评分
static constexpr float SCORE_GANG_SAFE = 80.0f;     // 安全杠牌评分
static constexpr float SCORE_GANG_RISK = 60.0f;     // 风险杠牌评分
static constexpr float SCORE_PENG_SAFE = 80.0f;     // 安全碰牌评分
static constexpr float SCORE_PENG_RISK = 60.0f;     // 风险碰牌评分
static constexpr float SCORE_GANG_BEST = 100.0f;    // 最佳杠牌评分

AMahjongAIController::AMahjongAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentPlayerIndex = 0;
	RemainingTiles = TOTAL_TILES;
	OpponentCount = 3;
	AIDifficulty = 0.7f;
	bUseAggressiveStrategy = false;
}

void AMahjongAIController::MakeStrategyDecision()
{
	UpdateGameState();
	float SituationScore = EvaluateGameSituation();

	UE_LOG(LogTemp, Log, TEXT("[AI] Strategy Decision - Situation Score: %.2f"), SituationScore);

	if (SituationScore > 0.6f)
	{
		bUseAggressiveStrategy = true;
		UE_LOG(LogTemp, Log, TEXT("[AI] Strategy: Aggressive"));
	}
	else if (SituationScore < 0.4f)
	{
		bUseAggressiveStrategy = false;
		UE_LOG(LogTemp, Log, TEXT("[AI] Strategy: Defensive"));
	}
	else
	{
		bUseAggressiveStrategy = FMath::Rand() < AIDifficulty;
		UE_LOG(LogTemp, Log, TEXT("[AI] Strategy: Balanced"));
	}
}

FMahjongAIActionResult AMahjongAIController::MakeActionDecision(
	const TArray<int32>& HandTiles,
	int32 LastDiscardTile,
	bool bCanPeng,
	bool bCanGang,
	bool bCanHu)
{
	FMahjongAIActionResult Result;
	Result.bCanExecute = false;
	Result.SelectedAction = 0; // 默认选择"过"
	Result.ActionScore = 0.0f;

	TArray<float> ActionScores;

	// 评估各动作得分
	float ScoreGuo = SCORE_GUO; // 过牌默认50分
	float ScorePeng = ScorePengAction(HandTiles, LastDiscardTile);
	float ScoreGang = ScoreGangAction(HandTiles, LastDiscardTile);
	float ScoreHu = ScoreHuAction(HandTiles, LastDiscardTile);

	ActionScores.Add(ScoreGuo);
	ActionScores.Add(ScorePeng);
	ActionScores.Add(ScoreGang);
	ActionScores.Add(ScoreHu);

	// 找出最高分动作
	float MaxScore = ScoreGuo;
	int32 BestAction = 0;

	for (int32 i = 1; i < ActionScores.Num(); ++i)
	{
		if (ActionScores[i] > MaxScore)
		{
			MaxScore = ActionScores[i];
			BestAction = i;
		}
	}

	// 根据AI难度决定是否执行
	float RandomFactor = FMath::FRand();
	if (RandomFactor < AIDifficulty)
	{
		Result.bCanExecute = true;
		Result.SelectedAction = BestAction;
		Result.ActionScore = MaxScore;
	}
	else
	{
		// AI难度不高时，可能选择次优动作
		Result.SelectedAction = 0;
		Result.ActionScore = ScoreGuo;
	}

	// 确定是否可以执行
	switch (Result.SelectedAction)
	{
	case 1: Result.bCanExecute = bCanPeng; break;
	case 2: Result.bCanExecute = bCanGang; break;
	case 3: Result.bCanExecute = bCanHu; break;
	default: Result.bCanExecute = true; break;
	}

	Result.Reason = GetActionReason(Result.SelectedAction, MaxScore);

	return Result;
}

FMahjongAIDiscardResult AMahjongAIController::MakeDiscardDecision(
	const TArray<int32>& HandTiles,
	bool bIsLastTurn)
{
	FMahjongAIDiscardResult Result;
	Result.SelectedTileIndex = -1;
	Result.DangerScore = 100.0f;

	if (HandTiles.Num() == 0)
	{
		return Result;
	}

	// 策略决策
	MakeStrategyDecision();

	int32 TileIndex = 0;

	if (bUseAggressiveStrategy && !bIsLastTurn)
	{
		TileIndex = FindAggressiveTileIndex(HandTiles);
		Result.Reason = TEXT("进攻性出牌");
	}
	else
	{
		TileIndex = FindSafeTileIndex(HandTiles);
		Result.Reason = TEXT("保守性出牌");
	}

	Result.SelectedTileIndex = TileIndex;
	Result.DangerScore = CalculateTileDanger(HandTiles[TileIndex], HandTiles);

	return Result;
}

FMahjongTingInfo AMahjongAIController::GetTingInfo(const TArray<int32>& HandTiles) const
{
	FMahjongTingInfo TingInfo;
	TingInfo.bIsTing = false;
	TingInfo.TingCount = 0;

	// 简单实现：检查手牌中是否有对子可以听牌
	TMap<int32, int32> TileCount;
	for (int32 Tile : HandTiles)
	{
		TileCount.FindOrAdd(Tile)++;
	}

	// 检查是否有3+1的情况可以听牌
	for (const auto& Pair : TileCount)
	{
		if (Pair.Value >= 3)
		{
			// 听这张牌的上下牌
			int32 TingTile = Pair.Key;
			TingInfo.TingTiles.Add(TingTile);
			TingInfo.bIsTing = true;
		}
	}

	TingInfo.TingCount = TingInfo.TingTiles.Num();
	return TingInfo;
}

void AMahjongAIController::UpdateGameState()
{
	// 更新游戏状态
	RemainingTiles = FMath::Max(0, RemainingTiles - 1);
}

float AMahjongAIController::EvaluateGameSituation() const
{
	// 简单评估：牌多时进攻，牌少时保守
	float HealthScore = static_cast<float>(RemainingTiles) / TOTAL_TILES;
	return HealthScore;
}

bool AMahjongAIController::ShouldPlayAggressive() const
{
	return RemainingTiles > HALF_TILES; // 牌过半时进攻
}

bool AMahjongAIController::ShouldPlayDefensive() const
{
	return RemainingTiles < QUARTER_TILES; // 牌少于1/4时保守
}

float AMahjongAIController::ScorePengAction(const TArray<int32>& HandTiles, int32 TileValue) const
{
	if (!HandTiles.Contains(TileValue))
	{
		return 0.0f;
	}

	// 碰牌评分：看是否影响听牌
	int32 Count = 0;
	for (int32 Tile : HandTiles)
	{
		if (Tile == TileValue) Count++;
	}

	if (Count >= 3)
	{
		return SCORE_PENG_SAFE; // 碰牌安全
	}
	else if (Count >= 2)
	{
		return SCORE_PENG_RISK; // 可能影响听牌
	}

	return 0.0f;
}

float AMahjongAIController::ScoreGangAction(const TArray<int32>& HandTiles, int32 TileValue) const
{
	int32 Count = 0;
	for (int32 Tile : HandTiles)
	{
		if (Tile == TileValue) Count++;
	}

	if (Count >= 4)
	{
		return SCORE_GANG_BEST; // 杠牌最高分
	}
	else if (Count >= 3)
	{
		return SCORE_GANG_SAFE;
	}

	return 0.0f;
}

float AMahjongAIController::ScoreHuAction(const TArray<int32>& HandTiles, int32 TileValue) const
{
	// 胡牌直接满分
	return SCORE_HU;
}

float AMahjongAIController::CalculateTileDanger(int32 TileValue, const TArray<int32>& HandTiles) const
{
	// 简单危险性评估
	// 实际应用中需要考虑对手手牌等更多信息

	// 花色判断
	int32 Suit = TileValue / 10;
	int32 Value = TileValue % 10;

	float Danger = 0.0f;

	// 幺九牌更危险（可能影响金鸡）
	if (Value == 1 || Value == 9)
	{
		Danger += 20.0f;
	}

	// 单张牌更危险
	int32 Count = 0;
	for (int32 Tile : HandTiles)
	{
		if (Tile == TileValue) Count++;
	}

	if (Count == 1)
	{
		Danger += 30.0f;
	}

	return Danger;
}

float AMahjongAIController::CalculateTileValue(int32 TileValue, const TArray<int32>& HandTiles) const
{
	int32 Suit = TileValue / 10;
	int32 Value = TileValue % 10;

	float ValueScore = 0.0f;

	// 中张价值更高
	if (Value >= 3 && Value <= 7)
	{
		ValueScore = 70.0f;
	}
	else
	{
		ValueScore = 30.0f;
	}

	return ValueScore;
}

int32 AMahjongAIController::FindSafeTileIndex(const TArray<int32>& HandTiles) const
{
	int32 SafestIndex = 0;
	float MinDanger = FLT_MAX;

	for (int32 i = 0; i < HandTiles.Num(); ++i)
	{
		float Danger = CalculateTileDanger(HandTiles[i], HandTiles);
		if (Danger < MinDanger)
		{
			MinDanger = Danger;
			SafestIndex = i;
		}
	}

	return SafestIndex;
}

int32 AMahjongAIController::FindAggressiveTileIndex(const TArray<int32>& HandTiles) const
{
	// 进攻性出牌：打掉不影响听牌的牌
	int32 BestIndex = 0;
	float MaxValue = -1.0f;

	for (int32 i = 0; i < HandTiles.Num(); ++i)
	{
		float TileVal = CalculateTileValue(HandTiles[i], HandTiles);
		if (TileVal > MaxValue)
		{
			MaxValue = TileVal;
			BestIndex = i;
		}
	}

	return BestIndex;
}

FString AMahjongAIController::GetActionReason(int32 Action, float Score) const
{
	switch (Action)
	{
	case 0: return TEXT("过牌");
	case 1: return TEXT("碰牌");
	case 2: return TEXT("杠牌");
	case 3: return TEXT("胡牌");
	default: return TEXT("未知动作");
	}
}

void AMahjongAIController::RecordPlayerDecision(
	int32 PlayerID,
	const TArray<int32>& HandTiles,
	int32 Action,
	int32 SelectedTile)
{
	// 记录决策数据用于神经网络训练
	FString Record = FString::Printf(
		TEXT("{\"player\":%d,\"hand\":[%s],\"action\":%d,\"tile\":%d,\"timestamp\":%lld}"),
		PlayerID,
		*FString::JoinBy(HandTiles, TEXT(","), [](int32 Tile) { return FString::FromInt(Tile); }),
		Action,
		SelectedTile,
		FDateTime::Now().ToUnixTimestamp()
	);

	TrainingDataRecords.Add(Record);
	HandHistory.Add(HandTiles);
	DiscardHistory.Add(SelectedTile);

	UE_LOG(LogTemp, Log, TEXT("[AI] Recorded decision: %s"), *Record);
}

TArray<FString> AMahjongAIController::GetTrainingData() const
{
	return TrainingDataRecords;
}

FString AMahjongAIController::ExportTrainingDataToJSON() const
{
	FString JSON = TEXT("{\"training_data\":[");
	for (int32 i = 0; i < TrainingDataRecords.Num(); ++i)
	{
		JSON += TrainingDataRecords[i];
		if (i < TrainingDataRecords.Num() - 1)
		{
			JSON += TEXT(",");
		}
	}
	JSON += TEXT("]}");
	return JSON;
}