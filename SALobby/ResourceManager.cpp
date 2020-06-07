#include "ResourceManager.h"

#include "CSV.h"

typedef std::list<std::unordered_map<std::string, std::string> > UNIT_CSV;

std::unordered_map<int, RewardData> m_mReward;

namespace ResourceManager {
	void LoadReward();
}

namespace ResourceManager {

	void Initiate()
	{
		LoadReward();
	}

	void LoadReward()
	{
		UNIT_CSV lmReward = CSV::Load( "Reward_First.csv" );
		for ( auto& mReward : lmReward ) {
			std::string sReward = mReward[ "ID" ];
			if ( sReward.empty() ) continue;

			RewardData rewardData;
			rewardData.nExp = atoi( mReward[ "Exp" ].c_str() );
			rewardData.nCoin = atoi( mReward[ "Coin" ].c_str() );
			rewardData.nGem = atoi( mReward[ "Gem" ].c_str() );

			int nID = atoi( mReward[ "ID" ].c_str() );
			m_mReward[ nID ] = rewardData;
		}
	}

	const RewardData& GetRewardData( int nCtxID )
	{
		return m_mReward[ nCtxID ];
	}

}