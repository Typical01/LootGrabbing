// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"

#include <random>



//奖品概率
enum GachaProbability : int8_t {
	White = 0,	// 白: 1
	Green,		// 绿: 2
	Blue,		// 蓝: 3
	Purple,		// 紫: 4
	Pink,		// 粉: 5
	Gold,		// 金: 6
	Red,		// 红: 7
	Orange		// 橙: 8
};

FString TransformProbabilityToString(const GachaProbability& _GachaProbability);

/**
 * 抽奖
 */
class TYPICALTOOL_API Gacha
{
private:
	static std::default_random_engine RandomEngine; //随机数引擎
	static std::uniform_real_distribution<double> Uniform; //分布

	TArray<double> ProbabilityDistributionGather; // 概率分布集合
	TArray<int64> ProbabilityCountGather; //概率计数集合
	TArray<int64> ProbabilityCountGatherSum; //概率计数集合: 总计
	TArray<int64> tempRawPrize; //临时 奖品统计
	TArray<double> tempRawProbability; //临时 概率统计

public:
	Gacha()
	{
		ProbabilityDistributionGather.Init(8, 0);
		ProbabilityCountGather.Init(8, 0);
		ProbabilityCountGatherSum.Init(8, 0);
	}
	~Gacha() {};

	/// <summary>
	/// 初始化
	/// </summary>
	/// <param name="ProbabilityDistributionGather">概率分布集合</param>
	/// <returns>-1: 概率总和不为1</returns>
	int64 Initialize(const TArray<double>& _ProbabilityDistributionGather);

	/// <summary>
	/// 开始抽奖: 刷新临时数据集合(随机数原始数据, 单次抽奖计数)
	/// </summary>
	/// <param name="Count">抽奖次数</param>
	/// <returns></returns>
	void StratGacha(int64 Count);
	// 打印: 概率
	void PrintProbability();
	// 打印: 单次抽奖次数
	void PrintCountOnce();
	// 打印: 总共抽奖次数
	void PrintCountSum();
	// 打印: 临时奖品数
	void PrintPrizeCountRaw();
	// 打印: 临时概率数
	void PrintProbabilityCountRaw();
	void Output();

public:
	//获取 奖品原始数据(临时)
	TArray<int64> GetRawPrize() { return tempRawPrize; }
	//获取 随机数原始数据(临时)
	TArray<double> GetRawProbability() { return tempRawProbability; }
	TArray<int64> GetCountOnce() { return ProbabilityCountGather; }
	TArray<int64> GetCountSum() { return ProbabilityCountGatherSum; }
	TArray<double> GetProbabilityDistributionGather() { return ProbabilityDistributionGather; }
	std::default_random_engine& GetRandomEngine() { return RandomEngine; }

private:
	/// <summary>
	/// 转换概率: [0, 1]
	/// </summary>
	/// <param name="Random">随机数</param>
	/// <returns>抽到的概率</returns>
	GachaProbability TransformProbability(double Random);
	double Min(double a, double b);
};



//物品格数
enum GoodsSlot : uint8_t {
	Slot_1 = 0, //1格
	Slot_2,		//2格
	Slot_4,		//4格
	Slot_6,		//6格
	Slot_9,		//9格
};
//摆放状态
enum PutState : uint8_t {
	Horizontal = 0, //水平
	Vertical		//垂直
};

class Goods
{
private:
	FString GoodsName; //物品名
	GoodsSlot Slot; //格数
	int32 CurrentSlotIndex; //当前所在格数索引
	PutState State; //摆放状态

public:
	Goods(FString _GoodsName, GoodsSlot _Slot, int32 _CurrentSlotIndex, PutState _State)
		: GoodsName(_GoodsName), Slot(_Slot), CurrentSlotIndex(_CurrentSlotIndex), State(_State) {
	}

public:
	FString GetGoodsName() const { return GoodsName; }
	GoodsSlot GetGoodsSlot() const { return Slot; }
	PutState GetPutState() const { return State; }
	int32 GetCurrentSlotIndex() const { return CurrentSlotIndex; }

	void SetGoodsName(FString& _GoodsName) { GoodsName = _GoodsName; }
	void SetGoodsSlot(GoodsSlot& _Slot) { Slot = _Slot; }
	void SetPutState(PutState& _State) { State = _State; }
	void SetCurrentSlotIndex(int32& _CurrentSlotIndex) { CurrentSlotIndex = _CurrentSlotIndex; }
};

/// <summary>
/// 容器槽位是否有效: 根据当前容器剩余槽位, 判断物品摆放状态是否合法
/// </summary>
/// <param name="_Goods">物品</param>
/// <param name="_PutState">摆放状态</param>
/// <param name="_ContainerArray">容器数组</param>
/// <param name="_ContainerWidth">容器宽度</param>
bool IsContainerSlotValid(const Goods& _Goods, const PutState& _PutState, TArray<int32> _ContainerArray, int32 _ContainerWidth);

void GachaGame_Goods_Test(Gacha& game);