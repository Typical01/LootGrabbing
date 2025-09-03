// Fill out your copyright notice in the Description page of Project Settings.


#include "Gacha.h"

#include <chrono>

#include "TypicalTool/Public/TypicalToolBPLib.h"
//#include "TypicalTool/Tool.h"



std::default_random_engine Gacha::RandomEngine;
std::uniform_real_distribution<double> Gacha::Uniform;


int64 Gacha::Initialize(const TArray<double>& _ProbabilityDistributionGather)
{
	double SumSum = 0.f; //总和
	//概率总和错误时
	for (int64 i = 0; i < ProbabilityDistributionGather.Num(); i++) {
		ProbabilityDistributionGather[i] = _ProbabilityDistributionGather[i];
		SumSum += _ProbabilityDistributionGather[i];
	}
	if (SumSum != 1.0)
	{
		return SumSum;
	}

	//初始化：0.025 == 2.5%
	static std::default_random_engine tempRandomEngine;
	RandomEngine = tempRandomEngine;
	static std::uniform_real_distribution<double> tempReal(0.0, 1.0);
	Uniform = tempReal;

	//使用当前时间作为随机数种子
	RandomEngine.seed(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
	return 1;
}

GachaProbability Gacha::TransformProbability(double Random)
{
	double Accumulation = 0.0;
	for (size_t i = 0; i < ProbabilityDistributionGather.Num(); ++i) {
		Accumulation += ProbabilityDistributionGather[i];
		if (Random < Accumulation) {  // 注意是 < 不是 <=
			return static_cast<GachaProbability>(i);
		}
	}
	return GachaProbability::Orange; // 处理浮点误差
}

void Gacha::StratGacha(int64 Count)
{
	//清除临时计数
	tempRawPrize.Empty();
	tempRawProbability.Empty();
	ProbabilityCountGather.SetNumZeroed(ProbabilityCountGather.Num());

	for (int i = 0; i < Count; i++)
	{
		this->tempRawProbability.Add(this->Uniform(this->RandomEngine)); //保存生成的随机数
		//计数
		switch (TransformProbability(this->tempRawProbability[i]))
		{
		case GachaProbability::White:
		{
			ProbabilityCountGather[0]++;
			ProbabilityCountGatherSum[0]++;
			tempRawPrize.Add(GachaProbability::White);
			break;
		}
		case GachaProbability::Green:
		{
			ProbabilityCountGather[1]++;
			ProbabilityCountGatherSum[1]++;
			tempRawPrize.Add(GachaProbability::Green);
			break;
		}
		case GachaProbability::Blue:
		{
			ProbabilityCountGather[2]++;
			ProbabilityCountGatherSum[2]++;
			tempRawPrize.Add(GachaProbability::Blue);
			break;
		}
		case GachaProbability::Purple:
		{
			ProbabilityCountGather[3]++;
			ProbabilityCountGatherSum[3]++;
			tempRawPrize.Add(GachaProbability::Purple);
			break;
		}
		case GachaProbability::Pink:
		{
			ProbabilityCountGather[4]++;
			ProbabilityCountGatherSum[4]++;
			tempRawPrize.Add(GachaProbability::Pink);
			break;
		}
		case GachaProbability::Gold:
		{
			ProbabilityCountGather[5]++;
			ProbabilityCountGatherSum[5]++;
			tempRawPrize.Add(GachaProbability::Gold);
			break;
		}
		case GachaProbability::Red:
		{
			ProbabilityCountGather[6]++;
			ProbabilityCountGatherSum[6]++;
			tempRawPrize.Add(GachaProbability::Red);
			break;
		}
		case GachaProbability::Orange:
		{
			ProbabilityCountGather[7]++;
			ProbabilityCountGatherSum[7]++;
			tempRawPrize.Add(GachaProbability::Orange);
			break;
		}
		}
	}
}

void Gacha::PrintProbability()
{
	UEtytl::DebugLog(FString::Printf(TEXT("Gacha::PrintProbability: 概率\n\
\t白(%f)%, 绿(%f)%, 蓝(%f)%, 紫(%f)%, \n\t粉(%f)%, 金(%f)%, 红(%f)%, 橙(%f)%."),
ProbabilityDistributionGather[0] * 100,
ProbabilityDistributionGather[1] * 100,
ProbabilityDistributionGather[2] * 100,
ProbabilityDistributionGather[3] * 100,
ProbabilityDistributionGather[4] * 100,
ProbabilityDistributionGather[5] * 100,
ProbabilityDistributionGather[6] * 100,
ProbabilityDistributionGather[7] * 100));
}

void Gacha::PrintCountOnce()
{
	int64 Sum = 0;
	for (auto& temp : ProbabilityCountGather) {
		Sum += temp;
	}
	UEtytl::DebugLog(FString::Printf(TEXT("Gacha::PrintProbability: 单次计数[%lld]\n\
\t白(%lld), 绿(%lld), 蓝(%lld), 紫(%lld), \n\t粉(%lld), 金(%lld), 红(%lld), 橙(%lld)."),
Sum,
ProbabilityCountGather[0],
ProbabilityCountGather[1],
ProbabilityCountGather[2],
ProbabilityCountGather[3],
ProbabilityCountGather[4],
ProbabilityCountGather[5],
ProbabilityCountGather[6],
ProbabilityCountGather[7]));
}

void Gacha::PrintCountSum()
{
	int64 Sum = 0;
	for (auto& temp : ProbabilityCountGatherSum) {
		Sum += temp;
	}
	UEtytl::DebugLog(FString::Printf(TEXT("Gacha::PrintCountSum: 总共计数[%lld]\n\
\t白(%lld), 绿(%lld), 蓝(%lld), 紫(%lld), \n\t粉(%lld), 金(%lld), 红(%lld), 橙(%lld)."),
Sum,
ProbabilityCountGatherSum[0],
ProbabilityCountGatherSum[1],
ProbabilityCountGatherSum[2],
ProbabilityCountGatherSum[3],
ProbabilityCountGatherSum[4],
ProbabilityCountGatherSum[5],
ProbabilityCountGatherSum[6],
ProbabilityCountGatherSum[7]));
}

void Gacha::Output()
{
	PrintProbability();
	PrintCountSum();
}

void Gacha::PrintPrizeCountRaw()
{
	UEtytl::DebugLog(FString::Printf(TEXT("Gacha::PrintCountRaw: tempRawPrize[%d]"), tempRawPrize.Num()));
	for (int i = 0; i < tempRawPrize.Num(); i++)
	{
		UEtytl::DebugLog(FString::Printf(TEXT("\tIndex[%d]: %s"), i, *TransformProbabilityToString(static_cast<GachaProbability>(tempRawPrize[i]))));
	} //
}

void Gacha::PrintProbabilityCountRaw()
{
	double tempMin = 1.f;

	UEtytl::DebugLog(FString::Printf(TEXT("Gacha::PrintProbabilityCountRaw: tempRawProbability[%d]"), tempRawProbability.Num()));
	for (int i = 0; i < tempRawProbability.Num(); i++)
	{
		if (Min(tempMin, tempRawProbability[i]) == 0)
		{
			tempMin = tempRawProbability[i];
		}
		UEtytl::DebugLog(FString::Printf(TEXT("\t%f, "), tempRawProbability[i]));
	}

	UEtytl::DebugLog(FString::Printf(TEXT("\tMin: %f"), tempMin));
}

double Gacha::Min(double a, double b)
{
	if (a < b) return 1;
	if (b < a) return 0;

	return -1;
}

FString TransformProbabilityToString(const GachaProbability& _GachaProbability)
{
	switch (_GachaProbability)
	{
	case GachaProbability::White:
	{
		return FString(TEXT("白"));
	}
	case GachaProbability::Green:
	{
		return FString(TEXT("绿"));
	}
	case GachaProbability::Blue:
	{
		return FString(TEXT("蓝"));
	}
	case GachaProbability::Purple:
	{
		return FString(TEXT("紫"));
	}
	case GachaProbability::Pink:
	{
		return FString(TEXT("粉"));
	}
	case GachaProbability::Gold:
	{
		return FString(TEXT("金"));
	}
	case GachaProbability::Red:
	{
		return FString(TEXT("红"));
	}
	case GachaProbability::Orange:
	{
		return FString(TEXT("橙"));
	}
	}
	return FString();
}

FString TransformGoodsSlotToString(const GoodsSlot& _GoodsSlot)
{
	return FString::Printf(TEXT("%d格"), static_cast<int32>(_GoodsSlot));
}
