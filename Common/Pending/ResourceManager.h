#pragma once

#include <unordered_map>

struct RewardData {
	int nExp;
	int nCoin;
	int nGem;
};

namespace ResourceManager {
	void Initiate();

	const RewardData& GetRewardData( int nCtxID );
}